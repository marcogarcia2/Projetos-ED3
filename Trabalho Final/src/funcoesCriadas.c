/*  
*   Lucas Lima Romero - 13676325
*   Marco Antonio Gaspar Garcia - 11833581
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

// Função que le cabeçalho do arquivo de dados
void leCabecalho(Cabecalho *c, FILE *arquivoBIN){

    // Voltando ao início do arquivo
    fseek(arquivoBIN, 0, SEEK_SET);

    // Lendo os valores adequadamente
    fread(&c->status, sizeof(char), 1, arquivoBIN);
    fread(&c->proxRRN, sizeof(int), 1, arquivoBIN);
    fread(&c->nroTecnologias, sizeof(int), 1, arquivoBIN);
    fread(&c->nroParesTecnologias, sizeof(int), 1, arquivoBIN);
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

// Função que concatena as strings de tecnologiaOrigem e tecnologiaDestino
char *concatenaStrings(Registro *r){
    // Concatena as strings de tecnologiaOrigem e tecnologiaDestino
    // Retorna a string concatenada
    char *stringConcatenada = (char*) malloc(r->tecnologiaOrigem.tamanho + r->tecnologiaDestino.tamanho + 1);
    strcpy(stringConcatenada, r->tecnologiaOrigem.string);
    strcat(stringConcatenada, r->tecnologiaDestino.string);

    return stringConcatenada;
}

// Função que busca uma string no arquivo de dados
void buscaString(char *nomeCampo, char *tecnologia, int tamTotal, FILE *arquivoBIN){

    Registro *r;

    // Flag que indica se o registro foi encontrado
    int flag = 0;

    // Pulando o cabeçalho
    fseek(arquivoBIN, 13, SEEK_SET);
    unsigned long byteOffset = ftell(arquivoBIN);

    while(byteOffset <= tamTotal){

        // Crio meu registro
        r = criaRegistro();

        // Trago as informações para a memória primária
        r = leRegistro(byteOffset, r, arquivoBIN);

        if(r->removido == '0'){
            if(!strcmp(nomeCampo, "nomeTecnologiaOrigem")){
                if(!strcmp(r->tecnologiaOrigem.string, tecnologia)){
                    // Encontrou o valor buscado
                    imprimeRegistro(r);
                    flag = 1;
                }
            }

            else if(!strcmp(nomeCampo, "nomeTecnologiaDestino")){
                if(!strcmp(r->tecnologiaDestino.string, tecnologia)){
                    // Encontrou o valor buscado
                    imprimeRegistro(r);
                    flag = 1;
                }
            }
        }

        // Liberando a memória alocada para o registro
        liberaRegistro(r);

        // Atualizando o byteOffset
        byteOffset += TAM_REGISTRO;
    }

    // Caso depois de rodar todo o while e não encontrar nenhum (flag = 0)
    if(!flag) printf("Registro inexistente.\n");
}

// Função que busca um inteiro no arquivo de dados
void buscaInteiro(char *nomeCampo, int valor, int tamTotal, FILE *arquivoBIN){

    Registro *r;

    // Flag que indica se o registro foi encontrado
    int flag = 0;

    // Pulando o cabeçalho
    fseek(arquivoBIN, 13, SEEK_SET);
    unsigned long byteOffset = ftell(arquivoBIN);

    while(byteOffset <= tamTotal){

        // Crio meu registro
        r = criaRegistro();

        // Trago as informações para a memória primária
        r = leRegistro(byteOffset, r, arquivoBIN);

        if (r->removido != '1'){

            if(!strcmp(nomeCampo, "grupo")){
                if(r->grupo == valor){
                    // Encontrou o valor buscado
                    imprimeRegistro(r);
                    flag = 1;
                }
            }

            else if(!strcmp(nomeCampo, "popularidade")){
                if(r->popularidade == valor){
                    // Encontrou o valor buscado
                    imprimeRegistro(r);
                    flag = 1;
                }
            }

            else if(!strcmp(nomeCampo, "peso")){
                if(r->peso == valor){
                    // Encontrou o valor buscado
                    imprimeRegistro(r);
                    flag = 1;
                }
            }
        }

        // Liberando a memória alocada para o registro
        liberaRegistro(r);
        // Atualizando o byteOffset
        byteOffset += TAM_REGISTRO;
    }

    // Caso depois de rodar todo o while e não encontrar nenhum (flag = 0)
    if(!flag) printf("Registro inexistente.\n");
}

// Essa função calcula o tamanho total e serve para dar a condição de parada na leitura do arquivo de dados
int calculaTamanhoTotal(FILE *arquivoBIN){ 
    // Para usar a função, é necessário que esteja no início do arquivo (byte 1, depois do status (byte 0))
    // No caso, a sua única utilização foi quando o byte de status do arquivo de dados foi lido, logo o ponteiro está no byte 1
    
    // Calculando o tamanho total do arquivo
    int ultimoRRN;
    fread(&ultimoRRN, sizeof(int), 1, arquivoBIN); 
    ultimoRRN--;

    // Calculando o tamanho total do arquivo
    const unsigned int tamTotal = 13 + (TAM_REGISTRO * ultimoRRN);

    return tamTotal;
}
