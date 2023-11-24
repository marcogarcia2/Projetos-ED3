/*  
*   Lucas Lima Romero - 13676325
*   Marco Antonio Gaspar Garcia - 11833581
*/

#ifndef FUNCOES_INDICE_H
#define FUNCOES_INDICE_H

#include <stdio.h>

#include "registros.h"
#include "arvoreB.h"

// Função que cria um Cabeçalho de Árvore, inserido no início do arquivo binário de índice
CabecalhoArv *criaCabecalhoArv(void);

// Função que grava o Cabeçalho de Árvore no início do arquivo
void gravaCabecalhoArv(CabecalhoArv *cArv, FILE *arquivoIND);

// Função que cria um Cabeçalho de Nó, inserido antes de cada nó
CabecalhoNo *criaCabecalhoNo(void);

// Função que grava o Cabeçalho  
void gravaCabecalhoNo(CabecalhoNo *cNo, FILE *arquivoIND);

// Função que grava um nó (ou página) no arquivo binário de índice
void gravaNo(No *no, FILE *arquivoIND);

// liberaNo está na implementação do TAD de árvore B
// não pensei nas outras funções ainda

// 


#endif