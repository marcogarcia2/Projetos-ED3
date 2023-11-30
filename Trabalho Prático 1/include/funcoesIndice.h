/*  
*   Lucas Lima Romero - 13676325
*   Marco Antonio Gaspar Garcia - 11833581
*/

#ifndef FUNCOES_INDICE_H
#define FUNCOES_INDICE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "registros.h"

#define TAM_PAGINA 205
#define ORDEM_M 4

// Estrutura do Cabeçalho do Arquivo de Índice da Árvore-B
typedef struct _cab_indice{
    char status;
    int noRaiz;
    int RRNproxNo;
} CabecalhoIndice;

typedef struct _noArvoreB{
    int nroChavesNo;
    int alturaNo;
    int RRNdoNo;

    int P[4]; // Ponteiros para uma subárvore
    char C[3][55]; // Chaves de busca, sempre ordenadas C1 < C2 < C3
    int PR[3]; // Ponteiros para o registro associado a Ci no arquivos de dados
} NoArvoreB;

typedef struct _noSplit{
    int Psplit[5];
    char Csplit[4][55];
    int PRsplit[4];
} NoSplit;

typedef struct _dadosChave{
    char *chave;
    int PR;
    int rrnDireita;
} DadosChave;

// Função que cria um Cabeçalho de Árvore, inserido no início do arquivo binário de índice
CabecalhoIndice *criaCabecalhoIndice(void);

// Função que grava um cabeçalho de árvore no arquivo de índices
void gravaCabecalhoIndice(CabecalhoIndice *c, FILE *arquivoIND);

// Função que cria um nó da árvore B
NoArvoreB *criaNoArvoreB(void);

// Função que lê um nó da árvore B do arquivo de índices e grava em um NoArvoreB
void leNoArvoreB(NoArvoreB *noArvB, FILE *arquivoIND);

// Função auxiliar para debugar
void imprimeNoArvoreB(NoArvoreB *no);

// Função que toma conta da inserção no arquivo de índices
void insereArquivoIndice(DadosChave *dados, CabecalhoIndice *cabecalho, FILE *arquivoIND);

// Função que cria um DadosChave, que é usado para inserir no arquivo de índices
DadosChave *criaDadosChave(void);

#endif
