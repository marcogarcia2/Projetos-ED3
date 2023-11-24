/*  
*   Lucas Lima Romero - 13676325
*   Marco Antonio Gaspar Garcia - 11833581
*/

#ifndef REGISTROS_H
#define REGISTROS_H

#define TAM_REGISTRO 76
#define TAM_REGISTRO_FIXO 21
#define LIXO "$"

// tamanho fixo de 13 bytes - Cabecalho
typedef struct{ 
    char status; // 0: arquivo inconsistente, 1: arquivo consistente
    int proxRRN; // indica o proximo RRN a ser inserido
    int nroTecnologias; // indica o nro de tecnologias unicas de origem
    int nroParesTecnologias; // indica todos os pares únicos de tecnologia (c - python) != python - c
} Cabecalho;

// Estrutura que define uma string de tamanho variavel
typedef struct{
    int tamanho;
    char *string;
} StringVariavel;

// Estrutura que define um registro, para nos auxiliar na escrita de arquivos binarios
typedef struct{
    char removido; 
    int grupo;
    int popularidade;
    int peso;
    StringVariavel tecnologiaOrigem;
    StringVariavel tecnologiaDestino;
} Registro;

// Estrutura do Cabeçalho do Arquivo de Índice
typedef struct{
    char status;
    int noRaiz;
    int RRNproxNo;
} CabecalhoIndice;


#endif
