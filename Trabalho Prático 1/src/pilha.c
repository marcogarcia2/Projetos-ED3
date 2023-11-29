#include <stdio.h>
#include <stdlib.h>

typedef struct Pilha {
    int topo;
    unsigned capacidade;
    int* array;
} Pilha;

Pilha* criaPilha(unsigned capacidade) {
    Pilha* pilha = (Pilha*) malloc(sizeof(Pilha));
    pilha->capacidade = capacidade;
    pilha->topo = -1;
    pilha->array = (int*) malloc(pilha->capacidade * sizeof(int));
    return pilha;
}

int estaCheia(Pilha* pilha) {
    return pilha->topo == pilha->capacidade - 1;
}

int estaVazia(Pilha* pilha) {
    return pilha->topo == -1;
}

void empilha(Pilha* pilha, int item) {
    if (estaCheia(pilha)) {
        printf("Pilha cheia\n");
        return;
    }
    pilha->array[++pilha->topo] = item;
}

int desempilha(Pilha* pilha) {
    if (estaVazia(pilha)) {
        printf("Pilha vazia\n");
        return -2;
    }
    return pilha->array[pilha->topo--];
}

int topo(Pilha* pilha) {
    if (estaVazia(pilha)) {
        printf("Pilha vazia\n");
        return -2;
    }
    return pilha->array[pilha->topo];
}

void destroiPilha(Pilha* pilha) {
    free(pilha->array);
    free(pilha);
}