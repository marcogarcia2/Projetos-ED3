#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "funcoesFornecidas.h"
#include "registros.h"

#define TAM_REGISTRO 76
#define TAM_REGISTRO_FIXO 21
#define LIXO "$"

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

// Gravando o cabecalho em 13 bytes
void gravaCabecalho(Cabecalho *c, FILE *arquivoBIN){
    fwrite(&c->status, sizeof(char), 1, arquivoBIN);
    fwrite(&c->proxRRN, sizeof(int), 1, arquivoBIN);
    fwrite(&c->nroTecnologias, sizeof(int), 1, arquivoBIN);
    fwrite(&c->nroParesTecnologias, sizeof(int), 1, arquivoBIN);
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

void gravaRegistro(Registro *r, FILE *arquivoBIN){ // grava o Registro criado no arquivo binário
    fwrite(&r->removido, sizeof(char), 1, arquivoBIN);
    fwrite(&r->grupo, sizeof(int), 1, arquivoBIN);
    fwrite(&r->popularidade, sizeof(int), 1, arquivoBIN);
    fwrite(&r->peso, sizeof(int), 1, arquivoBIN);
    fwrite(&r->tecnologiaOrigem.tamanho, sizeof(int), 1, arquivoBIN);
    fwrite(r->tecnologiaOrigem.string, r->tecnologiaOrigem.tamanho, 1, arquivoBIN);
    fwrite(&r->tecnologiaDestino.tamanho, sizeof(int), 1, arquivoBIN);
    fwrite(r->tecnologiaDestino.string, r->tecnologiaDestino.tamanho, 1, arquivoBIN);

    //Colocando o lixo nos caracteres restantes
    // char *lixo = (char*)'$';
    // int restante = TAM_REGISTRO - (TAM_REGISTRO_FIXO + r->tecnologiaOrigem.tamanho + r->tecnologiaDestino.tamanho);
    // while (restante > 0){
    //     fwrite(&lixo, sizeof(char), 1, arquivoBIN);
    //     restante--;
    // }

    int tamLixo = TAM_REGISTRO - (TAM_REGISTRO_FIXO + r->tecnologiaOrigem.tamanho + r->tecnologiaDestino.tamanho);
    for(int i = 0; i < tamLixo; i++)
        fwrite("$", sizeof(char), 1, arquivoBIN);

}

// Funcionalidade 1
void criaTabela(const char *nomeArquivoCSV, const char *nomeArquivoBIN){ 

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
    // Reservando os 13 primeiros bytes do arquivo para o cabecalho
    gravaCabecalho(cabecalho, arquivoBIN);

    // Descartando a primeira linha do arquivo 
    fscanf(arquivoCSV, "%*[^\n]\n");
    
    Registro *r = criaRegistro();

    char str1[30], str2[30];

    // A lógica é criar um loop que irá ler cada linha do arquivo CSV
    while (fscanf(arquivoCSV, "%[^,],%d,%d,%[^,],%d\n", str1, &r->grupo, &r->popularidade, str2, &r->peso) != EOF) {    
        //cabecalho->proxRRN++;

        r->tecnologiaOrigem.string = str1;
        r->tecnologiaOrigem.tamanho = strlen(str1);
        // buscar str1 em tecnologias, caabecalho->nroTecnologias++;

        r->tecnologiaDestino.string = str2;
        r->tecnologiaDestino.tamanho = strlen(str2);
        
        /*
        printf("r->tecnologiaOrigem.string = %s\n", r->tecnologiaOrigem.string);
        printf("r->tecnologiaOrigem.tamanho = %d\n\n", r->tecnologiaOrigem.tamanho);
        printf("r->tecnologiaDestino.string = %s\n", r->tecnologiaDestino.string);
        printf("r->tecnologiaDestino.tamanho = %d\n\n", r->tecnologiaDestino.tamanho);
        */

        // Escreve os valores lidos no arquivo binário
        gravaRegistro(r, arquivoBIN);
        r = resetaRegistro(r);
    }


    // gravaCabecalho(cabecalho, arquivoBIN);

    


    fclose(arquivoCSV);
    // fseek para voltar ao inicio e gravar o cabecalho

    // 1) Alterando o status para '1' antes de fechar o binário (criar uma função pra isso, fazendo as demais atualizaçãoes necessárias)
    fseek(arquivoBIN, 0, SEEK_SET); // Ponteiro aponta para o inicio do arquivo
    fwrite("1", sizeof(char), 1, arquivoBIN);

    fclose(arquivoBIN);

    free(r);
    free(cabecalho);

    //binarionatela()

}

// Funcionalidade 2: imprimir todo o arquivo binário na tela
void recuperaDados(const char* nomeArquivoBIN){

    FILE *arquivoBIN = fopen(nomeArquivoBIN, "rb");
    if (arquivoBIN == NULL) {
        printf("Falha no carregamento do Arquivo Binário.");
        return;
    }

    // Pular os bytes do cabecalho
    char aux[13];
    fread(aux, 1, 13, arquivoBIN);

    /*
    preciso printar nessa ordem:
    nomeTecnologiaOrigem,grupo,popularidade,nomeTecnologiaDestino,peso

    ordem do registro:
    removido: 1
    grupo: 4
    popularidade: 4
    peso: 4
    tecnologiaOrigem.tamanho: 4
    tecnologiaOrigem.string: tecnologiaOrigem.tamanho
    tecnologiaDestino.tamanho:4
    tecnologiaDestino.string: tecnologiaDestino.tamanho
    */

    // Lendo os campos do registro para recuperá-lo
    while(1){
        Registro *r = criaRegistro();
        
        // o byte do campo removido é o primeiro a ser lido, servirá de flag para saber se chegou ao fim do arquivo
        int byte = fread(&r->removido, sizeof(char), 1, arquivoBIN);

        if(byte == 0){
            free(r);
            break;
        }

        // se este não é o fim do arquivo, leremos todos os campos

        fread(&r->grupo, sizeof(int), 1, arquivoBIN);
        fread(&r->popularidade, sizeof(int), 1, arquivoBIN);
        fread(&r->peso, sizeof(int), 1, arquivoBIN);

        // para ler as strings, é necessário alocar memória 

        fread(&r->tecnologiaOrigem.tamanho, sizeof(int), 1, arquivoBIN);
        r->tecnologiaOrigem.string = (char *)malloc(r->tecnologiaOrigem.tamanho);
        fread(r->tecnologiaOrigem.string, r->tecnologiaOrigem.tamanho, 1, arquivoBIN);

        fread(&r->tecnologiaDestino.tamanho, sizeof(int), 1, arquivoBIN);
        r->tecnologiaDestino.string = (char *)malloc(r->tecnologiaDestino.tamanho);
        fread(r->tecnologiaDestino.string, r->tecnologiaDestino.tamanho, 1, arquivoBIN);

        printf("%s, %d, %d, %s, %d\n", r->tecnologiaOrigem.string, r->grupo, r->popularidade, r->tecnologiaDestino.string, r->peso);
        
        // Precisamos saltar até o próximo registro, calculando o quanto de lixo ainda resta
        int restante = TAM_REGISTRO - (TAM_REGISTRO_FIXO + r->tecnologiaOrigem.tamanho + r->tecnologiaDestino.tamanho);
        char lixos[restante];
        fread(lixos, 1, restante, arquivoBIN);
    
        free(r->tecnologiaOrigem.string);
        free(r->tecnologiaDestino.string);
        free(r);
    }

    fclose(arquivoBIN);
}

int main (int argc, char *argv[]){
    
    int f;
    char nomeArquivoCSV[30], nomeArquivoBIN[30];
    scanf("%d ", &f);

    switch (f){

    case 1:
        scanf("%s %s", nomeArquivoCSV, nomeArquivoBIN);
        criaTabela(nomeArquivoCSV, nomeArquivoBIN);
        break;
    
    case 2:
        scanf("%s", nomeArquivoBIN);
        //printf("Arquivo BIN: %s\n", nomeArquivoBIN); 
        //if (strcmp("binario3.bin", nomeArquivoBIN) == 0) printf("Sucesso\n");
        //else printf("NÃO\n");
        recuperaDados(nomeArquivoBIN);
        break;

    case 3:


        break;


    case 4:

        break;


    default:
        break;
    }
    



    return 0;
}
