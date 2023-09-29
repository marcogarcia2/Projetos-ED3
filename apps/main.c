#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "funcoesFornecidas.h"
#include "registros.h"
#include "funcoesCriadas.h"
#include "funcionalidade1.h"
#include "funcionalidade2.h"

// Depois (?) tem uma funçao que gera um binario aleatorio para checar se criou certo
// quando o campo do registro for vazio, colocar nulo

// comando linux para comparar binario cmp bin1.bin bin2.bin

int main (int argc, char *argv[]){
    
    int f;
    char nomeArquivoCSV[30], nomeArquivoBIN[30];
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

    case 3:


        break;


    case 4:

        break;


    default:
        break;
    }
    



    return 0;
}
