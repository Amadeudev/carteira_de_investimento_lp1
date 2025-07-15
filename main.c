#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef struct {
    char sigla[10];
    double valor;
    double retorno;
    char nome[100];
} Acoes;

int main() {
    char nome_arquivo_cenario[50];
    char caminho_completo_arquivo[100]; // Para armazenar o caminho completo do arquivo

    printf("----------------------------------------\n");
    printf("  Otimizacao de Carteira de Investimentos\n");
    printf("----------------------------------------\n\n");
    printf("Digite o nome do arquivo de cenario (ex: iniciante.txt, moderado.txt): ");
    scanf("%s", nome_arquivo_cenario);

    // Constrói o caminho completo para o arquivo.
    // Ajuste "Cenarios/" se seus arquivos TXT estiverem em outro local
    // Por exemplo, se estivessem na mesma pasta do executável, seria apenas strcpy(caminho_completo_arquivo, nome_arquivo_cenario);
    // Se main.exe está em 'output' e os .txt estão em 'Cenarios', use "../Cenarios/"
    sprintf(caminho_completo_arquivo, "../Cenarios/%s", nome_arquivo_cenario);


    int numero_linhas = 0;
    FILE *arquivo_acoes = fopen(caminho_completo_arquivo, "r"); // Abre o arquivo

    if (arquivo_acoes == NULL) {
        printf("Erro: Nao foi possivel abrir o arquivo '%s'. Verifique o nome e o caminho.\n", caminho_completo_arquivo);
        return 1;
    }

    char buffer_linha[256];
    // Conta o número de linhas de dados válidas no arquivo
    while (fgets(buffer_linha, sizeof(buffer_linha), arquivo_acoes) != NULL) {
        // Ignora linhas de comentário (começam com '#') e linhas vazias
        if (buffer_linha[0] != '#' && strlen(buffer_linha) > 1 && buffer_linha[0] != '\n') {
             numero_linhas++;
        }
    }

    if (numero_linhas == 0) {
        printf("Erro: O arquivo '%s' esta vazio ou nao contem dados de acoes validos.\n", caminho_completo_arquivo);
        fclose(arquivo_acoes);
        return 0;
    }

    // Aloca dinamicamente o array de estruturas Acoes
    Acoes *acoes_disponiveis = (Acoes *)malloc(numero_linhas * sizeof(Acoes));
    if (acoes_disponiveis == NULL) {
        printf("Erro: Falha na alocacao de memoria para as acoes.\n");
        fclose(arquivo_acoes);
        return 1;
    }

    rewind(arquivo_acoes); // Volta o ponteiro do arquivo para o início para reler os dados
    int acao_idx = 0;
    // Lê os dados de cada ação do arquivo
    while (fgets(buffer_linha, sizeof(buffer_linha), arquivo_acoes) != NULL && acao_idx < numero_linhas) {
        if (buffer_linha[0] != '#' && strlen(buffer_linha) > 1 && buffer_linha[0] != '\n') {
            // Extrai os dados da linha. Assume que os dados sao ID Custo Retorno Ticker
            // O retorno no arquivo é em percentagem (ex: 10.0 para 10%)
            if (sscanf(buffer_linha, "%s %lf %lf %s", acoes_disponiveis[acao_idx].sigla, 
                       &acoes_disponiveis[acao_idx].valor, &acoes_disponiveis[acao_idx].retorno, 
                       acoes_disponiveis[acao_idx].nome) == 4) {
                acoes_disponiveis[acao_idx].retorno /= 100.0; // Converte retorno para decimal
                acao_idx++;
            } else {
                // Mensagem de aviso para linhas com formato inesperado
                printf("Aviso: Linha com formato invalido ignorada: '%s'\n", buffer_linha);
            }
        }
    }
    fclose(arquivo_acoes); // Fecha o arquivo após a leitura

    double capital_investimento;
    // Solicita o capital de investimento ao usuário
    printf("\nCapital disponivel: Dados carregados do cenario '%s'.\n", nome_arquivo_cenario);
    printf("Digite o capital total para investir (R$): ");
    scanf("%lf", &capital_investimento);

    // Discretiza o capital de investimento para uso na matriz DP (cada unidade = R$10.00)
    int capital_discreto = (int)round(capital_investimento / 10.0);
    if (capital_discreto < 0) capital_discreto = 0; // Garante que o capital não seja negativo

    // Aloca a matriz de Programação Dinâmica (DP)
    // dp[i][j] armazenará o retorno máximo com 'i' ações e 'j' capital discreto
    double **dp = (double **)malloc((numero_linhas + 1) * sizeof(double *));
    if (dp == NULL) {
        printf("Erro: Falha na alocacao de memoria para a matriz DP.\n");
        free(acoes_disponiveis);
        return 1;
    }
    for (int i = 0; i <= numero_linhas; i++) {
        dp[i] = (double *)calloc((capital_discreto + 1), sizeof(double)); // Inicializa com zeros
        if (dp[i] == NULL) {
            printf("Erro: Falha na alocacao de memoria para linha %d da matriz DP.\n", i);
            for (int k = 0; k < i; k++) free(dp[k]); // Libera as linhas já alocadas em caso de erro
            free(dp);
            free(acoes_disponiveis);
            return 1;
        }
    }

    // Preenchimento da matriz DP (Algoritmo da Mochila 0/1)
    // i: índice da ação atual (de 1 a numero_linhas)
    // j: capacidade atual da "mochila" (capital discreto)
    for (int i = 1; i <= numero_linhas; i++) {
        // Custo da ação atual, também discretizado
        int custo_acao_atual_discreto = (int)round(acoes_disponiveis[i-1].valor / 10.0);
        for (int j = 1; j <= capital_discreto; j++) {
            // Verifica se a ação atual cabe no capital disponível
            if (custo_acao_atual_discreto <= j) {
                // Opção 1: Incluir a ação atual
                // Retorno da ação atual + retorno máximo do capital restante com as ações anteriores
                double retorno_incluindo = acoes_disponiveis[i-1].retorno + dp[i-1][j - custo_acao_atual_discreto];
                // Opção 2: Não incluir a ação atual
                // Retorno máximo com as ações anteriores e o mesmo capital
                double retorno_excluindo = dp[i-1][j];
                
                // Escolhe o maior retorno entre incluir ou excluir a ação
                if (retorno_incluindo > retorno_excluindo) {
                    dp[i][j] = retorno_incluindo;
                } else {
                    dp[i][j] = retorno_excluindo;
                }
            } else {
                // Se a ação não cabe, o retorno é o mesmo que não a incluindo
                dp[i][j] = dp[i-1][j];
            }
        }
    }

    // O retorno máximo total está na última célula da matriz DP
    double retorno_maximo_total = dp[numero_linhas][capital_discreto];
    double custo_final_carteira = 0.0; // Variável para acumular o custo da carteira otimizada

    // Array para marcar quais ações foram selecionadas (1 para selecionada, 0 para não)
    int *acoes_selecionadas = (int *)calloc(numero_linhas, sizeof(int));
    if (acoes_selecionadas == NULL) {
        printf("Erro: Falha na alocacao de memoria para o array de selecao.\n");
        // Libera a memória da matriz DP em caso de erro aqui
        for (int i = 0; i <= numero_linhas; i++) free(dp[i]);
        free(dp);
        free(acoes_disponiveis);
        return 1;
    }

    // Backtracking para determinar quais ações foram incluídas na carteira ótima
    int i_back = numero_linhas;
    int j_back = capital_discreto;
    while (i_back > 0 && j_back >= 0) { // Loop para trás através da matriz DP
        // Se o valor na célula atual for diferente do valor na célula acima (sem a ação atual),
        // isso significa que a ação atual foi incluída para chegar a este valor.
        if (dp[i_back][j_back] != dp[i_back-1][j_back]) {
            acoes_selecionadas[i_back-1] = 1; // Marca a ação como selecionada
            custo_final_carteira += acoes_disponiveis[i_back-1].valor; // Adiciona o custo da ação
            // Subtrai o custo da ação do capital para mover para o estado anterior da mochila
            j_back -= (int)round(acoes_disponiveis[i_back-1].valor / 10.0);
        }
        i_back--; // Move para a ação anterior
    }

    // Exibição dos resultados da otimização
    printf("\n----------------------------------------\n");
    printf("  Resultado da Otimizacao\n");
    printf("----------------------------------------\n");
    printf("Capital Disponivel: R$ %.2f\n\n", capital_investimento);

    printf("Acoes Sugeridas para Compra:\n");
    int total_acoes_compradas = 0;
    for (int k = 0; k < numero_linhas; k++) {
        if (acoes_selecionadas[k] == 1) {
            printf("- %s (%s) | Custo: R$%.2f | Retorno: %.1f%%\n",
                   acoes_disponiveis[k].sigla, acoes_disponiveis[k].nome, 
                   acoes_disponiveis[k].valor, acoes_disponiveis[k].retorno * 100.0);
            total_acoes_compradas++;
        }
    }

    if (total_acoes_compradas == 0) {
        printf("Nenhuma acao pode ser comprada com o capital disponivel neste cenario.\n");
    }

    printf("\nResumo da Carteira Otimizada:\n");
    printf("- Custo Total Estimado: R$%.2f\n", custo_final_carteira);
    printf("- Retorno Anual Esperado: %.1f%%\n", retorno_maximo_total * 100.0);
    printf("----------------------------------------\n");

    // Libera toda a memória alocada dinamicamente para evitar vazamentos
    for (int i = 0; i <= numero_linhas; i++) free(dp[i]);
    free(dp);
    free(acoes_disponiveis);
    free(acoes_selecionadas);

    return 0; // Indica que o programa terminou com sucesso
}