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

// Função que cria um registro
DadosChave *criaDadosChave(void){
    DadosChave *dados = (DadosChave *) malloc(sizeof(DadosChave));

    dados->chave = NULL;
    dados->PR = -1;

    return dados;
}

// Função que grava os dados de apenas uma chave
void gravaDadosIndice(DadosChave *dados, FILE *arquivoIND, int byteOffset){
    fseek(arquivoIND, byteOffset, SEEK_SET);

    int P = -1; // Indicação de nó folha

    // Gravando o ponteiro
    fwrite(&P, sizeof(int), 1, arquivoIND);

    // Gravando chave
    fwrite(dados->chave, sizeof(char), strlen(dados->chave), arquivoIND);

    // Lógica para gravar o lixo
    int len = strlen(dados->chave);
    while (len < 55) {
        fputc('$', arquivoIND);
        len++;
    }

    // Gravando PR
    fwrite(&dados->PR, sizeof(int), 1, arquivoIND);
}

// Ocorrerá quando o arquivo estiver vazio
void insereNaRaiz(DadosChave *dados, FILE *arquivoIND){
    NoArvoreB *no = criaNoArvoreB();

    fwrite(&no->nroChavesNo, sizeof(int), 1, arquivoIND); // Inserindo o nro de chaves
    fwrite(&no->alturaNo, sizeof(int), 1, arquivoIND); // Inserindo a altura
    fwrite(&no->RRNdoNo, sizeof(int), 1, arquivoIND); // Inserindo o RRN do nó

    gravaDadosIndice(dados, arquivoIND, ftell(arquivoIND));

    // Inserindo os dados no nó

}

// Função que insere uma chave dentro do nó
void insereRecursivamente(DadosChave *dados, FILE *arquivoIND){
    // Aqui conterá a lógica de inserção recursiva que poderá ou não ter split
    // Se tiver, será chamada uma função para efetuar o split
}

// Função que efetua o split
void splitNoArvore(DadosChave *dados, FILE *arquivoIND){

}

// Função que insere no arquivo de índices
void insereArquivoIndice(DadosChave *dados, int RRNraiz, FILE *arquivoIND){
    /* --------- TIPOS DE INSERÇÃO ---------
        1- Inserção na raiz (RRNraiz == -1)
        2- Inserção sem divisão (quando tem espaço)
        3- Inserção com divisão (quando está cheio)
    */
    // NoArvoreB *no = criaNoArvoreB();

    if(RRNraiz == -1){
        // Se o arquivo estiver vazio, vamos inserir na raiz
        insereNaRaiz(dados, arquivoIND);
    }
    else{
        // Se não estiver vazio, vamos inserir recursivamente
        //insereRecursivamente(dados, arquivoIND, RRNraiz);
    }
}
