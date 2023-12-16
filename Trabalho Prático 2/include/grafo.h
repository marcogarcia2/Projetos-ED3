/*  
*   Lucas Lima Romero - 13676325
*   Marco Antonio Gaspar Garcia - 11833581
*/

#ifndef GRAFO_H
#define GRAFO_H

#include "registros.h"

//
typedef struct _grafo Grafo;
typedef struct _vertice Vertice;
typedef struct _aresta Aresta;

// Função que cria um grafo
Grafo *criaGrafo(void);

// Função que adiciona um registro todo a um grafo
void insereGrafo(Grafo *grafo, Registro *r);

// Função que transpõe um grafo
Grafo *transpor(Grafo *grafo);

// Função que imprime um grafo
void imprimeGrafo(Grafo *grafo);

// Função que destrói um grafo
void destroiGrafo(Grafo *grafo);


#endif
