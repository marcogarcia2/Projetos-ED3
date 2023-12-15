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
void insereGrafo(Registro *r, Grafo *grafo);



#endif
