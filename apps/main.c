#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "funcoesFornecidas.h"

#define TAM_REGISTRO 76
#define TAM_REGISTRO_FIXO 21
#define LIXO '$'

typedef struct{ // tamanho fixo de 13 bytes? Cabelho
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

// A primeira função é criar a tabela de dados
// DEpois (?) tem uma funçao que gera um binario aleatorio para checar se criou certo
// quanndo o campo do registro for vazio, colocar nulo

// comando linux para comparar binario cmp bin1.bin bin2.bin

int main (int argc, char *argv[]){

    //Cabecalho cabecalho = {0, 0, 0, 0};
    //Registro *registros;    

    // leitura dos dados
    FILE *arq = fopen("arquivos/dados1.csv","rb");
    if (arq == NULL){
        printf("Erro ao abrir o arquivo");
        return 1;
    }
    

    fclose(arq);


    // primeira funcao, criar tabela


    return 0;
}
