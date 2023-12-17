/*  
*   Lucas Lima Romero - 13676325
*   Marco Antonio Gaspar Garcia - 11833581
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>

#include "funcionalidades.h"
#include "registros.h"
#include "funcoesCriadas.h"
#include "funcoesFornecidas.h"
#include "grafo.h"

// Funcionalidade 12: encontra o caminho mais curto entre a Origem e o Destino

void dijkstra(char *nomeArquivoBIN, int n){ 

    // Lendo o arquivo binário e gerando o grafo
    Grafo *grafo = gerarGrafo(nomeArquivoBIN);

    // Se o grafo for nulo, encerramos a função
    if (grafo == NULL) return;

    
    // Variáveis auxiliares
    char nomeTecnologiaOrigem[30], nomeTecnologiaDestino[30];
    int *distancia = (int*) malloc(grafo->numVertices * sizeof(int));
    bool *visitado = (bool*) malloc(grafo->numVertices * sizeof(bool));

    // Executando a funcionalidade n vezes
    for(int iteracao = 0; iteracao < n; iteracao++){

        scan_quote_string(nomeTecnologiaOrigem);
        scan_quote_string(nomeTecnologiaDestino);

        // Encontrando a posição de origem e destino no vetor de vértices
        int posOrigem = buscaBinariaGrafo(grafo, nomeTecnologiaOrigem);
        int posDestino = buscaBinariaGrafo(grafo, nomeTecnologiaDestino);

        for(int i = 0; i < grafo->numVertices; i++){
            distancia[i] = INT_MAX;
            visitado[i] = false;
        }
        distancia[posOrigem] = 0;

        // Encontrar o caminho mais curto
        for (int count = 0; count < grafo->numVertices - 1; count++) {
            int u = -1;
            for (int v = 0; v < grafo->numVertices; v++) {
                if (!visitado[v] && (u == -1 || distancia[v] < distancia[u])) {
                    u = v;
                }
            }

            visitado[u] = true;

            for (Aresta *aresta = grafo->vertices[u].arestaInicial; aresta != NULL; aresta = aresta->prox) {
                int v = buscaBinariaGrafo(grafo, aresta->tecDestino);
                int peso = aresta->peso;

                if (!visitado[v] && distancia[u] != INT_MAX && distancia[u] + peso < distancia[v]) {
                    distancia[v] = distancia[u] + peso;
                }
            }
        }

        if(distancia[posDestino] == INT_MAX){
            printf("%s %s: CAMINHO INEXISTENTE.\n", nomeTecnologiaOrigem, nomeTecnologiaDestino);
        } else {
            printf("%s %s: %d\n", nomeTecnologiaOrigem, nomeTecnologiaDestino, distancia[posDestino]);
        }
    }

    // Liberando a memória
    destroiGrafo(grafo);
    
}
