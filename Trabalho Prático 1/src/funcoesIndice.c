/*  
*   Lucas Lima Romero - 13676325
*   Marco Antonio Gaspar Garcia - 11833581
*/

#include "funcoesIndice.h"

// Função que cria um Cabeçalho de Árvore, inserido no início do arquivo binário de índice
CabecalhoIndice *criaCabecalhoIndice(void){

    // Alocando a memória dinamicamente
    CabecalhoIndice *cabIndice = (CabecalhoIndice*) malloc(sizeof(CabecalhoIndice));

    // Inicializando com os valores padrão
    cabIndice->status = '0';
    cabIndice->noRaiz = -1;
    cabIndice->RRNproxNo = 0;

    return cabIndice;
}

// Função que grava o Cabeçalho de Árvore no início do arquivo
void gravaCabecalhoIndice(CabecalhoIndice *cabIndice, FILE *arquivoIND){

    // Voltando ao início do arquivo
    fseek(arquivoIND, 0, SEEK_SET);

    // Escrevendo os valores no arquivo
    fwrite(&cabIndice->status, sizeof(char), 1, arquivoIND);
    fwrite(&cabIndice->noRaiz, sizeof(int), 1, arquivoIND);
    fwrite(&cabIndice->RRNproxNo, sizeof(int), 1, arquivoIND);
    
    // Completando com lixo o restante do cabeçalho
    for(int i = 0; i < 196; i++)
        fwrite(LIXO, sizeof(char), 1, arquivoIND);
}

// Função que grava um nó (ou página) no arquivo binário de índice
void gravaNo(NoArv *no, FILE *arquivoIND){

    // printf("%s -> ", no->C[1]);
    // if (no->C[2]) printf("%s -> ", no->C[2]);
    // if (no->C[3]) printf("%s -> ", no->C[3]);
    // printf("\n");
    


    int tam = 0, i = 0;

    fwrite(&no->nroChavesNo, sizeof(int), 1, arquivoIND);
    fwrite(&no->alturaNo, sizeof(int), 1, arquivoIND);
    fwrite(&no->RRNdoNo, sizeof(int), 1, arquivoIND);

    
    // GRAVANDO: P1, C1, PR1

    fwrite(&no->P[1], sizeof(int), 1, arquivoIND);

    // Descobrindo o tamanho da chave
    while(no->C[1][i] != '\0'){
        tam++;
        i++;
    }
    fwrite(no->C[1], tam, 1, arquivoIND);
    for (int i = 0; i < STRING_TAM - tam; i++)
        fwrite(LIXO, sizeof(char), 1, arquivoIND);

    fwrite(&no->PR[1], sizeof(int), 1, arquivoIND);
    // FIM


    // GRAVANDO: P2, C2, PR2

    fwrite(&no->P[2], sizeof(int), 1, arquivoIND);
    if (no->C[2]){
        tam = 0;
        i = 0;
        // Descobrindo o tamanho da chave
        while(no->C[2][i] != '\0'){
            tam++;
            i++;
        }
        fwrite(no->C[2], tam, 1, arquivoIND);
        for (int i = 0; i < STRING_TAM - tam; i++)
            fwrite(LIXO, sizeof(char), 1, arquivoIND);
    }
    else {
        for (int i = 0; i < STRING_TAM; i++)
            fwrite(LIXO, sizeof(char), 1, arquivoIND);
    }

    fwrite(&no->PR[2], sizeof(int), 1, arquivoIND);
    // FIM

    // GRAVANDO: P3, C3, PR3

    fwrite(&no->P[3], sizeof(int), 1, arquivoIND);
    if (no->C[3]){
        tam = 0;
        i = 0;
        // Descobrindo o tamanho da chave
        while(no->C[3][i] != '\0'){
            tam++;
            i++;
        }
        fwrite(no->C[3], tam, 1, arquivoIND);
        for (int i = 0; i < STRING_TAM - tam; i++)
            fwrite(LIXO, sizeof(char), 1, arquivoIND);
    }
    else {
        for (int i = 0; i < STRING_TAM; i++)
            fwrite(LIXO, sizeof(char), 1, arquivoIND);
    }
    
    fwrite(&no->PR[3], sizeof(int), 1, arquivoIND);
    // FIM

    fwrite(&no->P[4], sizeof(int), 1, arquivoIND);  
}

// Função que grava toda a Árvore B em um arquivo binário de índice
void gravaArvore(NoArv *no, FILE *arquivoIND){

    if(no) {
        
        for (int i = 0; i <= no->nroChavesNo; i++){
            gravaArvore(no->ligacoes[i], arquivoIND);
        }

        gravaNo(no, arquivoIND);

    }
}
