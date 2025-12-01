#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define TAM 5   // tamanho fixo da fila

// STRUCT DA PEÇA
typedef struct {
    char nome[5];   // I, O, T, L ...
    int id;         // identificador 
} Peca;

// GERA PEÇA AUTOMÁTICA
Peca gerarPeca() {
    Peca nova;
    char tipos[4][5] = {"I", "O", "T", "L"};

    int r = rand() % 4;
    strcpy(nova.nome, tipos[r]);

    nova.id = rand() % 1000; // id aleatório

    return nova;
}

// MOSTRAR A FILA
void mostrarFila(Peca fila[], int inicio, int tamanho) {
    printf("\n--- FILA DE PECAS ---\n");
    if (tamanho == 0) {
        printf("Fila vazia!\n");
        return;
    }

    for (int i = 0; i < tamanho; i++) {
        int idx = (inicio + i) % TAM;
        printf("[%d] Nome: %s | ID: %d\n", i+1, fila[idx].nome, fila[idx].id);
    }
}

// ENQUEUE - INSERIR PEÇA
void enqueue(Peca fila[], int *fim, int *tamanho) {
    if (*tamanho == TAM) {
        printf("\nFila cheia! Não é possível inserir.\n");
        return;
    }

    Peca nova = gerarPeca();
    fila[*fim] = nova;

    *fim = (*fim + 1) % TAM;
    (*tamanho)++;

    printf("\nPeça inserida: %s (ID %d)\n", nova.nome, nova.id);
}

// DEQUEUE - REMOVER PEÇA
void dequeue(Peca fila[], int *inicio, int *tamanho) {
    if (*tamanho == 0) {
        printf("\nFila vazia! Não há o que jogar.\n");
        return;
    }

    Peca removida = fila[*inicio];
    printf("\nPeça jogada/removida: %s (ID %d)\n", removida.nome, removida.id);

    *inicio = (*inicio + 1) % TAM;
    (*tamanho)--;
}

// PROGRAMA PRINCIPAL
int main() {
    srand(time(NULL));

    Peca fila[TAM];
    int inicio = 0;  // aponta para o começo da fila
    int fim = 0;     // aponta para a posição de inserção
    int tamanho = 0;

    // Inicializar com 5 peças
    for (int i = 0; i < TAM; i++) {
        enqueue(fila, &fim, &tamanho);
    }

    int opcao;

    do {
        printf("\n=======================");
        printf("\n  TETRIS STACK - FILA  ");
        printf("\n=======================\n");
        printf("1 - Jogar peça (dequeue)\n");
        printf("2 - Inserir nova peça (enqueue)\n");
        printf("3 - Mostrar fila atual\n");
        printf("0 - Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                dequeue(fila, &inicio, &tamanho);
                mostrarFila(fila, inicio, tamanho);
                break;

            case 2:
                enqueue(fila, &fim, &tamanho);
                mostrarFila(fila, inicio, tamanho);
                break;

            case 3:
                mostrarFila(fila, inicio, tamanho);
                break;

            case 0:
                printf("\nSaindo...\n");
                break;

            default:
                printf("\nOpcao invalida!\n");
        }

    } while (opcao != 0);

    return 0;
}
