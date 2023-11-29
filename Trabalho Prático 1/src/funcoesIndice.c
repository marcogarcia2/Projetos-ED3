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

void liberaNoArvoreB(NoArvoreB *no){
    free(no);
}
// ------------------ FUNÇÕES AUXILIARES ------------------ //
// As funções a seguir, foram criadas, a priori, para a funcionalidade 6

NoArvoreB *criaNoArvoreB(void){
    NoArvoreB *no = (NoArvoreB *) malloc(sizeof(NoArvoreB));

    no->nroChavesNo = no->alturaNo = 0;
    no->RRNdoNo = -1;

    for (int i = 0; i < 4; i++) {
        no->P[i] = -1;
        if (i < 3) {
            strcpy(no->C[i], "");
            no->PR[i] = -1;
        }
    }

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

    for (int i = 0; i < 4; i++) {
        fread(&noArvB->P[i], sizeof(int), 1, arquivoIND);
        if (i < 3) {
            leInformacaoUtil(arquivoIND, noArvB->C[i]);
            fread(&noArvB->PR[i], sizeof(int), 1, arquivoIND);
        }
    }
}

void imprimeNoArvoreB(NoArvoreB *no){
    printf("Nro de chaves: %d\n", no->nroChavesNo);
    printf("Altura do no: %d\n", no->alturaNo);
    printf("RRN do no: %d\n\n", no->RRNdoNo);

    // Escrevendo os ponteiros
    printf("P 1: %d\n", no->P[0]);
    printf("P 2: %d\n", no->P[1]);
    printf("P 3: %d\n", no->P[2]);
    
    // Escrevendo as chaves
    printf("Chave 1: %s\n", no->C[0]);
    printf("Chave 2: %s\n", no->C[1]);
    printf("Chave 3: %s\n", no->C[2]);

    // Escrevendo os ponteiros
    printf("PR 1: %d\n", no->PR[0]);
    printf("PR 2: %d\n", no->PR[1]);
    printf("PR 3: %d\n", no->PR[2]);
    
}

// ------------------ FUNÇÕES FUNCIONALIDADE 5 ------------------ //

DadosChave *criaDadosChave(void){
    DadosChave *dados = (DadosChave *) malloc(sizeof(DadosChave));

    dados->chave = NULL;
    dados->PR = -1;

    return dados;
}

// Função que insere no arquivo de índices
void insereArquivoIndice(DadosChave *dados, int RRNraiz, FILE *arquivoIND){
    /* --------- TIPOS DE INSERÇÃO ---------
        1- Inserção na raiz (RRNraiz == -1)
        2- Inserção sem divisão (quando tem espaço)
        3- Inserção com divisão (quando está cheio)
    */

    if(RRNraiz == -1){
        // Se o arquivo estiver vazio, vamos inserir na raiz
        //insereNaRaiz(dados, arquivoIND);
    }
    else{
        // Se não estiver vazio, vamos inserir recursivamente
        //insereRecursivamente(dados, arquivoIND, RRNraiz);
    }
}

// Ocorrerá quando o arquivo estiver vazio
void insereNaRaiz(DadosChave *dados, FILE *arquivoIND){

}

// Função que insere uma chave dentro do nó
void insereRecursivamente(DadosChave *dados, FILE *arquivoIND){
    // Aqui conterá a lógica de inserção recursiva que poderá ou não ter split
    // Se tiver, será chamada uma função para efetuar o split
}

// Função que efetua o split
void split(DadosChave *dados, FILE *arquivoIND){

}