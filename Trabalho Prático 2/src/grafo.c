/*  
*   Lucas Lima Romero - 13676325
*   Marco Antonio Gaspar Garcia - 11833581
*/

#include "grafo.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

//
typedef struct _aresta{
    char tecDestino[30];
    int peso;
    struct _aresta *prox;
} Aresta;

// AZURE .NET C# JAVA PYTHON PHP JAVASCRIPT RUBY GO C++ C SWIFT
// .NET   C#
// JAVA
typedef struct _vertice{
    char tecOrigem[30];
    int grupo;
    int grau, grauEntrada, grauSaida;
    Aresta *arestaInicial;
    unsigned int numArestas;
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

void insertionSort(Grafo *grafo) {

    // Variável auxiliar
    Vertice aux;
    int j;

    // Percorre o vetor de vértices
    for (int i = 1; i < grafo->numVertices; i++) {
        aux = grafo->vertices[i];
        j = i - 1;

        // Move os elementos do vetor[0..i-1], que são maiores que a chave, para uma posição à frente de sua posição atual
        while (j >= 0 && strcmp(grafo->vertices[j].tecOrigem, aux.tecOrigem) > 0) {
            grafo->vertices[j + 1] = grafo->vertices[j];
            j = j - 1;
        }
        grafo->vertices[j + 1] = aux;
    }
}

int escaneiaGrafo(Grafo *grafo, char *tec){

    int i;

    for (i = 0; i < grafo->numVertices; i++)
        if (!strcmp(grafo->vertices[i].tecOrigem, tec))
            return i;

    return -1;
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

    // Antes de adicionar a aresta, precisamos criar um novo vértice, o da tecDestino

    // Verificando se a string não é nula
    if (!strcmp(r->tecnologiaDestino.string, "")) return;

    Vertice *v2 = criaVertice();
    strcpy(v2->tecOrigem, r->tecnologiaDestino.string);
    v2->grupo = r->grupo; // ???????????????????????????????/
    
    grafo->vertices[1] = *v2;
    grafo->numVertices++;

    // Entretanto, v2 pode vir antes de v1 na ordem alfabética, ordenemos (existe essa palavra?)
    insertionSort(grafo);

    // Aresta
    Aresta *a = criaAresta();
    strcpy(a->tecDestino, r->tecnologiaDestino.string);
    a->peso = r->peso;

    // Atualizando na mão os atributos
    grafo->vertices[0].arestaInicial = a;
    grafo->vertices[0].numArestas++;


    grafo->vertices[0].grau++;
    grafo->vertices[0].grauSaida++;
    grafo->vertices[1].grau++;
    grafo->vertices[1].grauEntrada++;
}

// Função que adiciona um registro todo a um grafo
void insereGrafo(Registro *r, Grafo *grafo){

    // Se tecnologiaOrigem for nula, não inserimos nada
    if (!strcmp(r->tecnologiaOrigem.string, "")) return;

    // Se não for, vamos inserir no vértice correspondente.

    // Se o grafo estiver vazio, precisamos criar um vértice novo
    if (grafo->numVertices == 0) insereGrafoVazio(r, grafo);

    // Se o grafo não estiver vazio, temos mais alguns casos
    else{
        
        // Verificando se a tecnologiaOrigem já está no grafo
        int posOrigem = escaneiaGrafo(grafo, r->tecnologiaOrigem.string);
        int posDestino = escaneiaGrafo(grafo, r->tecnologiaDestino.string);
        
        // Caso 1: a tecnologiaOrigem já está no grafo
        if (posOrigem != -1){
            // Não é necessário criar um novo vértice para a origem
            // Talvez seja necessário criar um para a tecDestino


            // Caso 1.1: a tecnologiaDestino já está no grafo
            if (posDestino != -1){
                
                // Criamos direto a Aresta entre elas
                Aresta *a = criaAresta();
                strcpy(a->tecDestino, r->tecnologiaDestino.string);


            }


            // Caso 2: a tecnologiaOrigem não está no grafo
        } else{

            
        }







    }

}


