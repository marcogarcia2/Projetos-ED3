/*  
*   Lucas Lima Romero - 13676325
*   Marco Antonio Gaspar Garcia - 11833581
*/

#ifndef GRAFO_H
#define GRAFO_H

#include "registros.h"

//
// Estrutura que define uma aresta de um grafo
typedef struct _aresta{
    char tecDestino[30];
    int peso;
    struct _aresta *prox;
} Aresta;

// Estrutura que define um vértice de um grafo
typedef struct _vertice{
    char tecnologia[30];
    int grupo;
    int grau, grauEntrada, grauSaida;
    Aresta *arestaInicial;
    unsigned int numArestas;
} Vertice;

// Estrutura que define um grafo
typedef struct _grafo{
    Vertice *vertices;
    unsigned int numVertices;
} Grafo;

// Função que cria um grafo
Grafo *criaGrafo(void);

// Função que adiciona um registro todo a um grafo
void insereGrafo(Grafo *grafo, Registro *r);

// Função que transpõe um grafo
Grafo *transpor(Grafo *grafo);

// Função que imprime um grafo
void imprimeGrafo(Grafo *grafo);

// Função que faz uma busca binária nos vértices de um grafo direcionado
int buscaBinariaGrafo(Grafo *grafo, char *nomeTecnologia);

// Função que destrói um grafo
void destroiGrafo(Grafo *grafo);


#endif
