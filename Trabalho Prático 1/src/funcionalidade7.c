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
#include "lista.h"

// Funcionalidade 7
void insercaoNosArquivos(char *nomeArquivoBIN, char *nomeArquivoIND, int n){
    // Serão inseridos registros nessa ordem nomeTecnologiaOrigem grupo popularidade nomeTecnologiaDestino peso
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

    // Abrindo o arquivo de índices
    FILE *arquivoIND = fopen(nomeArquivoIND, "rb");
    if(arquivoIND == NULL){
        printf("Falha no processamento do arquivo.\n");
        return;
    }

    // Se o arquivo está inconsistente, encerra-se a função
    if(fgetc(arquivoIND) == '0'){
        printf("Falha no processamento do arquivo.\n");
        fclose(arquivoIND);
        return;
    }
    
    // Precisamos ler o cabecalho para saber o proxRRN 
    Cabecalho *cabecalho = criaCabecalho();
    // leCabecalho(cabecalho, arquivoBIN); // Nao temos essa funcao ainda

    // CabecalhoIndice *cabIndice = criaCabecalhoIndice();
    // leCabecalhoIndice(cabIndice, arquivoIND); // Nao temos essa funcao ainda
    
    // Variáveis que nos auxiliarão na análise do arquivo
    Lista *L = criaLista();
    Lista *ListaPares = criaLista();

    // PRECISAREMOS LER TODO O ARQUIVO DE DADOS E ARMAZENAR NESSAS DUAS LISTAS EM MEMORIA PRIMARIA
    // AQUI... nossas inserções serão no final dessa lista

    // Criando um loop que irá ler as "n" linhas do teclado
    for (int k = 0; k < n; k++) {
        // -------------------- INSERÇÃO NO ARQUIVO DE DADOS -------------------------- //
        char nomeTecnologiaOrigem[100];
        char grupo[10];
        char popularidade[10];
        char nomeTecnologiaDestino[100];
        char peso[10];

        // IMPORTANTE: PELA ESPECIFICACAO AQUI TEM QUE TER ASPAS EM ORIGEM E DESTINO MAS NO RUNCODES TA SEM (VERIFICAR DPS AO SUBMETER)
        scanf("%[^,], %[^,], %[^,], %[^,], %[^\n]", nomeTecnologiaOrigem, grupo, popularidade, nomeTecnologiaDestino, peso);

        // Tratando os campos
        Registro *r = criaRegistro();

        // Guardando o campo nomeTecnologiaOrigem
        r->tecnologiaOrigem.tamanho = strlen(nomeTecnologiaOrigem);
        r->tecnologiaOrigem.string = malloc(r->tecnologiaOrigem.tamanho + 1);
        strcpy(r->tecnologiaOrigem.string, nomeTecnologiaOrigem);

        // Guardando o campo grupo
        r->grupo = strcmp(grupo, "NULO") == 0 ? -1 : atoi(grupo);

        // Guardando o campo popularidade
        r->popularidade = strcmp(popularidade, "NULO") == 0 ? -1 : atoi(popularidade);

        // Guardando o campo nomeTecnologiaDestino
        r->tecnologiaDestino.tamanho = strlen(nomeTecnologiaDestino);
        r->tecnologiaDestino.string = malloc(r->tecnologiaDestino.tamanho + 1);
        strcpy(r->tecnologiaDestino.string, nomeTecnologiaDestino);

        // Guardando o campo peso
        r->peso = strcmp(peso, "NULO") == 0 ? -1 : atoi(peso);

        // Testando se a leitura e o armazenamento estao corretos:
        // printf("nomeTecnologiaOrigem: %s\n", r->tecnologiaOrigem.string);
        // printf("grupo: %d\n", r->grupo);
        // printf("popularidade: %d\n", r->popularidade);
        // printf("nomeTecnologiaDestino: %s\n", r->tecnologiaDestino.string);
        // printf("peso: %d\n", r->peso);
        //imprimeRegistro(r);

        // Logica da insercao
        // insercaoArquivoDados(); // NESSA FUNCAO PODEMOS ATUALIZAR JA O CABECALHO CONFORME ABAIXO
        //cabecalho->proxRRN++;
        /*
            // Atualizamos o nroTecnologias
            cabecalho->nroTecnologias = getTamanho(L);

            // Atualizamos o nroParesTecnologias
            cabecalho->nroParesTecnologias = getTamanho(ListaPares);
        */
        // adicionaLista(L, r->tecnologiaOrigem.string, r->tecnologiaOrigem.tamanho);        
        // adicionaLista(L, r->tecnologiaDestino.string, r->tecnologiaDestino.tamanho);

        if(strcmp(r->tecnologiaOrigem.string, "") != 0 && strcmp(r->tecnologiaDestino.string, "") != 0){
            char *stringConcatenada = (char*) malloc(r->tecnologiaOrigem.tamanho + r->tecnologiaDestino.tamanho + 1);
            strcpy(stringConcatenada, r->tecnologiaOrigem.string);
            strcat(stringConcatenada, r->tecnologiaDestino.string);
            adicionaLista(ListaPares, stringConcatenada, r->tecnologiaOrigem.tamanho + r->tecnologiaDestino.tamanho);
        }
        // ------------------------------------------------------------------------------ //

        // -------------------- INSERÇÃO NO ARQUIVO DE INDICES -------------------------- //
        // MODULARIZAR AS DUAS INSERCOES A SEREM FEITAS

        // Inserindo no arquivo de índices
        // insercaoArquivoIndices(); // NESSA FUNCAO PODEMOS ATUALIZAR JA O CABECALHO

        // ------------------------------------------------------------------------------ //

        // Liberando a memória do registro
        liberaRegistro(r);
    }

    // Alterando o status para '1' antes de fechar o binário
    cabecalho->status = '1';

    // Finalmente, gravando o cabeçalho no início do arquivo
    gravaCabecalho(cabecalho, arquivoBIN);

    // Fazemos o mesmo com o arquivo de índices
    // Alterando o status para '1' antes de fechar o binário
    // cabIndice->status = '1';

    // Finalmente, gravando o cabeçalho no início do arquivo
    // gravaCabecalhoIndice(cabIndice, arquivoIND)

    // Liberando a memória do cabecalho e das listas encadeadas
    free(cabecalho);
    // free(cabIndice);
    destroiLista(&L);
    destroiLista(&ListaPares);

    binarioNaTela(nomeArquivoBIN);
    binarioNaTela(nomeArquivoIND);
}