#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define TAM_FILA 5
#define TAM_PILHA 3

// STRUCT DA PEÇA
typedef struct {
    char nome[5];
    int id;
} Peca;

// GERA PEÇA AUTOMÁTICA
Peca gerarPeca() {
    Peca p;
    char tipos[4][5] = {"I", "O", "T", "L"};
    int r = rand() % 4;

    strcpy(p.nome, tipos[r]);
    p.id = rand() % 1000;

    return p;
}

// MOSTRAR A FILA
void mostrarFila(Peca fila[], int inicio) {
    printf("\n--- FILA DE PECAS (5 fixo) ---\n");
    for (int i = 0; i < TAM_FILA; i++) {
        int idx = (inicio + i) % TAM_FILA;
        printf("[%d] Nome: %s | ID: %d\n", i + 1, fila[idx].nome, fila[idx].id);
    }
}

// MOSTRAR A PILHA
void mostrarPilha(Peca pilha[], int topo) {
    printf("\n--- PILHA DE RESERVA (max 3) ---\n");

    if (topo == -1) {
        printf("Pilha vazia!\n");
        return;
    }

    for (int i = topo; i >= 0; i--) {
        printf("[%d] Nome: %s | ID: %d\n", i, pilha[i].nome, pilha[i].id);
    }
}

// ENQUEUE (INSERE NO FIM DA FILA)
void enqueue(Peca fila[], int *fim) {
    fila[*fim] = gerarPeca();
    *fim = (*fim + 1) % TAM_FILA;
}

// DEQUEUE (REMOVE DA FRENTE)
Peca dequeue(Peca fila[], int *inicio) {
    Peca removida = fila[*inicio];
    *inicio = (*inicio + 1) % TAM_FILA;
    return removida;
}

// PUSH NA PILHA (RESERVAR)
void push(Peca pilha[], int *topo, Peca elemento) {
    if (*topo == TAM_PILHA - 1) {
        printf("\nERRO: Pilha cheia! Não é possível reservar mais.\n");
        return;
    }
    pilha[++(*topo)] = elemento;
    printf("\nPeça %s (ID %d) reservada com sucesso!\n", elemento.nome, elemento.id);
}

// POP DA PILHA (USAR RESERVADA)
Peca pop(Peca pilha[], int *topo, int *temElemento) {
    if (*topo == -1) {
        printf("\nERRO: Pilha vazia! Não há peça reservada.\n");
        *temElemento = 0;
        Peca vazia = {"__", -1};
        return vazia;
    }
    *temElemento = 1;
    return pilha[(*topo)--];
}

// PROGRAMA PRINCIPAL
int main() {
    srand(time(NULL));

    Peca fila[TAM_FILA];
    Peca pilha[TAM_PILHA];

    int inicio = 0;
    int fim = 0;
    int topo = -1;

    // Inicializar fila com 5 peças
    for (int i = 0; i < TAM_FILA; i++) {
        enqueue(fila, &fim);
    }

    int opcao;

    do {
        printf("\n========================");
        printf("\n  TETRIS STACK - NIVEL AVENTUREIRO");
        printf("\n========================\n");
        printf("1 - Jogar peça (dequeue)\n");
        printf("2 - Reservar peça da frente (push)\n");
        printf("3 - Usar peça reservada (pop)\n");
        printf("0 - Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcao);

        if (opcao == 1) {
            // Jogar peça (fila anda)
            Peca jogada = dequeue(fila, &inicio);
            printf("\nPeça jogada: %s (ID %d)\n", jogada.nome, jogada.id);

            // Repor para manter 5 peças
            enqueue(fila, &fim);

        } else if (opcao == 2) {
            // Reservar peça da frente
            Peca reservada = dequeue(fila, &inicio);
            push(pilha, &topo, reservada);

            // Repor na fila
            enqueue(fila, &fim);

        } else if (opcao == 3) {
            int ok;
            Peca usada = pop(pilha, &topo, &ok);

            if (ok) {
                printf("\nPeça usada: %s (ID %d)\n", usada.nome, usada.id);
            }

        } else if (opcao == 0) {
            printf("\nSaindo...\n");
        } else {
            printf("\nOpcão inválida!\n");
        }

        // Mostrar as duas estruturas sempre
        mostrarFila(fila, inicio);
        mostrarPilha(pilha, topo);

    } while (opcao != 0);

    return 0;
    //ACABA PELO AMOR DE DEUS ACABA
}
