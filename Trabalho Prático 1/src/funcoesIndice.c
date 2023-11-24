/*  
*   Lucas Lima Romero - 13676325
*   Marco Antonio Gaspar Garcia - 11833581
*/

#include "funcoesIndice.h"

// Função que cria um Cabeçalho de Árvore, inserido no início do arquivo binário de índice
CabecalhoArv *criaCabecalhoArv(void){

    // Alocando a memória dinamicamente
    CabecalhoArv *cArv = (Cabecalho*) malloc(sizeof(Cabecalho));

    // Inicializando com os valores padrão
    cArv->status = '0';
    cArv->noRaiz = -1;
    cArv->RRNproxNo = 0;

    return cArv;
}

// Função que grava o Cabeçalho de Árvore no início do arquivo
void gravaCabecalhoArv(CabecalhoArv *cArv, FILE *arquivoIND){

    // Voltando ao início do arquivo
    fseek(arquivoIND, 0, SEEK_SET);

    // Escrevendo os valores no arquivo
    fwrite(&cArv->status, sizeof(char), 1, arquivoIND);
    fwrite(&cArv->noRaiz, sizeof(int), 1, arquivoIND);
    fwrite(&cArv->RRNproxNo, sizeof(int), 1, arquivoIND);
    
    // Completando com lixo o restante do cabeçalho
    for(int i = 0; i < 196; i++)
        fwrite(LIXO, sizeof(char), 1, arquivoIND);
}

// Função que cria um Cabeçalho de Nó, inserido antes de cada nó
CabecalhoNo *criaCabecalhoNo(void){

    // Alocando a memória dinamicamente
    CabecalhoNo *cNo = (No*) malloc(sizeof(CabecalhoNo));

    // Iniciando com valores nulos, precisam ser lidos da Árvore B
    cNo->nroChavesNo = -1;
    cNo->alturaNo = -1;
    cNo->RRNdoNo = -1;

    return cNo;
}

// Função que grava o Cabeçalho  
void gravaCabecalhoNo(CabecalhoNo *cNo, FILE *arquivoIND){

    // DEPOIS QUE cNo ESTÁ COM OS VALORES CORRETOS
    fwrite(&cNo->nroChavesNo, sizeof(int), 1, arquivoIND);
    fwrite(&cNo->alturaNo, sizeof(int), 1, arquivoIND);
    fwrite(&cNo->RRNdoNo, sizeof(int), 1, arquivoIND);    
}

// Função que grava um nó (ou página) no arquivo binário de índice
void gravaNo(No *no, FILE *arquivoIND){

    /*
    fwrite(&no->P1, sizeof(int), 1, arquivoIND);
    fwrite(no->C1, sizeof(C1->tamanho???), 1, arquivoIND);
    fwrite(lixo restante)
    fwrite(&no->PR1, sizeof(int), 1, arquivoIND);

    ...


    */
}
