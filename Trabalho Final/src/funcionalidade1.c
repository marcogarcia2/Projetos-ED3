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
#include "lista.h"

// Funcionalidade 1: criar um arquivo binário a partir de um arquivo CSV

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

    // Criando e reservando os 13 primeiros bytes ao cabeçalho
    Cabecalho *cabecalho = criaCabecalho();
    gravaCabecalho(cabecalho, arquivoBIN);

    // Descartando a primeira linha do arquivo CSV
    fscanf(arquivoCSV, "%*[^\n]\n");
    
    // Variáveis que nos auxiliarão na análise do arquivo
    char linha[100];
    Lista *L = criaLista();
    Lista *ListaPares = criaLista();

    // Criando um loop que irá ler cada linha do arquivo CSV
    while (fgets(linha, sizeof(linha), arquivoCSV)) { 

        // A variável linha contém todas as informações que precisamos, separadas por ','
        
        // Criando um registro para guardar as informações desta linha
        Registro *r = criaRegistro();

        int i = 0; // Variável que percorre a linha

        // Serão cinco iterações, uma para ler cada campo
        for (int iter = 1; iter <= 5; iter++){

            int j = 0;            // Variável que percorre o campo
            char aux[100] = "";   // Variável que guardará os campos

            // Tratando os 4 primeiros campos
            if (iter != 5) {

                // Percorrendo a linha e atribuindo em aux
                while (linha[i] != ','){
                    aux[j++] = linha[i++];
                }
                i++; // Pula a vírgula

                if (iter == 1){

                    // Guardando o campo nomeTecnologiaOrigem
                    r->tecnologiaOrigem.tamanho = strlen(aux);
                    r->tecnologiaOrigem.string = (char*)malloc(r->tecnologiaOrigem.tamanho + 1);
                    strcpy(r->tecnologiaOrigem.string, aux);
                    r->tecnologiaOrigem.string[r->tecnologiaOrigem.tamanho] = '\0';
                }

                else if (iter == 2) {

                    // Guardando o campo grupo
                    r->grupo = aux[0] == '\0' ? -1 : atoi(aux);
                }

                else if (iter == 3) {

                    // Guardando o campo popularidade
                    r->popularidade = aux[0] == '\0' ? -1 : atoi(aux);
                    //printf("Popularidade = %d\n", r->popularidade);
                }

                else if (iter == 4) {

                    // Guardando o campo nomeTecnologiaDestino
                    r->tecnologiaDestino.tamanho = strlen(aux);
                    r->tecnologiaDestino.string = (char*)malloc(r->tecnologiaDestino.tamanho + 1);
                    strcpy(r->tecnologiaDestino.string, aux);
                    r->tecnologiaDestino.string[r->tecnologiaDestino.tamanho] = '\0';
                }
            }

            // iter == 5, deve ser tratada de forma diferente pois não termina em ','
            else {

                // Percorrendo o restante da linha
                while(linha[i+1] != '\n' && linha[i] != '\0'){
                    aux[j++] = linha[i++];
                }

                // Guardando o campo peso
                r->peso = strlen(aux) == 0 ? -1 : atoi(aux);
            }
        }

        gravaRegistro(r, arquivoBIN);
        cabecalho->proxRRN++;
        adicionaLista(L, r->tecnologiaOrigem.string, r->tecnologiaOrigem.tamanho);        
        adicionaLista(L, r->tecnologiaDestino.string, r->tecnologiaDestino.tamanho);

        if(strcmp(r->tecnologiaOrigem.string, "") != 0 && strcmp(r->tecnologiaDestino.string, "") != 0){
            char *stringConcatenada = (char*) malloc(r->tecnologiaOrigem.tamanho + r->tecnologiaDestino.tamanho + 1);
            strcpy(stringConcatenada, r->tecnologiaOrigem.string);
            strcat(stringConcatenada, r->tecnologiaDestino.string);
            adicionaLista(ListaPares, stringConcatenada, r->tecnologiaOrigem.tamanho + r->tecnologiaDestino.tamanho);
        }

        // Liberando a memória do registro
        liberaRegistro(r);
    }

    // nroTecnologias é == o tamanho da lista L
    cabecalho->nroTecnologias = getTamanho(L);

    // nroParesTecnologias é == o tamanho da ListaPares
    cabecalho->nroParesTecnologias = getTamanho(ListaPares);

    // 1) Alterando o status para '1' antes de fechar o binário
    cabecalho->status = '1';

    // Finalmente, gravando o cabeçalho no início do arquivo
    gravaCabecalho(cabecalho, arquivoBIN);
    
    // Fechando os dois arquivos
    fclose(arquivoCSV);
    fclose(arquivoBIN);

    // Liberando a memória do cabecalho e das listas encadeadas
    free(cabecalho);
    destroiLista(&L);
    destroiLista(&ListaPares);

    binarioNaTela(nomeArquivoBIN);
}
