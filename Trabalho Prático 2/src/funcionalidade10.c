/*  
*   Lucas Lima Romero - 13676325
*   Marco Antonio Gaspar Garcia - 11833581
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "funcionalidades.h"
#include "registros.h"
#include "funcoesCriadas.h"
#include "funcoesFornecidas.h"
#include "grafo.h"

// Funcionalidade 10: lista todas as tecnologias de origem associadas a "n" tecnologias de destino dadas pelo usuário
void buscaTecnologia(Grafo *grafoTransposto, char *nomeTecnologiaDestino){

    // Calculando a posição desejada no grafo
    int posDestino = buscaBinariaGrafo(grafoTransposto, nomeTecnologiaDestino);

    // O vértice não existe no grafo
    if(strcmp(grafoTransposto->vertices[posDestino].tecnologia, nomeTecnologiaDestino) != 0){
        printf("Registro inexistente.\n");
        return;
    }

    // Se não, quer dizer que existe e precisamos listar as arestas associadas (tecnologias de origem)

    // Criando a aresta que percorrerá o grafo, começando na inicial do vértice desejado
    Aresta *arestaAtual = grafoTransposto->vertices[posDestino].arestaInicial;

    // Imprimindo o nome da tecnologia de destino
    printf("%s:", nomeTecnologiaDestino);

    // Imprimindo as tecnologias de origem
    while(arestaAtual != NULL){
        printf(" %s", arestaAtual->tecDestino);
        if(arestaAtual->prox != NULL) {
            printf(",");
        }
        arestaAtual = arestaAtual->prox;
    }

    // Pulando uma linha para a próxima execução
    printf("\n\n");
}

void listaTecnologias(char *nomeArquivoBIN, int n){

    // Obtendo o grafo referente ao arquivo binário
    Grafo *grafo = gerarGrafo(nomeArquivoBIN);

    if (grafo == NULL) return;

    // Tomemos o grafo transposto
    Grafo *grafoTransposto = transpor(grafo);
    
    // Variável auxiliar
    char nomeTecnologiaDestino[30];

    for(int i = 0; i < n; i++){

        // Lê a tecnologia de destino
        scan_quote_string(nomeTecnologiaDestino);

        // Busca a tecnologia de destino no grafo transposto e recupera as de origem
        buscaTecnologia(grafoTransposto, nomeTecnologiaDestino);
    }

    // Liberando a memória
    destroiGrafo(grafo);
    destroiGrafo(grafoTransposto);
}
