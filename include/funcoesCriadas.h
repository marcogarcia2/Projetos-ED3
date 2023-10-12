/*  
*   Lucas Lima Romero - 13676325
*   Marco Antonio Gaspar Garcia - 11833581
*/

#ifndef FUNCOES_CRIADAS_H
#define FUNCOES_CRIADAS_H

#include <stdio.h>
#include <stdlib.h>

#include "registros.h"

// Função que cria um Cabecalho
Cabecalho *criaCabecalho(void);

// Gravando o cabecalho nos 13 bytes iniciais
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

#endif
