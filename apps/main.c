#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

//#include "funcoesFornecidas.h"

#define TAM_REGISTRO 76
#define TAM_REGISTRO_FIXO 21
#define LIXO '$'

typedef struct{ // tamanho fixo de 13 bytes - Cabecalho
    char status;
    int proxRRN;
    int nroTecnologias; // indica o nro de tecnologias unicas de origem
    int nroParesTecnologias;
} Cabecalho;

typedef struct{
    int tamanho;
    char* string;
} StringVariavel;

typedef struct{
    char removido;
    int grupo;
    int popularidade;
    int peso;
    StringVariavel tecnologiaOrigem;
    StringVariavel tecnologiaDestino;
} Registro;

// Depois (?) tem uma funçao que gera um binario aleatorio para checar se criou certo
// quando o campo do registro for vazio, colocar nulo

// comando linux para comparar binario cmp bin1.bin bin2.bin

Cabecalho *criaCabecalho(){ // Inicializa o cabecalho
    
    Cabecalho *c = (Cabecalho *) malloc(sizeof(Cabecalho));
    c->status = '0';
    c->proxRRN = 0;
    c->nroTecnologias = 0;
    c->nroParesTecnologias = 0;

    return c;
}

//void gravaCabecalho(char *nomeArquivoCSV, char *nomeArquivoBIN){
    // só é executada no final
// }

Registro *criaRegistro(){ // Aloca-se memória para um registro
    Registro *r = (Registro *) malloc(sizeof(Registro));
    
    return r;
}

void gravaRegistro(Registro *r, FILE *arqBIN){ // grava o Registro criado no arquivo binário
    fwrite(&r->removido, sizeof(char), 1, arqBIN);
    fwrite(&r->grupo, sizeof(int), 1, arqBIN);
    fwrite(&r->popularidade, sizeof(int), 1, arqBIN);
    fwrite(&r->peso, sizeof(int), 1, arqBIN);
    fwrite(&r->tecnologiaOrigem.tamanho, sizeof(int), 1, arqBIN);
    fwrite(r->tecnologiaOrigem.string, sizeof(char), r->tecnologiaOrigem.tamanho, arqBIN);
    fwrite(&r->tecnologiaDestino.tamanho, sizeof(int), 1, arqBIN);
    fwrite(r->tecnologiaDestino.string, sizeof(char), r->tecnologiaDestino.tamanho, arqBIN);
}


void criaTabela(char *nomeArquivoCSV, char *nomeArquivoBIN){ // Funcionalidade 1

    FILE *arquivoCSV = fopen(nomeArquivoCSV, "r");

    if(arquivoCSV == NULL){
        printf("Falha no carregamento do Arquivo CSV.\n");
        return;
    }

    FILE *arquivoBIN = fopen(nomeArquivoBIN, "wb");

    if(arquivoBIN == NULL){
        printf("Falha no carregamento do Arquivo Binário.\n");
        return;
    }

    //Cabecalho *cabecalho = criaCabecalho();

    Registro *r = criaRegistro();

    // A lógica é criar um loop que irá ler cada linha do arquivo csv
    while (fscanf(arquivoCSV, "%[^,],%d,%d,%[^,],%d\n", r->tecnologiaOrigem.string, &r->grupo, &r->popularidade, r->tecnologiaDestino.string, &r->peso) != EOF) {

        r->tecnologiaOrigem.tamanho = (int) strlen(r->tecnologiaOrigem.string);
        r->tecnologiaDestino.tamanho = (int) strlen(r->tecnologiaDestino.string);
        printf("%d\n", r->tecnologiaOrigem.tamanho);
        printf("%d\n", r->tecnologiaDestino.tamanho);

        // Coloca os valores lidos no arquivo binário
        gravaRegistro(r, arquivoBIN);
    }

    //gravaCabecalho(nomeArquivoCSV, nomeArquivoBIN);
    
    fclose(arquivoCSV);
    fclose(arquivoBIN);

}

int main (){
   
// primeira funcao, criar tabela
    criaTabela("tecnologia.csv","tecnologia.bin");

    return 0;
}
