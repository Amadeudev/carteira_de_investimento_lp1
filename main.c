#include <stdio.h>
#include <stdlib.h>
#include <limits.h> // Para INT_MAX

// Define o tamanho máximo da matriz
#define MAX_SIZE 10

// Função para imprimir a matriz (para depuração)
void printMatrix(int matrix[MAX_SIZE][MAX_SIZE], int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            printf("%4d ", matrix[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

// Implementa o algoritmo Húngaro
int hungarianAlgorithm(int costMatrix[MAX_SIZE][MAX_SIZE], int n) {
    int matrix[MAX_SIZE][MAX_SIZE];
    // Copia a matriz de custo original para não modificá-la diretamente
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            matrix[i][j] = costMatrix[i][j];
        }
    }

    printf("Matriz Original:\n");
    printMatrix(matrix, n);

    // PASSO 1: Redução por Linha
    // Para cada linha, subtrai o menor elemento de todos os elementos da linha
    for (int i = 0; i < n; i++) {
        int minVal = INT_MAX;
        for (int j = 0; j < n; j++) {
            if (matrix[i][j] < minVal) {
                minVal = matrix[i][j];
            }
        }
        for (int j = 0; j < n; j++) {
            matrix[i][j] -= minVal;
        }
    }
    printf("Matriz após Redução por Linha:\n");
    printMatrix(matrix, n);

    // PASSO 2: Redução por Coluna
    // Para cada coluna, subtrai o menor elemento de todos os elementos da coluna
    for (int j = 0; j < n; j++) {
        int minVal = INT_MAX;
        for (int i = 0; i < n; i++) {
            if (matrix[i][j] < minVal) {
                minVal = matrix[i][j];
            }
        }
        for (int i = 0; i < n; i++) {
            matrix[i][j] -= minVal;
        }
    }
    printf("Matriz após Redução por Coluna:\n");
    printMatrix(matrix, n);

    // PASSO 3: Cobrir os Zeros com o mínimo de linhas
    // Este é o passo mais complexo. Envolve encontrar um conjunto de linhas
    // e colunas que cobrem todos os zeros e, se o número de linhas for menor que N,
    // ajustar a matriz. Esta implementação simplificada omite a lógica completa
    // de cobertura e ajuste, focando na lógica principal de redução.
    // Uma implementação completa envolveria:
    // 1. Marcar zeros independentes.
    // 2. Cobrir todos os zeros com o mínimo de linhas.
    // 3. Se o número de linhas for menor que N, encontrar o menor elemento
    //    não coberto, subtraí-lo de todos os elementos não cobertos e adicioná-lo
    //    aos elementos cobertos duas vezes. Repetir passos 1-3.

    // Para este exemplo, vamos simplificar e assumir que a matriz já está
    // "quase" pronta para a atribuição após as reduções, e então
    // faremos uma atribuição simples baseada nos zeros existentes.
    // Em uma implementação completa e robusta, você precisaria de um algoritmo
    // para encontrar a cobertura mínima de zeros e iterar até ter N linhas.
    // Bibliotecas de otimização geralmente possuem implementações mais completas.

    // PASSO SIMPLIFICADO PARA ATRIBUIÇÃO (para ilustrar a ideia final)
    // Encontrar uma atribuição de zeros
    int assignment[MAX_SIZE]; // assignment[i] = j significa linha i atribuída à coluna j
    int assignedCol[MAX_SIZE]