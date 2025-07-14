#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define FATOR_ESCALA 100 //convertendo valor monetário em inteiro
typedef struct {
    char nome[20];
    double custo;
    double retorno;
} Acao;

int main() {
    double capital_de_investimento;
    int num_acoes_de_entrada;

    printf("----------------------------------------\n");
    printf("  Otimizacao de Carteira de Investimentos\n");
    printf("----------------------------------------\n\n");

    printf("Digite o capital de investimento: ");
    scanf("%lf", &capital_de_investimento);
    
    printf("Digite o numero de ações de entrada: ");
    scanf("%d", &num_acoes_de_entrada);

    Acao *acoes_disponiveis = (Acao *)malloc(num_acoes_de_entrada * sizeof(Acao));
    if (acoes_disponiveis == NULL) {
        printf("Erro de alocacao de memoria para as ações.\n");
        return 1;
    }

    return 0;
}
