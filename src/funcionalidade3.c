/*  
*   Lucas Lima Romero - 13676325
*   Marco Antonio Gaspar Garcia - 11833581
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "funcionalidades.h"
#include "registros.h"
#include "funcoesCriadas.h"
#include "funcoesFornecidas.h"

// Funcionalidade 3: imprime os registros associados a um dado campo e seu respectivo valor

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

// Funcionalidade 3 em si

void buscaPorCampo(char *nomearquivoBIN, int N){ 

    // Variáveis que nos auxiliarão
    char nomeCampo[30], valorCampo[30];
    int valorCampoint;

    // Abrindo o arquivo binário
    FILE *arquivoBIN = fopen(nomearquivoBIN, "rb"); // Modo de leitura em binário
    if (arquivoBIN == NULL){ // Se o arquivo não existir, erro
        printf("Falha no processamento do arquivo.\n");
        return;
    }

    // Se o arquivo está inconsistente, encerra-se a função
    if(fgetc(arquivoBIN) == '0'){
        printf("Falha no processamento do arquivo.\n");
        fclose(arquivoBIN);
        return;
    }

    // Adquirindo informações sobre o último RRN, lendo direto do cabeçalho
    int ultimoRRN;
    fread(&ultimoRRN, sizeof(int), 1, arquivoBIN); 
    ultimoRRN--;

    // Calculando o tamanho total do arquivo
    unsigned int tamTotal = 13 + (TAM_REGISTRO * ultimoRRN);

    // Loop mestre, N é o número de buscas que vem da chamada da função
    for (int i = 0; i < N; i++){

        // Pula o cabeçalho
        fseek(arquivoBIN, 13, SEEK_SET);

        // Leitura do nome do campo
        scanf("%s", nomeCampo);

        /* O tratamento será diferente para buscarmos uma string ou um inteiro */

        // Se o campo for string: nomeTecnologiaOrigem ou nomeTecnologiaDestino
        if(!strcmp(nomeCampo, "nomeTecnologiaOrigem") || !strcmp(nomeCampo, "nomeTecnologiaDestino")){
            
            // Leitura da entrada entre aspas e chamda da função de busca
            scan_quote_string(valorCampo); 
            buscaString(nomeCampo, valorCampo, tamTotal, arquivoBIN);
        }

        // Se o campo for um inteiro: grupo, popularidade ou peso
        else {

            // Leitura da entrada e chamada da função de busca
            scanf("%d", &valorCampoint);
            buscaInteiro(nomeCampo, valorCampoint, tamTotal, arquivoBIN);
        }

        // Volto para o início do arquivo
        fseek(arquivoBIN, 0, SEEK_SET); 
    }

    // Fechando o arquivo
    fclose(arquivoBIN);
}

