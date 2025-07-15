#include <stdio.h>
#include <stdlib.h>
//
// ESSE CODIGO É INTEIRO GPT, MAS FUNCIONAL, O INTUITO DELE É SERVIR PRA NOSSA BASE PRO CODIGO DE VERDADE, MAIS LIMPO E COMENTADO.
//
typedef struct {
    char sigla[10];
    double valor;
    double retorno;
    char nome[100];
} Acoes;

int main() {
    int n = 0;
    FILE *arquivo = fopen("dados_acoes.txt", "r");

    if (!arquivo) {
        printf("erro ao abrir o arquivo.\n");
        return 1;
    }

    char linhas[256];
    while (fgets(linhas, sizeof(linhas), arquivo) != NULL) n++;

    if (n == 0) {
        printf("arquivo vazio.\n");
        fclose(arquivo);
        return 0;
    }

    Acoes *acoes = malloc(n * sizeof(Acoes));
    if (!acoes) {
        printf("erro ao alocar memória.\n");
        fclose(arquivo);
        return 1;
    }

    rewind(arquivo);
    for (int i = 0; i < n; i++) {
        if (fscanf(arquivo, "%s %lf %lf %s", acoes[i].sigla, &acoes[i].valor, &acoes[i].retorno, acoes[i].nome) != 4) {
            printf("Erro ao ler linha %d.\n", i+1);
            free(acoes);
            fclose(arquivo);
            return 1;
        }
    }
    fclose(arquivo);

    double capitalInicial;
    printf("Digite a quantidade de capital para investir: ");
    scanf("%lf", &capitalInicial);

    int capital_discreto = (int)(capitalInicial / 10);

    // Alocar DP 2D
    double **dp = malloc((n+1) * sizeof(double*));
    for (int i = 0; i <= n; i++) dp[i] = calloc(capital_discreto+1, sizeof(double));

    if (!dp) {
        printf("erro ao alocar DP.\n");
        free(acoes);
        return 1;
    }

    // Preencher DP
    for (int i = 1; i <= n; i++) {
        int custo_i = (int)(acoes[i-1].valor / 10);
        for (int w = 0; w <= capital_discreto; w++) {
            if (custo_i <= w) {
                double incluir = acoes[i-1].retorno + dp[i-1][w - custo_i];
                double excluir = dp[i-1][w];
                dp[i][w] = (incluir > excluir) ? incluir : excluir;
            } else {
                dp[i][w] = dp[i-1][w];
            }
        }
    }

    printf("\nRetorno máximo possível: %.2lf%%\n", dp[n][capital_discreto]);

    // Traceback
    printf("\nCarteira ótima:\n");
    double custo_total = 0.0;
    int w = capital_discreto;
    for (int i = n; i >= 1; i--) {
        if (dp[i][w] != dp[i-1][w]) {
            printf("- %s (%s) | Custo: R$%.2lf | Retorno: %.2lf%%\n",
                   acoes[i-1].sigla, acoes[i-1].nome, acoes[i-1].valor, acoes[i-1].retorno);
            custo_total += acoes[i-1].valor;
            w -= (int)(acoes[i-1].valor / 10);
        }
    }

    printf("\nCusto total da carteira: R$%.2lf\n", custo_total);

    // Liberação de memória
    for (int i = 0; i <= n; i++) free(dp[i]);
    free(dp);
    free(acoes);

    return 0;
}