/*  
*   Lucas Lima Romero - 13676325
*   Marco Antonio Gaspar Garcia - 11833581
*/

#ifndef FUNCOES_INDICE_H
#define FUNCOES_INDICE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "registros.h"
#include "arvoreB.h"

#define TAM_PAGINA 205

// Função que cria um Cabeçalho de Árvore, inserido no início do arquivo binário de índice
CabecalhoIndice *criaCabecalhoIndice(void);

// Função que grava o Cabeçalho de Árvore no início do arquivo
void gravaCabecalhoIndice(CabecalhoIndice *cIndice, FILE *arquivoIND);

// Função que grava um nó (ou página) no arquivo binário de índice
void gravaNo(NoArv *no, FILE *arquivoIND);

// Função que grava toda a Árvore B em um arquivo binário de índice
void gravaArvore(NoArv *raiz, FILE *arquivoIND);

typedef struct _noArvoreB{
    int nroChavesNo;
    int alturaNo;
    int RRNdoNo;

    int P[4]; // Ponteiros para uma subárvore
    char C[3][55]; // Chaves de busca, sempre ordenadas C1 < C2 < C3
    int PR[3]; // Ponteiros para o registro associado a Ci no arquivos de dados
} NoArvoreB;

typedef struct _dadosChave{
    char *chave;
    int PR;
} DadosChave;


NoArvoreB *criaNoArvoreB(void);

// Função que lê um nó da árvore B do arquivo de índices e grava em um NoArvoreB
void leNoArvoreB(NoArvoreB *noArvB, FILE *arquivoIND);

// Função auxiliar para debugar
void imprimeNoArvoreB(NoArvoreB *no);

// Função que toma conta da inserção no arquivo de índices
void insereArquivoIndice(DadosChave *dados, int RRNraiz, FILE *arquivoIND);

// Função que cria um DadosChave, que é usado para inserir no arquivo de índices
DadosChave *criaDadosChave(void);

#endif
