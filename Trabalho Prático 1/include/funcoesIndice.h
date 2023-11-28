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

    int P1, P2, P3, P4; // Ponteiros para uma subárvore
    char C1[55], C2[55], C3[55]; // Chaves de busca, sempre ordenadas C1 < C2 < C3
    int PR1, PR2, PR3; // Ponteiros para o registro associado a Ci no arquivos de dados
} NoArvoreB;

NoArvoreB *criaNoArvoreB(void);

// Função que lê um nó da árvore B do arquivo de índices e grava em um NoArvoreB
void leNoArvoreB(NoArvoreB *noArvB, FILE *arquivoIND);

void imprimeNoArvoreB(NoArvoreB *no);

#endif
