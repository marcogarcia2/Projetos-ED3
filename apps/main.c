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

void gravaCabecalho(Cabecalho *c, FILE *arqBIN){
    fwrite(c, sizeof(Cabecalho), 1, arqBIN);
}

Registro *criaRegistro(void){ // Aloca-se memória para um registro
    Registro *r = (Registro *) malloc(sizeof(Registro));

    r->removido = '0';
    r->grupo = -1;
    r->popularidade = -1;
    r->peso = -1;

    r->tecnologiaOrigem.string = NULL;
    r->tecnologiaOrigem.tamanho = 0; 

    r->tecnologiaDestino.string = NULL;
    r->tecnologiaDestino.tamanho = 0; 
    
    return r;
}

Registro *resetaRegistro(Registro *r){ // Será chamada para resetar os parâmetros do registro
    r->grupo = -1;
    r->popularidade = -1;
    r->peso = -1;

    r->tecnologiaOrigem.string = NULL;
    r->tecnologiaOrigem.tamanho = 0; 

    r->tecnologiaDestino.string = NULL;
    r->tecnologiaDestino.tamanho = 0;

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

    // Agora falta tratar o lixo, isto é, preencher com '$' os bytes restantes
    // tamLixo = 76 - (1 + 3 * 4 + 2 * 4 + r->tecnologiaOrigem.tamanho + r->tecnologiaDestino.tamanho)
    // size_t tamLixo = TAM_REGISTRO_FIXO - (sizeof(char) + 3*sizeof(int) + 2*sizeof(int) + r->tecnologiaOrigem.tamanho + r->tecnologiaDestino.tamanho);
    
    // for(size_t i = 0; i < tamLixo; i++){
    //     fputc(LIXO, arqBIN);
    // }


    // const char *lixo;
    // for(size_t i = 0; i < tamLixo; i++){
    //     fputc(LIXO, arqBIN);
    // }
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

    Cabecalho *cabecalho = criaCabecalho();
    // printf("Status: %c\n", cabecalho->status);
    // printf("proxRRN: %d\n", cabecalho->proxRRN);
    // printf("nroTecnologias: %d\n", cabecalho->nroTecnologias);
    // printf("nroParesTecnologias: %d\n", cabecalho->nroParesTecnologias);
    gravaCabecalho(cabecalho, arquivoBIN);

    // Descartando a primeira linha do arquivo 
    fscanf(arquivoCSV, "%*[^\n]\n");
    
    Registro *r = criaRegistro();

    char str1[30], str2[30];

    // A lógica é criar um loop que irá ler cada linha do arquivo CSV
    while (fscanf(arquivoCSV, "%[^,],%d,%d,%[^,],%d\n", str1, &r->grupo, &r->popularidade, str2, &r->peso) != EOF) {    
        
        r->tecnologiaOrigem.string = str1;
        r->tecnologiaOrigem.tamanho = strlen(str1);
        r->tecnologiaDestino.string = str2;
        r->tecnologiaDestino.tamanho = strlen(str2);
        
        printf("r->tecnologiaOrigem.string = %s\n", r->tecnologiaOrigem.string);
        printf("r->tecnologiaOrigem.tamanho = %d\n\n", r->tecnologiaOrigem.tamanho);
        printf("r->tecnologiaDestino.string = %s\n", r->tecnologiaDestino.string);
        printf("r->tecnologiaDestino.tamanho = %d\n\n", r->tecnologiaDestino.tamanho);
        

        // Escreve os valores lidos no arquivo binário
        gravaRegistro(r, arquivoBIN);
        r = resetaRegistro(r);
    }

    //gravaCabecalho(nomeArquivoCSV, nomeArquivoBIN);

    
    free(r);
    free(cabecalho);

    fclose(arquivoCSV);
    fclose(arquivoBIN);

}

int main (int argc, char *argv[]){
   
    // primeira funcao, criar tabela
    criaTabela("arquivos/dados1.csv","tecnologia.bin");
    // criaTabela("arquivos/testecamposnulos.csv","tecnologia.bin"); // Teste para campos nulos


    return 0;
}
