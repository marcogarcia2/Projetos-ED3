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

// ------------------ FUNÇÕES AUXILIARES ------------------ //
// As funções a seguir, foram criadas, a priori, para a funcionalidade 6

NoArvoreB *criaNoArvoreB(void){
    NoArvoreB *no = (NoArvoreB *) malloc(sizeof(NoArvoreB));

    no->nroChavesNo = no->alturaNo = 0;
    no->RRNdoNo = -1;

    no->P1 = no->P2 = no->P3 = no->P4 = -1;
    strcpy(no->C1, "");
    strcpy(no->C2, "");
    strcpy(no->C3, "");
    no->PR1 = no->PR2 = no->PR3 = -1;

    return no;
}

// Função auxiliar da leNoArvoreB
void leInformacaoUtil(FILE *arquivoIND, char *Ci) {
    // Lê todos os 55 bytes (preciso disso, para não ser necessário dar fseeks)
    fread(Ci, sizeof(char), 55, arquivoIND);

    // Encontra e substitui o primeiro '$' por '\0'
    for (int i = 0; i < 55; i++) {
        if (Ci[i] == '$') {
            Ci[i] = '\0';
            break;
        }
    }
}

// Função que lê um nó da árvore B do arquivo de índices e grava em um NoArvoreB
void leNoArvoreB(NoArvoreB *noArvB, FILE *arquivoIND){
    fread(&noArvB->nroChavesNo, sizeof(int), 1, arquivoIND);
    fread(&noArvB->alturaNo, sizeof(int), 1, arquivoIND);
    fread(&noArvB->RRNdoNo, sizeof(int), 1, arquivoIND);

    fread(&noArvB->P1, sizeof(int), 1, arquivoIND);
    leInformacaoUtil(arquivoIND, noArvB->C1);
    fread(&noArvB->PR1, sizeof(int), 1, arquivoIND);

    fread(&noArvB->P2, sizeof(int), 1, arquivoIND);
    leInformacaoUtil(arquivoIND, noArvB->C2);
    fread(&noArvB->PR2, sizeof(int), 1, arquivoIND);

    fread(&noArvB->P3, sizeof(int), 1, arquivoIND);
    leInformacaoUtil(arquivoIND, noArvB->C3);
    fread(&noArvB->PR3, sizeof(int), 1, arquivoIND);

    fread(&noArvB->P4, sizeof(int), 1, arquivoIND);
}

void imprimeNoArvoreB(NoArvoreB *no){
    printf("Nro de chaves: %d\n", no->nroChavesNo);
    printf("Altura do no: %d\n", no->alturaNo);
    printf("RRN do no: %d\n\n", no->RRNdoNo);
    
    printf("P1: %d\n", no->P1);
    printf("P2: %d\n", no->P2);
    printf("P3: %d\n", no->P3);
    printf("P4: %d\n\n", no->P4);

    printf("C1: %s\n", no->C1);
    printf("C2: %s\n", no->C2);
    printf("C3: %s\n\n", no->C3);

    printf("PR1: %d\n", no->PR1);
    printf("PR2: %d\n", no->PR2);
    printf("PR3: %d\n", no->PR3);
}
