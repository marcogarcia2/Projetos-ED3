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

// Funcionalidade 12: encontra o caminho mais curto entre a Origem e o Destino (algoritmo guloso)

void dijkstra(char *nomeArquivoBIN, int n){ 

    // Lendo o arquivo binário e gerando o grafo
    Grafo *grafo = gerarGrafo(nomeArquivoBIN);

    // Se o grafo for nulo, encerramos a função
    if (grafo == NULL) {
        printf("Falha na execução da funcionalidade.");
        return;
    }

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
        
        // Caso as tecnologias não existam no grafo
        if (strcmp(grafo->vertices[posOrigem].tecnologia, nomeTecnologiaOrigem) != 0 
        || strcmp(grafo->vertices[posDestino].tecnologia, nomeTecnologiaDestino) != 0) {
            printf("%s %s: CAMINHO INEXISTENTE.\n", nomeTecnologiaOrigem, nomeTecnologiaDestino);
            continue;
        }

        // Inicialmente, todas as distâncias são definidas como infinito, e nenhum vértice foi visitado
        for(int i = 0; i < grafo->numVertices; i++){
            distancia[i] = INT_MAX;
            visitado[i] = false;
        }

        // Marca o ponto de origem como tendo distância 0
        distancia[posOrigem] = 0;

        // Algoritmo de Dijkstra
        for (int contador = 0; contador < grafo->numVertices - 1; contador++) {

            // Encontra o vértice com a menor distância entre os vértices ainda não visitados
            int u = -1; 
            for (int v = 0; v < grafo->numVertices; v++) { 
                if (!visitado[v] && (u == -1 || distancia[v] < distancia[u])) { 
                    u = v;
                }
            }

            // Marca o vértice encontrado como visitado
            visitado[u] = true;

            // Atualiza as distâncias dos vértices adjacentes ao vértice encontrado
            Aresta *arestaAtual = grafo->vertices[u].arestaInicial;
            while(arestaAtual != NULL){
                
                // Encontra a posição do vértice de destino da aresta atual
                int v = buscaBinariaGrafo(grafo, arestaAtual->tecDestino);
                int peso = arestaAtual->peso;

                // Se o vértice de destino ainda não foi visitado e a distância atual é maior que a distância do vértice atual + o peso da aresta atual, atualizamos a distância
                if (!visitado[v] && distancia[u] != INT_MAX && distancia[u] + peso < distancia[v]) {
                    distancia[v] = distancia[u] + peso;
                }

                arestaAtual = arestaAtual->prox;
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
    free(distancia);
    free(visitado);
}
