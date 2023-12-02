/*  
*   Lucas Lima Romero - 13676325
*   Marco Antonio Gaspar Garcia - 11833581
*/

#include "lista.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// Estrutura que define o nó de uma lista
typedef struct _no {
    char *string;
    struct _no *prox;
} No;

// Estrutura que define a lista
typedef struct _lista {
    No *ini;
    No *fim;
    unsigned int tam;
} Lista;

// Funcao que cria um nó, dada uma string
No *criaNo(char *string, int tamString) {

    No *p = (No*) malloc(sizeof(No));
    p->string = (char*)malloc(tamString+1);

    strcpy(p->string, string);

    p->prox = NULL;

    return p;
}

// Função que cria uma lista de nós
Lista *criaLista(void) {

    Lista *L = (Lista*) malloc(sizeof(Lista));
    L->ini = NULL;
    L->fim = NULL;
    L->tam = 0;

    return L;
}

// Função que libera a memória alocada de um nó
void destroiNo(No *p){

    free(p->string);
    free(p);
}

// Função que libera toda a memória
void destroiLista(Lista **L_ref){

    Lista *L = *L_ref;

    No *p = L->ini;
    No *aux = NULL;

    while(p != NULL){
        aux = p; 
        p = p->prox;
        destroiNo(aux);
    }
    free(L);

    *L_ref = NULL;
}

// Função que detecta se um elemento ainda não existe na lista
bool escaneiaLista(const Lista *L, const No *p){

    No *aux = L->ini;
    while(aux != NULL){

        if (!strcmp(aux->string, p->string)) return false;
        aux = aux->prox;
    }

    return true;
}

// Função que retorna a quantidade de Nós de uma lista
int getTamanho(const Lista *L){
    return L->tam;
}

// Função que adiciona elementos ao fim de uma lista
void adicionaLista(Lista *L, char *string, int tamString) {

    if (!strcmp(string, "") || tamString <= 0) return;

    No *p = criaNo(string, tamString);

    if (getTamanho(L) == 0) {
        L->ini = p;
        L->fim = p; // Se esse for o início
        L->tam++;
    }
    
    // Só adicionaremos se o elemento ainda não existe na lista:
    else if (escaneiaLista(L, p)){
        L->fim->prox = p;
        L->fim = L->fim->prox; 
        L->tam++;
    }

    // Elemento já existe
    else destroiNo(p);
}

// Função que exibe os elementos da lista
void imprimeLista(const Lista *L){

    No *p = L->ini;

    printf("L -> ");

    while(p != NULL) {
        printf("%s -> ", p->string);
        p = p->prox;
    }

    printf("fim\n");
}
