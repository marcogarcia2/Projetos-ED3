#ifndef REGISTROS_H
#define REGISTROS_H

typedef struct{ // tamanho fixo de 13 bytes - Cabecalho
    char status; // 0: arquivo inconsistente, 1: arquivo consistente
    int proxRRN; // indica o proximo RRN a ser inserido
    int nroTecnologias; // indica o nro de tecnologias unicas de origem
    int nroParesTecnologias; // indica todos os pares únicos de tecnologia (c - python) != python - c
} Cabecalho;

typedef struct{
    int tamanho;
    char *string;
} StringVariavel;

typedef struct{
    char removido;
    int grupo;
    int popularidade;
    int peso;
    StringVariavel tecnologiaOrigem;
    StringVariavel tecnologiaDestino;
} Registro;

#endif