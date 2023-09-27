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

// Depois (?) tem uma funçao que gera um binario aleatorio para checar se criou certo
// quando o campo do registro for vazio, colocar nulo

// comando linux para comparar binario cmp bin1.bin bin2.bin

Cabecalho *criaCabecalho(void){ // Inicializa o cabecalho
    
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

Registro *criaRegistro(void){ // Aloca-se memória para um registro
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

// Funcionalidade 1
void criaTabela(char *nomeArquivoCSV, char *nomeArquivoBIN){ 

    // Abrindo o arquivo CSV
    FILE *arquivoCSV = fopen(nomeArquivoCSV, "r");
    if(arquivoCSV == NULL){
        printf("Falha no carregamento do Arquivo CSV.\n");
        return;
    }

    // Criando um arquivo binário
    FILE *arquivoBIN = fopen(nomeArquivoBIN, "wb");
    if(arquivoBIN == NULL){
        printf("Falha no carregamento do Arquivo Binário.\n");
        return;
    }

    //Cabecalho *cabecalho = criaCabecalho();
    //char a[20];
    // A primeira linha do arquivo CSV deve ser descartada (aparentemente dá certo)
    fscanf(arquivoCSV, "%*[^\n]\n");
    
    Registro *r = criaRegistro();

    // A lógica é criar um loop que irá ler cada linha do arquivo CSV
    while (fscanf(arquivoCSV, "%[^,],%d,%d,%[^,],%d\n", r->tecnologiaOrigem.string, &r->grupo, &r->popularidade, r->tecnologiaDestino.string, &r->peso) == 5) {

        // Está dando erro ao ler e alocar diretamente as strings acima.
        // Sugestão: criar variáveis auxiliares e tratar adequadamente a alocação.
        
        // r->tecnologiaOrigem.tamanho = (int) strlen(r->tecnologiaOrigem.string);
        // r->tecnologiaDestino.tamanho = (int) strlen(r->tecnologiaDestino.string);
        // printf("%d\n", r->grupo);
        // printf("%d\n", r->popularidade);
        // printf("%d\n", r->peso);

        // Coloca os valores lidos no arquivo binário
        gravaRegistro(r, arquivoBIN);
        
        // Debugando o código
        //printf("Tecnologia Origem: %s\nGrupo: %d\nPopularidade: %d\nTecnologia Destino: %s\nPeso: %d", r->tecnologiaOrigem.string,  r->grupo, r->popularidade, r->tecnologiaDestino.string, r->peso);
    }

    //gravaCabecalho(nomeArquivoCSV, nomeArquivoBIN);
    
    free(r);

    fclose(arquivoCSV);
    fclose(arquivoBIN);

}

int main (int argc, char *argv[]){
   
    // primeira funcao, criar tabela
    criaTabela("arquivos/dados1.csv","tecnologia.bin");


    return 0;
}
