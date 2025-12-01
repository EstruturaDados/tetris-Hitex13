#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_FILA 5
#define MAX_PILHA 3
#define MAX_HIST 20   // histórico para desfazer

typedef struct {
    char nome;
    int id;
} Peca;

typedef struct {
    Peca fila[MAX_FILA];
    int ini, fim;
    int qtd;
} Fila;

typedef struct {
    Peca pilha[MAX_PILHA];
    int topo;
} Pilha;

// ---------- HISTÓRICO (para desfazer) ----------
typedef struct {
    Peca fila[MAX_FILA];
    int ini, fim, qtd;

    Peca pilha[MAX_PILHA];
    int topo;
} Estado;

Estado historico[MAX_HIST];
int hist_topo = -1;

// ---------- GERAR PEÇA ----------
Peca gerarPeca() {
    Peca p;
    char tipos[] = {'I','O','T','L','J','S','Z'};
    p.nome = tipos[rand() % 7];
    p.id = rand() % 1000;
    return p;
}

// ---------- FUNÇÕES DE FILA ----------
void inicializarFila(Fila *f) {
    f->ini = 0;
    f->fim = 0;
    f->qtd = 0;

    for (int i = 0; i < MAX_FILA; i++) {
        Peca p = gerarPeca();
        f->fila[f->fim] = p;
        f->fim = (f->fim + 1) % MAX_FILA;
        f->qtd++;
    }
}

void enfileirar(Fila *f, Peca p) {
    if (f->qtd == MAX_FILA) return;
    f->fila[f->fim] = p;
    f->fim = (f->fim + 1) % MAX_FILA;
    f->qtd++;
}

Peca desenfileirar(Fila *f) {
    Peca vazia = {'-', -1};
    if (f->qtd == 0) return vazia;

    Peca p = f->fila[f->ini];
    f->ini = (f->ini + 1) % MAX_FILA;
    f->qtd--;

    return p;
}

// ---------- FUNÇÕES DE PILHA ----------
void inicializarPilha(Pilha *p) {
    p->topo = -1;
}

void push(Pilha *p, Peca x) {
    if (p->topo < MAX_PILHA - 1) {
        p->pilha[++p->topo] = x;
    }
}

Peca pop(Pilha *p) {
    Peca vazia = {'-', -1};
    if (p->topo == -1) return vazia;
    return p->pilha[p->topo--];
}

// ---------- HISTÓRICO (para desfazer) ----------
void salvarEstado(Fila f, Pilha p) {
    if (hist_topo < MAX_HIST - 1) {
        hist_topo++;
        for (int i = 0; i < MAX_FILA; i++)
            historico[hist_topo].fila[i] = f.fila[i];

        historico[hist_topo].ini = f.ini;
        historico[hist_topo].fim = f.fim;
        historico[hist_topo].qtd = f.qtd;

        for (int i = 0; i < MAX_PILHA; i++)
            historico[hist_topo].pilha[i] = p.pilha[i];

        historico[hist_topo].topo = p.topo;
    }
}

void desfazer(Fila *f, Pilha *p) {
    if (hist_topo < 0) {
        printf("Nada para desfazer!\n");
        return;
    }
    Estado e = historico[hist_topo--];

    for (int i = 0; i < MAX_FILA; i++)
        f->fila[i] = e.fila[i];

    f->ini = e.ini;
    f->fim = e.fim;
    f->qtd = e.qtd;

    for (int i = 0; i < MAX_PILHA; i++)
        p->pilha[i] = e.pilha[i];

    p->topo = e.topo;

    printf("Última ação desfeita!\n");
}

// ---------- UTILITÁRIOS ----------
void mostrarFila(Fila f) {
    printf("\nFILA: ");
    int i = f.ini;
    for (int c = 0; c < f.qtd; c++) {
        Peca p = f.fila[i];
        printf("[%c-%d] ", p.nome, p.id);
        i = (i + 1) % MAX_FILA;
    }
    printf("\n");
}

void mostrarPilha(Pilha p) {
    printf("PILHA: ");
    for (int i = 0; i <= p.topo; i++) {
        printf("[%c-%d] ", p.pilha[i].nome, p.pilha[i].id);
    }
    printf("\n\n");
}

// ---------- OPERAÇÕES ESPECIAIS ----------
void trocar(Fila *f, Pilha *p) {
    if (p->topo == -1) {
        printf("Pilha vazia!\n");
        return;
    }
    salvarEstado(*f, *p);

    Peca topo = p->pilha[p->topo];
    Peca frente = f->fila[f->ini];

    p->pilha[p->topo] = frente;
    f->fila[f->ini] = topo;

    printf("Trocado!\n");
}

void inverter(Fila *f, Pilha *p) {
    salvarEstado(*f, *p);

    // inverter fila
    Peca tempFila[MAX_FILA];
    int i = f->ini;
    for (int c = 0; c < f->qtd; c++) {
        tempFila[c] = f->fila[i];
        i = (i + 1) % MAX_FILA;
    }
    for (int c = 0; c < f->qtd; c++)
        f->fila[c] = tempFila[f->qtd - 1 - c];
    f->ini = 0;
    f->fim = f->qtd;

    // inverter pilha
    for (int c = 0; c < p->topo / 2; c++) {
        Peca tmp = p->pilha[c];
        p->pilha[c] = p->pilha[p->topo - c];
        p->pilha[p->topo - c] = tmp;
    }

    printf("Fila e pilha invertidas!\n");
}

// ---------- MAIN ----------
int main() {
    Fila f;
    Pilha p;

    inicializarFila(&f);
    inicializarPilha(&p);

    int op;
    srand(time(NULL));

    do {
        mostrarFila(f);
        mostrarPilha(p);

        printf("1 - Jogar peça\n");
        printf("2 - Reservar peça\n");
        printf("3 - Usar peça reservada\n");
        printf("4 - Trocar topo da pilha com frente da fila\n");
        printf("5 - Desfazer última ação\n");
        printf("6 - Inverter fila e pilha\n");
        printf("0 - Sair\n> ");
        scanf("%d", &op);

        switch (op) {
            case 1: {
                salvarEstado(f, p);
                Peca jogada = desenfileirar(&f);
                printf("Jogou: [%c-%d]\n", jogada.nome, jogada.id);
                enfileirar(&f, gerarPeca());
                break;
            }
            case 2: {
                salvarEstado(f, p);
                Peca r = desenfileirar(&f);
                printf("Reservou: [%c-%d]\n", r.nome, r.id);
                push(&p, r);
                enfileirar(&f, gerarPeca());
                break;
            }
            case 3: {
                salvarEstado(f, p);
                Peca usado = pop(&p);
                if (usado.id == -1) {
                    printf("Pilha vazia!\n");
                    break;
                }
                printf("Usou reservada: [%c-%d]\n", usado.nome, usado.id);
                break;
            }
            case 4:
                trocar(&f, &p);
                break;

            case 5:
                desfazer(&f, &p);
                break;

            case 6:
                inverter(&f, &p);
                break;

            case 0:
                printf("Saindo...\n");
                break;

            default:
                printf("Opção inválida!\n");
        }

    } while (op != 0);

    return 0;
    //Estou Cansado jefrey estou cansado
}
