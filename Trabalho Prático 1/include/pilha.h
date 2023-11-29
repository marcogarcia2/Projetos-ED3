#ifndef PILHA_H
#define PILHA_H

typedef struct Pilha Pilha;

Pilha* criaPilha(unsigned capacidade);
int estaCheia(Pilha* pilha);
int estaVazia(Pilha* pilha);
void empilha(Pilha* pilha, int item);
int desempilha(Pilha* pilha);
int topo(Pilha* pilha);
void destroiPilha(Pilha* pilha);

#endif // PILHA_H