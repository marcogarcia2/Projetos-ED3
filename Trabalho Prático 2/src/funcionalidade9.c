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

// Funcionalidade 9: gera um Grafo Transposto a partir de um arquivo binário

void exibeGrafoTransposto(char *nomeArquivoBIN){ 

    // Lendo o arquivo binário e gerando o grafo
    Grafo *grafo = gerarGrafo(nomeArquivoBIN);
    
    // Se o grafo for nulo, encerramos a função
    if (grafo == NULL) return;

    // Transpondo o grafo
    Grafo *grafoTransposto = transpor(grafo);
    imprimeGrafo(grafoTransposto);

    // Liberando a memória
    destroiGrafo(grafo);
    destroiGrafo(grafoTransposto);
    
}
