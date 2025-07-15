#include <stdio.h>
#include <stdlib.h>
#include <locale.h>

typedef struct{
    char sigla[10];
    double valor;
    double retorno;
    char nome[100];
}Acoes;

int main() {
    setlocale(LC_ALL, "");
    setlocale(LC_NUMERIC, "C");

    Acoes *acoes;
    //n para contar quantas linhas existem no arquivo.
    int nAcoes = 0;

    //abrindo arquivo para leitura
    FILE *arquivo = fopen("dados_acoes.txt", "r");
        //se arquivo existir, executa as linhas
        if (arquivo == NULL){
            printf("erro ao abrir arquivo\n");
            return 1;
        }

        //vai contar a quantidade de linhas do arquivo para alocar dinamicamente o Arr[Ações] com o tamanho de linhas do arquivo dados_acoes
        char linhas[256];
        while (fgets(linhas, sizeof(linhas), arquivo) != NULL){
            nAcoes++;
        }
        // tá verificando se o arquivo tem dados.

        if (nAcoes==0){
            printf("sem dados.\n");
            fclose(arquivo);
            return 0;
        }

        //aloca dinamicamente o array[acoes]
         acoes = malloc(nAcoes *sizeof(Acoes));

        //verifica erro de alocação
        if (acoes == NULL){
            printf("erro ao alocar memoria.\n");
            fclose(arquivo);
            return 1;
        }
        rewind(arquivo); //retorna o ponteiro *arquivo pro começo do arquivo

        //le o arquivo e registra os dados no array[acoes], na ordem: Sigla | Valor | Retorno | Nome_Acao.
        for (int i = 0; i < nAcoes; i++) {
        // verifica se a quantidade de dados lidos é igual a quantidade necessaria para cada acao, caso nao seja, retorna erro.
        //caso seja lido apenas tres dados ou menos (como Sigla Valor Retorno ) retorna eeeo, é necessario que todas as ações tenham os 4 dados necessarios.
        int verificar = fscanf(arquivo, "%s %lf %lf %s", acoes[i].sigla, &acoes[i].valor, &acoes[i].retorno, acoes[i].nome);
        if (verificar != 4) {
            printf("Erro ao ler os dados, necessario todos os dados para todas as ações.\n");
            fclose(arquivo);
            return 1;
            }
        }
        fclose(arquivo);

    // CASO DÊ CERTO, COMEÇA O PROCESSO DO ALGORITMO.
    //______________________________________________________________

    float capitalInicial;
    printf("Digite a quantidade de capital para investir: ");
    scanf("%f",&capitalInicial);

    printf("----------------------------------------\n");
    printf("Carteira de Investimentos Otimizada\n");
    printf("----------------------------------------\n");
    printf("Capital Disponivel: R$ %.2f\n", capitalInicial);

        // multiplica pra ser inteiro, trabalhar com inteiros é mais simples
        int capitalInt = (capitalInicial * 100);

        // criando tabela da mochila (matriz dinamica)
        // usa nAcoes por que a matriz para o funcionamento da mochila deve ser n acoes +1 x n acoes +1
        float **mochila = malloc((nAcoes + 1) * sizeof(float *));
        for (int i = 0; i < nAcoes+1; i++){
            mochila[i] = malloc((capitalInt + 1) * sizeof(float));
        }

        // alocar 0 para todas as posições.
        for (int i = 0; i<nAcoes+1; i++){
            for (int j = 0 ; j<nAcoes+1; j++){
                mochila[i][j]=0;
            }
        }

        // enche a mochila com as opções
        for (int i = 1; i <= nAcoes; i++) {
            int custoi = (acoes[i - 1].valor * 100+0.5);
            for (int j = 0; j <= capitalInt; j++) {
                if (custoi <= j) {
                    float incluir = acoes[i - 1].retorno + mochila[i - 1][j - custoi];
                    float excluir = mochila[i - 1][j];
                    mochila[i][j] = (incluir > excluir) ? incluir : excluir;
                } else {
                    mochila[i][j] = mochila[i - 1][j];
                }
            }
        }

        // volta na mochila, sempre procurando as acoes escolhidas que foram selecionadas
        printf("\nAções a comprar:\n");
        float custoTotal = 0.0;
        int j = capitalInt;
        for (int i = nAcoes; i >= 1; i--) {
            if (mochila[i][j] != mochila[i - 1][j]) {
                printf("- %s (%s) (Custo: R$ %.2f, Retorno: %.2f%%)\n",
                       acoes[i - 1].sigla, acoes[i - 1].nome, acoes[i - 1].valor, acoes[i - 1].retorno);
                custoTotal += acoes[i - 1].valor;
                j -= (acoes[i - 1].valor * 100);
            }
        }

        printf("\nResumo da Cateira:");
        printf("\n- Custo Total: R$ %.2f\n", custoTotal);
        float retornoMax = mochila[nAcoes][capitalInt]; // valor maximo possível
        printf("- Retorno Máximo Esperado: %.2f%%\n", retornoMax);
        printf("----------------------------------------\n");

        // limpa a memoria
        for (int i = 0; i <= nAcoes; i++){
        free(mochila[i]);}
        free(mochila);
        free(acoes);

    return 0;
}