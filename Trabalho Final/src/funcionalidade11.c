/*  
*   Lucas Lima Romero - 13676325
*   Marco Antonio Gaspar Garcia - 11833581
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "funcionalidades.h"
#include "registros.h"
#include "funcoesCriadas.h"
#include "grafo.h"

// Funcionalidade 11: verifica a conectividade forte de um grafo (Algoritmo de Kosaraju)

// Busca em profundidade num grafo
void DFS(Grafo *grafo, int posOrigem, bool *visitado, int *pilha, int *topo){

    // O vértice atual foi visitado
    visitado[posOrigem] = true;

    // Percorrendo as arestas do vértice atual
    Aresta *arestaAtual = grafo->vertices[posOrigem].arestaInicial;
    while(arestaAtual != NULL){
        
        // Chamando recursivamente a DFS para o vértice de destino da aresta atual, se ele não foi visitado
        int posDestino = buscaBinariaGrafo(grafo, arestaAtual->tecDestino);
        if(visitado[posDestino] == false){
            DFS(grafo, posDestino, visitado, pilha, topo);
        }
        
        arestaAtual = arestaAtual->prox;
    }

    // Atualizando a pilha
    (*topo)++;
    pilha[(*topo)] = posOrigem;
}

// Função DFS para a segunda passagem no grafo transposto (usando índice)
void DFSTransposto(Grafo *grafoTransposto, bool *visitado, int posOrigem) {
    
    // O vértice atual foi visitado
    visitado[posOrigem] = true;

    // Percorrendo as arestas do vértice atual no grafo transposto
    Aresta *arestaAtual = grafoTransposto->vertices[posOrigem].arestaInicial;
    while (arestaAtual != NULL) {

        // Chamando recursivamente a DFS para o vértice de destino da aresta atual
        int posDestino = buscaBinariaGrafo(grafoTransposto, arestaAtual->tecDestino);
        if (visitado[posDestino] == false) {
            DFSTransposto(grafoTransposto, visitado, posDestino);
        }

        arestaAtual = arestaAtual->prox;
    }
}

// Algoritmo de Kosaraju: encontrar quantos componentes tem o grafo
void kosaraju(char *nomeArquivoBIN){

    // Obtendo o grafo referente ao arquivo binário e seu transposto
    Grafo *grafo = gerarGrafo(nomeArquivoBIN);
    if (grafo == NULL) {
        printf("Falha na execução da funcionalidade.");
        return;
    }
    Grafo *grafoTransposto = transpor(grafo);

    // Inicializando o vetor de vértices visitados -> todos iniciam falsos
    bool *visitados = (bool*) calloc(grafo->numVertices, sizeof(bool));

    // Inicializando a pilha que irá guardar a ordem de visita dos vértices
    int *pilha = (int*) calloc(grafo->numVertices, sizeof(int));
    int topo = -1; 

    // Realizando a DFS (busca em profundidade) no grafo original
    for (int i = 0; i < grafo->numVertices; i++) {

        // Se o vértice não foi visitado, chamamos a DFS para ele
        if (!visitados[i]) {
            DFS(grafo, i, visitados, pilha, &topo);
        }
    }

    // Reinicializando o vetor de vértices visitados para a segunda passagem
    memset(visitados, false, grafo->numVertices * sizeof(bool));

    // Chamemos a DFS para o grafo transposto para cada vértice na ordem da pilha (desempilhando)
    int numComponentes = 0;
    for (int i = grafo->numVertices - 1; i >= 0; i--) {
        int posOrigem = pilha[i];
        if (!visitados[posOrigem]) {
            DFSTransposto(grafoTransposto, visitados, posOrigem);
            numComponentes++;
        }
    }

    /*
        O grafo será fortemente conexo se e somente se o número de componentes for 1.
        Isto significa que DFSTransposto() foi chamada uma única vez, pois visitou 
        todos os vértices na primeira passada.
    */

    // Imprimir se o grafo é fortemente conexo e o número de componentes
    if (numComponentes == 1)
        printf("Sim, o grafo é fortemente conexo e possui 1 componente.");
    
    else 
        printf("Não, o grafo não é fortemente conexo e possui %d componentes.", numComponentes);
    
    
    // Liberando a memória
    destroiGrafo(grafo);
    destroiGrafo(grafoTransposto);
    free(visitados);
    free(pilha);
}
