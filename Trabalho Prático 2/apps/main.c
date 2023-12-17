/*  
*   Lucas Lima Romero - 13676325
*   Marco Antonio Gaspar Garcia - 11833581
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "funcoesFornecidas.h"
#include "registros.h"
#include "funcoesCriadas.h"
#include "funcionalidades.h"

// main: switch case com as funcionalidades 

int main (int argc, char *argv[]){
    
    int f; // Funcionalidade
    char nomeArquivoBIN[30]; // Nome do arquivo binário
    int n; // Número de vezes que será executada alguma funcionalidade
    scanf("%d ", &f);

    switch (f){

    case 8: // Funcionalidade 8
        scanf("%s", nomeArquivoBIN);
        exibeGrafo(nomeArquivoBIN);
        break;

    case 9: // Funcionalidade 9
        scanf("%s", nomeArquivoBIN);
        exibeGrafoTransposto(nomeArquivoBIN);
        break;

    case 10: // Funcionalidade 10
        scanf("%s %d", nomeArquivoBIN, &n);
        listaTecnologias(nomeArquivoBIN, n);
        break;

    case 11: // Funcionalidade 10
        scanf("%s", nomeArquivoBIN);
        kosaraju(nomeArquivoBIN);
        break;

    case 12: // Funcionalidade 10
        scanf("%s %d", nomeArquivoBIN, &n);
        dijkstra(nomeArquivoBIN, n);
        break;

    default:
        break;
    }

    return 0;
}
