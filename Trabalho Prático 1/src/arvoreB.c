/*  
*   Lucas Lima Romero - 13676325
*   Marco Antonio Gaspar Garcia - 11833581
*/

#include "arvoreB.h"

CabecalhoIndice *criaCabecalhoIndice(void){

    // Alocando memória dinamicamente
    CabecalhoIndice *cabecalho = (CabecalhoIndice*) malloc(sizeof(CabecalhoIndice));

    // Iniciando com os parâmetros iniciais
    cabecalho->status = '0';
    cabecalho->noRaiz = -1;
    cabecalho->RRNproxNo = 0;

    return cabecalho;

}

// Função que cria um Cabeçalho de Nó, inserido antes de cada nó
No *criaNo(void){

    // Alocando a memória dinamicamente
    No *no = (No*) malloc(sizeof(No));

    // Iniciando com valores nulos, precisam ser lidos da Árvore B
    no->nroChavesNo = -1;
    no->alturaNo = -1;
    no->RRNdoNo = -1;
    for (int i = 0; i < 4; i++){
        for(int j = 0; j < STRING_TAM; j++){

            // Preenchendo com lixo, para facilitar posteriormente a escrita
            no->C[i][j] = '$'; //LIXO???
        }
    }

    return no;
}

