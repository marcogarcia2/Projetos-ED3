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
#include "funcoesIndice.h"
#include "arvoreB.h"

// Funcionalidade 6

/*

- A primeira coisa que preciso fazer eh:
. Ler o que queremos procurar, se eh:
    - tecnologia de origem, destino (func 3 - busca string)
    - chave de busca (concatenacao origem destino) (func 6) + (func 4)
    - grupo (int) - chamo a (func 3 - busca inteiro)
    - popularidade (int) - chamo a (func 3 - busca inteiro)
    - peso (int) - chamo a (func 3 - busca inteiro)

. Abrir o arquivo de dados 
. Checar sua consistencia
. Abrir arquivo de indices (so preciso abrir se o anterior for valido)

*/

int buscaPelaChave(char *nomeChave, FILE *arquivoIND){ // Essa funcao retorna o Pr da chave buscada
    int rrnBuscado = -1;

    // Logica de busca do RRN

    return rrnBuscado;
}   

void buscaComIndice(char *nomeArquivoBIN, char *nomeArquivoIND, int n){

    // Abrindo o arquivo binário
    FILE *arquivoBIN = fopen(nomeArquivoBIN, "rb"); // Modo de leitura em binário
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

    FILE *arquivoIND = fopen(nomeArquivoIND, "rb");
    if(arquivoIND == NULL){
        printf("Falha no processamento do arquivo.\n");
        return;
    }

    // Preciso checar a consistencia tambem? Vou checar por via das duvidas
    // Se o arquivo está inconsistente, encerra-se a função
    if(fgetc(arquivoIND) == '0'){
        printf("Falha no processamento do arquivo.\n");
        fclose(arquivoIND);
        return;
    }

    char nomeCampo[30], valorCampo[30];
    int valorCampoint;
    int rrnBuscado;

    int ultimoRRN;
    fread(&ultimoRRN, sizeof(int), 1, arquivoBIN); 
    ultimoRRN--;

    // Calculando o tamanho total do arquivo
    unsigned int tamTotal = 13 + (TAM_REGISTRO * ultimoRRN);

    for(int i = 0; i < n; i++){
        // Leitura do nome do campo
        scanf("%s", nomeCampo);

        /* O tratamento será diferente para buscarmos uma string ou um inteiro */

        // Se o campo for string: nomeTecnologiaOrigem ou nomeTecnologiaDestino
        if(!strcmp(nomeCampo, "nomeTecnologiaOrigem") || !strcmp(nomeCampo, "nomeTecnologiaDestino")){
            
            // Leitura da entrada entre aspas e chamada da função de busca
            scan_quote_string(valorCampo); 
            buscaString(nomeCampo, valorCampo, tamTotal, arquivoBIN);
        }

        // Se o campo for a chave de busca
        else if(!strcmp(nomeCampo, "nomeTecnologiaOrigemDestino ")){

            // Leitura da entrada entre aspas e chamada da função de busca
            scan_quote_string(valorCampo);

            // Guardo o valor do RRN encontrado
            rrnBuscado = buscaPelaChave(valorCampo, arquivoIND);

            // Realizo a busca a partir da funcionalidade 4
            if(rrnBuscado != -1){
                buscaPorRRN(nomeArquivoBIN, rrnBuscado);
            }
        }

        // Se o campo for um inteiro: grupo, popularidade ou peso
        else {

            // Leitura da entrada e chamada da função de busca
            scanf("%d", &valorCampoint);
            buscaInteiro(nomeCampo, valorCampoint, tamTotal, arquivoBIN);
        }
    }
}