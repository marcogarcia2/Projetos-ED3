/*  
*   Lucas Lima Romero - 13676325
*   Marco Antonio Gaspar Garcia - 11833581
*/

#ifndef FUNCOES_CRIADAS_H
#define FUNCOES_CRIADAS_H

#include <stdio.h>
#include <stdlib.h>

#include "registros.h"

Cabecalho *criaCabecalho(void);

// Gravando o cabecalho em 13 bytes
void gravaCabecalho(Cabecalho *c, FILE *arquivoBIN);

Registro *criaRegistro(void);

Registro *resetaRegistro(Registro *r);

void liberaRegistro(Registro *r);

void gravaRegistro(Registro *r, FILE *arquivoBIN);

Registro *leRegistro(FILE *arquivoBIN, int byteInicial, Registro *r);

void imprimeRegistro(Registro *r);

#endif
