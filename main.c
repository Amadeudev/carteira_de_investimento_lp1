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
}