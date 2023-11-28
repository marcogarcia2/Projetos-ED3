/*  
*   Lucas Lima Romero - 13676325
*   Marco Antonio Gaspar Garcia - 11833581
*/

#ifndef FUNCOES_CRIADAS_H
#define FUNCOES_CRIADAS_H

#include <stdio.h>

#include "registros.h"
#include "arvoreB.h"

// Função que cria um Cabeçalho
Cabecalho *criaCabecalho(void);

// Função que grava o Cabeçalho nos 13 bytes iniciais
void gravaCabecalho(Cabecalho *c, FILE *arquivoBIN);

// Função que cria um Registro
Registro *criaRegistro(void);

// Função que desaloca a memória de um Registro
void liberaRegistro(Registro *r);

// Função que escreve um registor em um arquivo binário
void gravaRegistro(Registro *r, FILE *arquivoBIN);

// Função que lê um registro de um arquivo binário
Registro *leRegistro(int byteInicial, Registro *r, FILE *arquivoBIN);

// Função que imprime os campos de um registro
void imprimeRegistro(const Registro *r);

// Funcao que concatena as strings nomeTecnologiaOrigem e nomeTecnologiaDestino de um registro
char *concatenaStrings(Registro *r);

void buscaString(char *nomeCampo, char *tecnologia, int tamTotal, FILE *arquivoBIN);

void buscaInteiro(char *nomeCampo, int valor, int tamTotal, FILE *arquivoBIN);

int calculaTamanhoTotal(FILE *arquivoBIN);

#endif
