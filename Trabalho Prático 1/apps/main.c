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
    char nomeArquivoCSV[30], nomeArquivoBIN[30], nomeArquivoIND[30]; // Strings da funcionalidade 1
    int n; // Número de vezes que será executada alguma funcionalidade
    int rrn; // RRN utilizado na funcionalidade 4
    scanf("%d ", &f);

    switch (f){

    case 1: // Funcionalidade 1
        scanf("%s %s", nomeArquivoCSV, nomeArquivoBIN);
        criaTabela(nomeArquivoCSV, nomeArquivoBIN);
        break;
    
    case 2: // Funcionalidade 2
        scanf("%s", nomeArquivoBIN);
        recuperaDados(nomeArquivoBIN);
        break;

    case 3: // Funcionalidade 3
        scanf("%s %d", nomeArquivoBIN, &n);
        buscaPorCampo(nomeArquivoBIN, n);
        break;

    case 4: // Funcionalidade 4
        scanf("%s %d", nomeArquivoBIN, &rrn);
        buscaPorRRN(nomeArquivoBIN, rrn);
        break;

    case 5: // Funcionalidade 5
        scanf("%s %s", nomeArquivoBIN, nomeArquivoIND);
        geraArquivoIndice(nomeArquivoBIN, nomeArquivoIND);
        break;

    case 6: // Funcionalidade 6
        scanf("%s %s %d", nomeArquivoBIN, nomeArquivoIND, &n);
        buscaComIndice(nomeArquivoBIN, nomeArquivoIND, n);
        break;

    default:
        break;
    }
    

    return 0;
}
