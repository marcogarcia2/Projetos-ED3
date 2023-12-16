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
#include "grafo.h"

// Funcionalidade 8: gera um Grafo a partir de um arquivo binário

void exibeGrafo(char *nomeArquivoBIN){ 

    // Lendo o arquivo binário e gerando o grafo
    Grafo *grafo = gerarGrafo(nomeArquivoBIN);

    if (grafo != NULL){

        // Imprimindo o grafo
        imprimeGrafo(grafo);

        // Liberando a memória
        destroiGrafo(grafo);
    }
}
