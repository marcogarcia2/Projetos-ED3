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
    char status; // '0' para inconsistente e '1' para consistente
    int noRaiz; // RRN do nó raiz
    int RRNproxNo; // RRN do próximo nó a ser inserido
} CabecalhoIndice;

// Estrutura do Nó da Árvore-B
typedef struct _noArvoreB{
    int nroChavesNo; // Número de chaves armazenadas no nó
    int alturaNo; // Altura do nó na árvore
    int RRNdoNo; // RRN do nó

    int P[4]; // Ponteiros para uma subárvore
    char C[3][55]; // Chaves de busca, sempre ordenadas C1 < C2 < C3
    int PR[3]; // Ponteiros para o registro associado a C[i] no arquivos de dados
} NoArvoreB;

// Estrutura auxiliar para dividir um nó
typedef struct _noSplit{
    int Psplit[5]; // Ponteiros para uma subárvore
    char Csplit[4][55]; // Chaves de busca, sempre ordenadas C1 < C2 < C3
    int PRsplit[4]; // Ponteiros para o registro associado a C[i] no arquivos de dados
} NoSplit;

// Estrutura auxiliar para inserir um nó
typedef struct _dadosChave{
    char *chave; // Chave de busca
    int PR; // Ponteiro para o registro associado a chave no arquivo de dados
    int rrnDireita; // RRN da subárvore à direita da chave
} DadosChave;

// Função que cria um Cabeçalho de Árvore, inserido no início do arquivo binário de índice
CabecalhoIndice *criaCabecalhoIndice(void);

// Função que lê um cabeçalho do arquivo de índices e grava em um CabecalhoIndice
void leCabecalhoIndice(CabecalhoIndice *cabIndice, FILE *arquivoIND);

// Função que grava um cabeçalho de árvore no arquivo de índices
void gravaCabecalhoIndice(CabecalhoIndice *c, FILE *arquivoIND);

// Função que cria um nó da árvore B
NoArvoreB *criaNoArvoreB(void);

// Função que lê um nó da árvore B do arquivo de índices e grava em um NoArvoreB
void leNoArvoreB(NoArvoreB *noArvB, FILE *arquivoIND);

// Função auxiliar para debugar
void imprimeNoArvoreB(NoArvoreB *no);

// Função que cria um DadosChave, que é usado para inserir no arquivo de índices
DadosChave *criaDadosChave(void);

// Função que adiciona um DadosChave no arquivo de índices
void adicionar(DadosChave *dados, FILE *arquivoIND, CabecalhoIndice *cabecalho);

// Função de busca binária que retorna o índice
int buscaBinaria(NoArvoreB *no, char *chave);

#endif
