/*  
*   Lucas Lima Romero - 13676325
*   Marco Antonio Gaspar Garcia - 11833581
*/

#include <stdio.h>
#include <stdlib.h>

#include "arvoreB.h"
#include "registros.h"
#include "funcoesCriadas.h"

// Criando uma estrutura Cabecalho
Cabecalho *criaCabecalho(void){
    
    // Alocando memória dinamicamente para o cabeçalho
    Cabecalho *c = (Cabecalho*) malloc(sizeof(Cabecalho));

    // inicializando seus atributos com valores vazios
    c->status = '0';
    c->proxRRN = 0;
    c->nroTecnologias = 0;
    c->nroParesTecnologias = 0;

    return c;
}

// Gravando o cabecalho nos 13 bytes iniciais
void gravaCabecalho(Cabecalho *c, FILE *arquivoBIN){

    // Voltando ao início do arquivo
    fseek(arquivoBIN, 0, SEEK_SET);

    // Escrevendo os valores adequadamente
    fwrite(&c->status, sizeof(char), 1, arquivoBIN);
    fwrite(&c->proxRRN, sizeof(int), 1, arquivoBIN);
    fwrite(&c->nroTecnologias, sizeof(int), 1, arquivoBIN);
    fwrite(&c->nroParesTecnologias, sizeof(int), 1, arquivoBIN);
}

// Função que cria um Registro
Registro *criaRegistro(void){
    
    Registro *r = (Registro *) malloc(sizeof(Registro));

    // inicializando os atributos com um valor vazio
    r->removido = '0';
    r->grupo = -1;
    r->popularidade = -1;
    r->peso = -1;

    r->tecnologiaOrigem.tamanho = 0; 
    r->tecnologiaOrigem.string = NULL;

    r->tecnologiaDestino.tamanho = 0; 
    r->tecnologiaDestino.string = NULL;
    
    return r;
}

// Função que desaloca a memória de um Registro
void liberaRegistro(Registro *r){

    if (r->tecnologiaOrigem.string != NULL)
        free(r->tecnologiaOrigem.string);
    
    if (r->tecnologiaDestino.string != NULL)
        free(r->tecnologiaDestino.string);
    
    if (r != NULL)
        free(r);

    return;
}

// Função que escreve um registro em um arquivo binário
void gravaRegistro(Registro *r, FILE *arquivoBIN){ 

    // Escrevendo na ordem apropriada
    fwrite(&r->removido, sizeof(char), 1, arquivoBIN);
    fwrite(&r->grupo, sizeof(int), 1, arquivoBIN);
    fwrite(&r->popularidade, sizeof(int), 1, arquivoBIN);
    fwrite(&r->peso, sizeof(int), 1, arquivoBIN);
    fwrite(&r->tecnologiaOrigem.tamanho, sizeof(int), 1, arquivoBIN);
    fwrite(r->tecnologiaOrigem.string, r->tecnologiaOrigem.tamanho, 1, arquivoBIN);
    fwrite(&r->tecnologiaDestino.tamanho, sizeof(int), 1, arquivoBIN);
    fwrite(r->tecnologiaDestino.string, r->tecnologiaDestino.tamanho, 1, arquivoBIN);

    // Calculando a quantidade de lixo adequada e escrevendo-os
    int tamLixo = TAM_REGISTRO - (TAM_REGISTRO_FIXO + r->tecnologiaOrigem.tamanho + r->tecnologiaDestino.tamanho);
    for(int i = 0; i < tamLixo; i++)
        fwrite(LIXO, sizeof(char), 1, arquivoBIN);

}

// Função que lê um registro de um arquivo binário
Registro *leRegistro(int byteInicial, Registro *r, FILE *arquivoBIN){

    // O byteInicial é o byte do campo removido

    // Vou para o byte inicial do registro
    fseek(arquivoBIN, byteInicial, SEEK_SET); 

    // Detecta se existe um registro neste byteOffset
    int flag = fread(&r->removido, sizeof(char), 1, arquivoBIN);
    if (flag == 0) return NULL; // Se não leu nada, retorna nulo, pois não existem mais registros
    

    // Lendo os outros campos na ordem apropriada
    fread(&r->grupo, sizeof(int), 1, arquivoBIN);
    fread(&r->popularidade, sizeof(int), 1, arquivoBIN);
    fread(&r->peso, sizeof(int), 1, arquivoBIN);

    // As strings variáveis devem ser alocadas corretamente com malloc

    fread(&r->tecnologiaOrigem.tamanho, sizeof(int), 1, arquivoBIN);
    r->tecnologiaOrigem.string = (char *)malloc(r->tecnologiaOrigem.tamanho+1);
    fread(r->tecnologiaOrigem.string, r->tecnologiaOrigem.tamanho, 1, arquivoBIN);

    fread(&r->tecnologiaDestino.tamanho, sizeof(int), 1, arquivoBIN);
    r->tecnologiaDestino.string = (char *)malloc(r->tecnologiaDestino.tamanho+1);
    fread(r->tecnologiaDestino.string, r->tecnologiaDestino.tamanho, 1, arquivoBIN);

    r->tecnologiaOrigem.string[r->tecnologiaOrigem.tamanho] = '\0';
    r->tecnologiaDestino.string[r->tecnologiaDestino.tamanho] = '\0';

    // O ponteiro do arquivo está no final do registro.

    return r;
}

// Função que imprime os campos de um registro
void imprimeRegistro(const Registro *r){
    // Caso os inteiros sejam -1 ou o tamanho das strings sejam 0, imprime-se "NULO"
    (r->tecnologiaOrigem.tamanho != 0) ? printf("%s, ", r->tecnologiaOrigem.string) : printf("NULO, ");
    (r->grupo != -1) ? printf("%d, ", r->grupo) : printf("NULO, ");
    (r->popularidade != -1) ? printf("%d, ", r->popularidade) : printf("NULO, ");
    (r->tecnologiaDestino.tamanho != 0) ? printf("%s, ", r->tecnologiaDestino.string) : printf("NULO, ");
    (r->peso != -1) ? printf("%d\n", r->peso) : printf("NULO\n");
}

// Função que grava o cabeçalho de um arquivo de índice
void gravaCabecalhoIndice(CabecalhoIndice *c, FILE *arquivoIND){

    // Voltando ao início do arquivo
    fseek(arquivoIND, 0, SEEK_SET);

    // Escrevendo os valores adequadamente
    fwrite(&c->status, sizeof(char), 1, arquivoIND);
    fwrite(&c->noRaiz, sizeof(int), 1, arquivoIND);
    fwrite(&c->RRNproxNo, sizeof(int), 1, arquivoIND);
}
