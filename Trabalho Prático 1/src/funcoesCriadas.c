/*  
*   Lucas Lima Romero - 13676325
*   Marco Antonio Gaspar Garcia - 11833581
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

char *concatenaStrings(Registro *r){
    // Concatena as strings de tecnologiaOrigem e tecnologiaDestino
    // Retorna a string concatenada
    char *stringConcatenada = (char*) malloc(r->tecnologiaOrigem.tamanho + r->tecnologiaDestino.tamanho + 1);
    strcpy(stringConcatenada, r->tecnologiaOrigem.string);
    strcat(stringConcatenada, r->tecnologiaDestino.string);

    return stringConcatenada;
}

void buscaString2(char *nomeCampo, char *tecnologia, int tamTotal, FILE *arquivoBIN){

    // Guarda o byte inicial
    Registro *r;
    int flag = 0;
    unsigned long byteOffset = ftell(arquivoBIN);

    while(byteOffset < tamTotal){
        if(fgetc(arquivoBIN) == '0'){ // Se não estiver removido

            // Crio meu registro
            r = criaRegistro();

            // Trago as informações para a memória primária
            r = leRegistro(byteOffset, r, arquivoBIN);

            if(!strcmp(nomeCampo, "nomeTecnologiaOrigem")){
                if(!strcmp(r->tecnologiaOrigem.string, tecnologia)){
                    imprimeRegistro(r);
                    flag = 1;
                }
            }

            else if(!strcmp(nomeCampo, "nomeTecnologiaDestino")){
                if(!strcmp(r->tecnologiaDestino.string, tecnologia)){
                    imprimeRegistro(r);
                    flag = 1;
                }
            }

            liberaRegistro(r);
            byteOffset += TAM_REGISTRO;
        }
    }

    // Caso depois de rodar todo o while e não encontrar nenhum (flag = 0)
    if(!flag) printf("Registro inexistente.\n");
}

void buscaInteiro2(char *nomeCampo, int valor, int tamTotal, FILE *arquivoBIN){

    // Guarda o byte inicial
    Registro *r;
    int flag = 0;
    unsigned long byteOffset = ftell(arquivoBIN);

    while(byteOffset < tamTotal){
        if(fgetc(arquivoBIN) == '0'){ // Se não estiver removido

            // Crio meu registro
            r = criaRegistro();

            // Trago as informações para a memória primária
            r = leRegistro(byteOffset, r, arquivoBIN);

            if(!strcmp(nomeCampo, "grupo")){
                if(r->grupo == valor){
                    imprimeRegistro(r);
                    flag = 1;
                }
            }

            else if(!strcmp(nomeCampo, "popularidade")){
                if(r->popularidade == valor){
                    imprimeRegistro(r);
                    flag = 1;
                }
            }

            else if(!strcmp(nomeCampo, "peso")){
                if(r->peso == valor){
                    imprimeRegistro(r);
                    flag = 1;
                }
            }

            liberaRegistro(r);
            byteOffset += TAM_REGISTRO;
        }
    }

    // Caso depois de rodar todo o while e não encontrar nenhum (flag = 0)
    if(!flag) printf("Registro inexistente.\n");
}

void buscaString(char *nomeCampo, char *tecnologia, int tamTotal, FILE *arquivoBIN){

    // Variáveis que nos auxiliarão na análise
    int flag = 0;
    int jump = 0;

    // Guarda em que byte começa o registro que está sendo analisado, primeira vez = 13
    unsigned long byteOffset = ftell(arquivoBIN); 
    
    // Busca Sequencial no arquivo binário
    while(byteOffset < tamTotal){ 

        // Só será lido se o registro não foi removido
        if(fgetc(arquivoBIN) == '0'){
            
            // Pulando os campos grupo, popularidade e peso (3 ints)
            fseek(arquivoBIN, 3 * sizeof(int), SEEK_CUR);

            // Caso 1: nomeTecnologiaOrigem
            // Não saltamos nada

            // Caso 2: nomeTecnologiaDestino
            // Precisamos saltar alguns bytes
            if (!strcmp(nomeCampo, "nomeTecnologiaDestino")){

                // Calculando o tamanho do salto
                fread(&jump, sizeof(int), 1, arquivoBIN);
                fseek(arquivoBIN, jump, SEEK_CUR);
            }

            // Leio o tamanho da string e em seguida a própria string
            unsigned int tamanhoString;
            fread(&tamanhoString, sizeof(int), 1, arquivoBIN);
            char stringEncontrada[tamanhoString + 1];
            fread(stringEncontrada, tamanhoString, 1, arquivoBIN);
            stringEncontrada[tamanhoString] = '\0';
            
            // Comparando a stringEncontrada com a que estamos buscando
            if(!strcmp(stringEncontrada, tecnologia)){

                // Registro Encontrado! Agora iremos salvá-lo e imprimí-lo
                Registro *r = criaRegistro();
                r = leRegistro(byteOffset, r, arquivoBIN);
                imprimeRegistro(r);
                liberaRegistro(r);
                flag = 1;
            }
            
            // Se não encontrou, fseek para o próximo registro
            // if (jump == 0)
            else if (!strcmp(nomeCampo, "nomeTecnologiaOrigem")){
                fseek(arquivoBIN, TAM_REGISTRO - (17 + tamanhoString), SEEK_CUR);
            }
            
            // if (jump > 0)
            else if (!strcmp(nomeCampo, "nomeTecnologiaDestino")){
                fseek(arquivoBIN, TAM_REGISTRO - (21 + tamanhoString + jump), SEEK_CUR);
            }

            byteOffset += TAM_REGISTRO;
        }
    }

    // Caso depois de rodar todo o while e não encontrar nenhum (flag = 0)
    if(!flag) printf("Registro inexistente.\n");
}

void buscaInteiro(char *nomeCampo, int valor, int tamTotal, FILE *arquivoBIN){

    // Flag que nos auxiliará na análise
    int flag = 0;

    // Guarda em que byte começa o registro que está sendo analisado, primeira vez = 13
    unsigned long byteOffset = ftell(arquivoBIN); 
    
    // Busca Sequencial no arquivo binário
    while(byteOffset < tamTotal){
    
        // Só será lido se o registro não foi removido
        if(fgetc(arquivoBIN) == '0'){
        
            // Caso 3: grupo
            // Não saltamos nada

            // Caso 4: popularidade
            // Precisamos saltar 4 bytes (grupo)
            if (!strcmp(nomeCampo, "popularidade")) fseek(arquivoBIN, sizeof(int), SEEK_CUR);

            // Caso 5: peso
            // Precisamos saltar 8 bytes (grupo + popularidade)
            else if (!strcmp(nomeCampo, "peso")) fseek(arquivoBIN, 2 * sizeof(int), SEEK_CUR);

            // Agora estamos no byte certo a ser lido
            int inteiroEncontrado;
            fread(&inteiroEncontrado, sizeof(int), 1, arquivoBIN);

            // Comparando o inteiroEncontrado com o que estamos buscando
            if (inteiroEncontrado == valor){

                // Registro Encontrado! Agora iremos salvá-lo e imprimí-lo
                Registro *r = criaRegistro();
                r = leRegistro(byteOffset, r, arquivoBIN);
                imprimeRegistro(r);
                liberaRegistro(r);
                flag = 1;
            }

            // Se não encontrou, fseek para o próximo registro
            else if (!strcmp(nomeCampo, "grupo")){
                fseek(arquivoBIN, TAM_REGISTRO - 5, SEEK_CUR);
            }
            
            else if (!strcmp(nomeCampo, "popularidade")){
                fseek(arquivoBIN, TAM_REGISTRO - 9, SEEK_CUR);
            }

            else if (!strcmp(nomeCampo, "peso")){
                fseek(arquivoBIN, TAM_REGISTRO - 13, SEEK_CUR);
            }

            byteOffset += TAM_REGISTRO;
        
        }
    }

    // Caso depois de rodar todo o while e não encontrar nenhum (flag = 0)
    if(!flag) printf("Registro inexistente.\n");
}
