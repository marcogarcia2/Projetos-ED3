/*  
*   Lucas Lima Romero - 13676325
*   Marco Antonio Gaspar Garcia - 11833581
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "arvoreB.h"

#define M 4
#define STRING_TAM 55

// // Estrutura que define um Nó (ou página) de Árvore B
// typedef struct _no_arv {
//     int nroChavesNo, alturaNo, RRNdoNo;
//     char *C[M+1]; // chaves de busca  // a 0 não é utilizada, e a 4 usamos temporariamente para o split
//                                       // portanto o número máximo de chaves por nó é 3
//     int P[M], PR[M-1];                // P guarda o RRN do arquivo de índice do nó filho
//                                       // R guarda o RRN da chave no arquivo binário anterior
//     struct _no_arv *ligacoes[M+1];
// } NoArv;

// // Estrutura do Cabeçalho do Arquivo de Índice da Árvore-B
// typedef struct _cab_indice{
//     char status;
//     int noRaiz;
//     int RRNproxNo;
// } CabecalhoIndice;

// Função interna que cria um novo nó de Árvore B
NoArv *criaNoArv(char *chave, NoArv *filho, NoArv *raiz) {
    NoArv *no;
    no = (NoArv*) malloc(sizeof(NoArv));
    no->C[1] = (char*) malloc(strlen(chave) + 1);
    strcpy(no->C[1], chave);
    no->nroChavesNo = 1;
    no->ligacoes[0] = raiz;
    no->ligacoes[1] = filho;
    
    
    //------------------------------------------------------------//   
    // Depois preencher corretamente (NAO SEI COMO kkkkkkkkkk)
    srand(time(NULL));
    no->alturaNo = rand() % 10;
    no->RRNdoNo = rand() % 1000;
    for (int i = 1; i < M; i++){
        if (i < M-1) no->PR[i] = rand() % 1000;
        no->P[i] = rand() % 1000;
    }
    //------------------------------------------------------------//

    return no;
}

// Função interna que insere as chaves em um nó ///////////////////// NAO SEI SE E ISSO MESMO
void inserirNo(char *chave, int pos, NoArv *no, NoArv *filho) {
    int j = no->nroChavesNo;
    while (j > pos) {
        no->C[j + 1] = malloc(strlen(no->C[j]) + 1);
        strcpy(no->C[j + 1], no->C[j]);
        no->ligacoes[j + 1] = no->ligacoes[j];
        j--;
    }
    no->C[j + 1] = malloc(strlen(chave) + 1);
    strcpy(no->C[j + 1], chave);
    no->ligacoes[j + 1] = filho;
    no->nroChavesNo++;
}

// Função interna que divide um nó de uma Árvore B
void split(char *chave, char **ptr, int pos, NoArv *noCheio, NoArv *filho, NoArv **novoNo) {
    int median, j;

    if (pos > M/2)
        median = M/2 + 1;
    else
        median = M/2;

    *novoNo = (NoArv *)malloc(sizeof(NoArv));
    j = median + 1;
    while (j <= M - 1) {
        (*novoNo)->C[j - median] = malloc(strlen(noCheio->C[j]) + 1);
        strcpy((*novoNo)->C[j - median], noCheio->C[j]);
        (*novoNo)->ligacoes[j - median] = noCheio->ligacoes[j];
        j++;
    }
    noCheio->nroChavesNo = median;
    (*novoNo)->nroChavesNo = M - 1 - median;

    if (pos <= M/2) {
        inserirNo(chave, pos, noCheio, filho);
    } else {
        inserirNo(chave, pos - median, *novoNo, filho);
    }
    *ptr = malloc(strlen(noCheio->C[noCheio->nroChavesNo]) + 1);
    strcpy(*ptr, noCheio->C[noCheio->nroChavesNo]);
    (*novoNo)->ligacoes[0] = noCheio->ligacoes[noCheio->nroChavesNo];
    noCheio->nroChavesNo--;
}

// Função interna que insere recursivamente uma chave em uma Árvore B
int inserirRecursivo(char *chave, char **ptr, NoArv *no, NoArv **filho) {
    int pos;
    if (!no) {
        *ptr = malloc(strlen(chave) + 1);
        strcpy(*ptr, chave);
        *filho = NULL;
        return 1;
    }

    if (strcmp(chave, no->C[1]) < 0) {
        pos = 0;
    } else {
        for (pos = no->nroChavesNo;
            (strcmp(chave, no->C[pos]) < 0 && pos > 1); pos--)
            ;
        if (strcmp(chave, no->C[pos]) == 0) {
            printf("Duplicates are not permitted\n");
            return 0;
        }
    }
    if (inserirRecursivo(chave, ptr, no->ligacoes[pos], filho)) {
        if (no->nroChavesNo < M - 1) {
            inserirNo(*ptr, pos, no, *filho);
        } else {
            split(*ptr, ptr, pos, no, *filho, filho);
            return 1;
        }
    }
    return 0;
}

// Função que insere uma chave em uma Árvore B
NoArv* inserirChave(char *chave, NoArv *raiz) {
    int flag;
    char *i = NULL;
    NoArv *filho;

    flag = inserirRecursivo(chave, &i, raiz, &filho);
    if (flag)
        raiz = criaNoArv(i, filho, raiz);
    return raiz;
}

// Função que busca uma chave em uma Árvore B
void buscarChave(char *chave, int *pos, NoArv *no) {
    if (!no) {
        return;
    }

    if (strcmp(chave, no->C[1]) < 0) {
        *pos = 0;
    } else {
        for (*pos = no->nroChavesNo;
            (strcmp(chave, no->C[*pos]) < 0 && *pos > 1); (*pos)--)
            ;
        if (strcmp(chave, no->C[*pos]) == 0) {
            printf("%s is found\n", chave);
            return;
        }
    }
    buscarChave(chave, pos, no->ligacoes[*pos]);

    return;
}

// Função que imprime todos as chaves da Árvore B em ordem alfabética
void imprimeArvoreB(NoArv *no) {
    int i;
    if (no) {
        for (i = 0; i < no->nroChavesNo; i++) {
            imprimeArvoreB (no->ligacoes[i]);
            printf("%s -> ", no->C[i + 1]);
        }
        imprimeArvoreB(no->ligacoes[i]);
    }
}

// Função que desaloca a memória de uma Árvore B
void destroiArvoreB(NoArv *no) {

    if (no != NULL) {

        int i;
        for (i = 1; i <= no->nroChavesNo; i++) {
            if (no->ligacoes[i]) {
                destroiArvoreB(no->ligacoes[i]);
            }
            if (no->C[i]) {
                free(no->C[i]);
            }
        }

        // Desloca a desalocação do nó para fora do loop
        free(no);
    }
}
