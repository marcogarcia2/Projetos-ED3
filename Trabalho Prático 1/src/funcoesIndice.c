/*  
*   Lucas Lima Romero - 13676325
*   Marco Antonio Gaspar Garcia - 11833581
*/

#include "funcoesIndice.h"

// Função que cria um Cabeçalho de Árvore, inserido no início do arquivo binário de índice
CabecalhoIndice *criaCabecalhoIndice(void){

    // Alocando a memória dinamicamente
    CabecalhoIndice *cIndice = (CabecalhoIndice*) malloc(sizeof(CabecalhoIndice));

    // Inicializando com os valores padrão
    cIndice->status = '0';
    cIndice->noRaiz = -1;
    cIndice->RRNproxNo = 0;

    return cIndice;
}

// Função que grava o Cabeçalho de Árvore no início do arquivo
void gravaCabecalhoIndice(CabecalhoIndice *cIndice, FILE *arquivoIND){

    // Voltando ao início do arquivo
    fseek(arquivoIND, 0, SEEK_SET);

    // Escrevendo os valores no arquivo
    fwrite(&cIndice->status, sizeof(char), 1, arquivoIND);
    fwrite(&cIndice->noRaiz, sizeof(int), 1, arquivoIND);
    fwrite(&cIndice->RRNproxNo, sizeof(int), 1, arquivoIND);
    
    // Completando com lixo o restante do cabeçalho
    for(int i = 0; i < 196; i++)
        fwrite(LIXO, sizeof(char), 1, arquivoIND);
}

// Função que grava um nó (ou página) no arquivo binário de índice
void gravaNo(No *no, FILE *arquivoIND){

    fwrite(&no->nroChavesNo, sizeof(int), 1, arquivoIND);
    fwrite(&no->alturaNo, sizeof(int), 1, arquivoIND);
    fwrite(&no->RRNdoNo, sizeof(int), 1, arquivoIND);

    fwrite(&no->P[0], sizeof(int), 1, arquivoIND);
    fwrite(no->C[0], STRING_TAM, 1, arquivoIND);
    fwrite(&no->PR[0], sizeof(int), 1, arquivoIND);

    fwrite(&no->P[1], sizeof(int), 1, arquivoIND);
    fwrite(no->C[1], STRING_TAM, 1, arquivoIND);
    fwrite(&no->PR[1], sizeof(int), 1, arquivoIND);

    fwrite(&no->P[2], sizeof(int), 1, arquivoIND);
    fwrite(no->C[2], STRING_TAM, 1, arquivoIND);
    fwrite(&no->PR[2], sizeof(int), 1, arquivoIND);

    fwrite(&no->P[3], sizeof(int), 1, arquivoIND);    
}
