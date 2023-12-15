/*  
*   Lucas Lima Romero - 13676325
*   Marco Antonio Gaspar Garcia - 11833581
*/

#include "grafo.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct _aresta{
    char tecDestino[30];
    int peso;
    struct _aresta *prox;
} Aresta;

// AZURE .NET
typedef struct _vertice{
    char tecOrigem[30];
    int grupo;
    int grau, grauEntrada, grauSaida;
    Aresta *arestas;
    //unsigned int numArestas;
} Vertice;

// Um grafo é composto de vértices
typedef struct _grafo{
    Vertice *vertices;
    unsigned int numVertices;
} Grafo;

// Função que cria uma aresta
Aresta *criaAresta(void){
    
    Aresta *a = (Aresta*) malloc(sizeof(Aresta));
    a->tecDestino[0] = '\0';
    a->peso = -1;
    a->prox = NULL;

    return a;
}

// Função que cria um vértice
Vertice *criaVertice(void){

    Vertice *v = (Vertice*) malloc(sizeof(Vertice));
    v->tecOrigem[0] = '\0';
    v->grupo = -1;
    v->grau = 0;
    v->grauEntrada = 0;
    v->grauSaida = 0;
    v->arestas = NULL;

    return v;
}

// Função que cria um grafo
Grafo *criaGrafo(void){

    Grafo *G = (Grafo*) malloc(sizeof(Grafo));

    G->vertices = NULL;
    G->numVertices = 0;
    
    return G;
}

void bubbleSort(Grafo *grafo){

    // Variável auxiliar
    Vertice aux;
    bool flag = false;

    // Percorre o vetor de vértices
    for (int i = 0; i < grafo->numVertices; i++){

        // Percorre o vetor de vértices
        for (int j = 0; j < grafo->numVertices - 1 - i; j++){

            // Se o vértice j vier depois do vértice j+1 na ordem alfabética, trocamos
            if (strcmp(grafo->vertices[j].tecOrigem, grafo->vertices[j+1].tecOrigem) > 0){
                aux = grafo->vertices[j];
                grafo->vertices[j] = grafo->vertices[j+1];
                grafo->vertices[j+1] = aux;
                flag = true;
            }
        }

        if (!flag) break;
    }
}

// Função que insere um registro em um grafo vazio
insereGrafoVazio(Registro *r, Grafo *grafo){

    // Quando o grafo está vazio, iremos inserir dois novos vértices
    grafo->vertices = (Vertice*) calloc(2, sizeof(Vertice));

    // Criando um novo vértice
    Vertice *v1 = criaVertice();

    // Copiando os valores necessários do registro para o vértice
    strcpy(v1->tecOrigem, r->tecnologiaOrigem.string);
    v1->grupo = r->grupo;

    // Atribuindo o vértice ao grafo
    grafo->vertices[0] = *v1;
    grafo->numVertices++;

    // Antes de adicionar a aresta, precisamos criar um novo vértice, da tecDestino
    Vertice *v2 = criaVertice();
    strcpy(v2->tecOrigem, r->tecnologiaDestino.string);
    v2->grupo = r->grupo; // ???????????????????????????????/
    
    grafo->vertices[1] = *v2;
    grafo->numVertices++;

    // Entretanto, v2 pode vir antes de v1 na ordem alfabética, ordenemos (existe essa palavra?)
    bubbleSort(grafo);


    // Criando a aresta

}

// Função que adiciona um registro todo a um grafo
void insereGrafo(Registro *r, Grafo *grafo){

    // Se tecnologiaOrigem for nula, não inserimos nada
    if (!strcmp(r->tecnologiaOrigem.string, "")) return;

    // Se não for, vamos inserir no vértice correspondente.

    // Se o grafo estiver vazio, precisamos criar um vértice novo
    if (grafo->numVertices == 0) insereGrafoVazio(r, grafo);

}


