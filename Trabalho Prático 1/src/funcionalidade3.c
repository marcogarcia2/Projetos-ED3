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

// Funcionalidade 3 em si

void buscaPorCampo(char *nomeArquivoBIN, int N){ 

    // Variáveis que nos auxiliarão
    char nomeCampo[30], valorCampo[30];
    int valorCampoint;

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

