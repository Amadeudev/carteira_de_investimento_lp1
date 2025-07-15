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
    int numero_linhas = 0;
    FILE *arquivo_acoes = fopen("dados_acoes.txt", "r");

    if (arquivo_acoes == NULL) {
        printf("Erro: Nao foi possivel abrir o arquivo 'dados_acoes.txt'.\n");
        return 1;
    }

    char buffer_linha[256];
    while (fgets(buffer_linha, sizeof(buffer_linha), arquivo_acoes) != NULL) {
        numero_linhas++;
    }

    if (numero_linhas == 0) {
        printf("Erro: O arquivo 'dados_acoes.txt' esta vazio.\n");
        fclose(arquivo_acoes);
        return 0;
    }

    Acoes *acoes_disponiveis = (Acoes *)malloc(numero_linhas * sizeof(Acoes));
    if (acoes_disponiveis == NULL) {
        printf("Erro: Falha na alocacao de memoria para as acoes.\n");
        fclose(arquivo_acoes);
        return 1;
    }

    rewind(arquivo_acoes);
    for (int i = 0; i < numero_linhas; i++) {
        if (fscanf(arquivo_acoes, "%s %lf %lf %s", acoes_disponiveis[i].sigla, 
                   &acoes_disponiveis[i].valor, &acoes_disponiveis[i].retorno, 
                   acoes_disponiveis[i].nome) != 4) {
            printf("Erro: Falha ao ler dados na linha %d do arquivo.\n", i + 1);
            free(acoes_disponiveis);
            fclose(arquivo_acoes);
            return 1;
        }
        acoes_disponiveis[i].retorno /= 100.0;
    }
    fclose(arquivo_acoes);

    double capital_investimento;
    printf("----------------------------------------\n");
    printf("  Otimizacao de Carteira de Investimentos\n");
    printf("----------------------------------------\n\n");
    printf("Digite o capital total para investir (R$): ");
    scanf("%lf", &capital_investimento);

    int capital_discreto = (int)round(capital_investimento / 10.0);
    if (capital_discreto < 0) capital_discreto = 0;

    double **dp = (double **)malloc((numero_linhas + 1) * sizeof(double *));
    if (dp == NULL) {
        printf("Erro: Falha na alocacao de memoria para a matriz DP.\n");
        free(acoes_disponiveis);
        return 1;
    }
    for (int i = 0; i <= numero_linhas; i++) {
        dp[i] = (double *)calloc((capital_discreto + 1), sizeof(double));
        if (dp[i] == NULL) {
            printf("Erro: Falha na alocacao de memoria para linha %d da matriz DP.\n", i);
            for (int k = 0; k < i; k++) free(dp[k]);
            free(dp);
            free(acoes_disponiveis);
            return 1;
        }
    }

        for (int i = 1; i <= numero_linhas; i++) {
        int custo_acao_atual_discreto = (int)round(acoes_disponiveis[i-1].valor / 10.0);
        for (int j = 1; j <= capital_discreto; j++) {
            if (custo_acao_atual_discreto <= j) {
                double retorno_incluindo = acoes_disponiveis[i-1].retorno + dp[i-1][j - custo_acao_atual_discreto];
                double retorno_excluindo = dp[i-1][j];
                
                if (retorno_incluindo > retorno_excluindo) {
                    dp[i][j] = retorno_incluindo;
                } else {
                    dp[i][j] = retorno_excluindo;
                }
            } else {
                dp[i][j] = dp[i-1][j];
            }
        }
    }

    double retorno_maximo_total = dp[numero_linhas][capital_discreto];
    double custo_final_carteira = 0.0;
    int *acoes_selecionadas = (int *)calloc(numero_linhas, sizeof(int));
    if (acoes_selecionadas == NULL) {
        printf("Erro: Falha na alocacao de memoria para o array de selecao.\n");
        for (int i = 0; i <= numero_linhas; i++) free(dp[i]);
        free(dp);
        free(acoes_disponiveis);
        return 1;
    }

    int i_back = numero_linhas;
    int j_back = capital_discreto;
    while (i_back > 0 && j_back >= 0) {
        if (dp[i_back][j_back] != dp[i_back-1][j_back]) {
            acoes_selecionadas[i_back-1] = 1;
            custo_final_carteira += acoes_disponiveis[i_back-1].valor;
            j_back -= (int)round(acoes_disponiveis[i_back-1].valor / 10.0);
        }
        i_back--;
    }

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
        printf("Nenhuma acao pode ser comprada com o capital disponivel.\n");
    }

    printf("\nResumo da Carteira Otimizada:\n");
    printf("- Custo Total Estimado: R$%.2f\n", custo_final_carteira);
    printf("- Retorno Anual Esperado: %.1f%%\n", retorno_maximo_total * 100.0);
    printf("----------------------------------------\n");

    for (int i = 0; i <= numero_linhas; i++) free(dp[i]);
    free(dp);
    free(acoes_disponiveis);
    free(acoes_selecionadas);

    return 0;
}