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

    no->nroChavesNo = 0;
    no->alturaNo = 1;
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

    int i;
    for (i = 0; i < 3; i++) {
        printf("P %d: %d\n", i+1, no->P[i]);
        printf("Chave %d: %s\n", i+1, no->C[i]);
        printf("PR %d: %d\n", i+1, no->PR[i]);
    }
    printf("P %d: %d\n\n\n", i+1, no->P[i]);
}

// ------------------ FUNÇÕES FUNCIONALIDADE 5 ------------------ //

// Função que cria um registro
DadosChave *criaDadosChave(void){
    DadosChave *dados = (DadosChave *) malloc(sizeof(DadosChave));

    dados->chave = NULL;
    dados->PR = -1;

    return dados;
}

// Função que grava um nó inteiro num arquivo de árvore B
void gravaNoArvoreB(NoArvoreB *no, FILE *arquivoIND){
    // Gravando o nó no arquivo
    fwrite(&no->nroChavesNo, sizeof(int), 1, arquivoIND); // Inserindo o nro de chaves
    fwrite(&no->alturaNo, sizeof(int), 1, arquivoIND); // Inserindo a altura
    fwrite(&no->RRNdoNo, sizeof(int), 1, arquivoIND); // Inserindo o RRN do nó

    for (int i = 0; i < 4; i++) {
        fwrite(&no->P[i], sizeof(int), 1, arquivoIND);
        if (i < 3) {
            fwrite(no->C[i], sizeof(char), strlen(no->C[i]), arquivoIND);
            int len = strlen(no->C[i]);
            for (int j = len; j < 55; j++) {
                fputc('$', arquivoIND);
            }

            fwrite(&no->PR[i], sizeof(int), 1, arquivoIND);
        }
    }
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

    // Isso só acontece uma vez, quando o arquivo está vazio
    fseek(arquivoIND, TAM_PAGINA, SEEK_SET);
    NoArvoreB *no = criaNoArvoreB();

    no->nroChavesNo = 1;
    no->RRNdoNo = 0;
    
    strcpy(no->C[0], dados->chave);
    no->PR[0] = dados->PR;

    //gravaDadosIndice(dados, arquivoIND, ftell(arquivoIND));
    gravaNoArvoreB(no, arquivoIND);

    imprimeNoArvoreB(no);
    liberaNoArvoreB(no);
}

// Função que insere uma chave dentro do nó
void insereRecursivamente(DadosChave *dados, FILE *arquivoIND, int RRNraiz){
    // Aqui conterá a lógica de inserção recursiva que poderá ou não ter split
    // Se tiver, será chamada uma função para efetuar o split

    // Pula para o nó desejado (o primeiro será a raiz)
    fseek(arquivoIND, TAM_PAGINA + (RRNraiz * TAM_PAGINA), SEEK_SET); // Vou pro começo do nó da "raiz"
    int byteInicial = ftell(arquivoIND);
    int flagInseriu = 0;

    // Carrega o nó da raiz em memória principal
    NoArvoreB *no = criaNoArvoreB();
    leNoArvoreB(no, arquivoIND);

    // Vou fazer a checagem, se consigo colocar no nó atual
    if(no->nroChavesNo < ORDEM_M - 1){ 
        //printf("Inserindo no nó\n");
        
        // Se o nó não estiver cheio, insere no nó
        for (int i = 0; i < no->nroChavesNo; i++){ // Só entrará aqui se o nó tiver no máximo 2 chaves
            
            if(strcmp(dados->chave, no->C[i]) <= 0){

                if(strcmp(dados->chave, no->C[i]) == 0){
                    // Achamos a chave, retornamos seu ponteiro de referência para o arquivo de dados
                    printf("Elemento repetido!"); // Não é para ocorrer!
                    break;
                }

                else{ // Ou seja, se for menor na ordem alfabética, vamos inserir na sua esquerda
                    // Fazendo um shift dos elementos para a direita
                    printf("Inserindo no meio\n");
                    printf("Chave: %s\n", dados->chave);

                    for(int j = no->nroChavesNo - 1; j >= i; j--){
                        strcpy(no->C[j + 1], no->C[j]);
                        no->PR[j+1] = no->PR[j];
                    }
                    // Inserindo a chave e o PR
                    strcpy(no->C[i], dados->chave);
                    no->PR[i] = dados->PR;

                    flagInseriu = 1;
                    imprimeNoArvoreB(no);
                    break;
                }
            }

            // else // Se não for menor nem igual, vou para a próxima chave!
            //     continue;
        }

        // Fora do loop, precisamos saber se houve inserção (se não, inserimos no final)
        if(flagInseriu == 0){
            printf("Inserindo no final\n");
            printf("Chave: %s\n", dados->chave);
            imprimeNoArvoreB(no);
            strcpy(no->C[no->nroChavesNo], dados->chave);
            no->PR[no->nroChavesNo] = dados->PR;
            flagInseriu = 1;
        }

        if(flagInseriu == 1){
            // Incrementando o nro de chaves
            no->nroChavesNo++;
            fseek(arquivoIND, byteInicial, SEEK_SET);
            gravaNoArvoreB(no, arquivoIND);
        }
    }

    else{ // Se o nó estiver cheio, insere no nó e faz o split
        
        // printf("Inserindo no nó e fazendo split\n");
        // insereNo(no, dados, arquivoIND, RRNraiz);
        // splitNoArvore(dados, arquivoIND);
    }

}

// Função que efetua o split
void splitNoArvore(DadosChave *dados, FILE *arquivoIND){

}

// Função que insere no arquivo de índices
void insereArquivoIndice(DadosChave *dados, CabecalhoIndice *cabecalho, FILE *arquivoIND){
    /* --------- TIPOS DE INSERÇÃO ---------
        1- Inserção na raiz (RRNraiz == -1)
        2- Inserção sem divisão (quando tem espaço)
        3- Inserção com divisão (quando está cheio)
    */

    // NoArvoreB *no = criaNoArvoreB();
    //printf("Dado a ser inserido: %s\n", dados->chave);
    if(cabecalho->noRaiz == -1){
        // Se o arquivo estiver vazio, vamos inserir na raiz
        insereNaRaiz(dados, arquivoIND);
        cabecalho->noRaiz = 0;
        gravaCabecalhoIndice(cabecalho, arquivoIND);
    }
    else{
        // Se não estiver vazio, vamos inserir recursivamente
        insereRecursivamente(dados, arquivoIND, cabecalho->noRaiz);
    }
}
