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

// Função que lê o arquivo dados e guarda o número de tecnologias e pares de tecnologias
void contagemTecnologiasEPares(Lista *ListaNroTecnologias, Lista *ListaPares, FILE *arquivoBIN){

    // Pula os 13 primeiros bytes do arquivo, referentes ao cabeçalho
    int byteOffset = 13;
    fseek(arquivoBIN, byteOffset, SEEK_SET);

    // Lendo todos os registros e colocando os campos de tecnologiaOrigem e tecnologiaDestino nas listas de número e pares de tecnologias
    while(1){

        // Cria e lê um registro
        Registro *r = criaRegistro();
        r = leRegistro(byteOffset, r, arquivoBIN);

        // Se acabaram os registros, encerra o ciclo, senão imprime-o na tela
        if (r == NULL) {
            free(r);
            break;
        }

        else {
            if (r->removido == '0'){

                if (r->tecnologiaOrigem.tamanho != 0)
                    adicionaLista(ListaNroTecnologias, r->tecnologiaOrigem.string, r->tecnologiaOrigem.tamanho);
               
                if (r->tecnologiaDestino.tamanho != 0)
                    adicionaLista(ListaNroTecnologias, r->tecnologiaDestino.string, r->tecnologiaDestino.tamanho);

                if(r->tecnologiaOrigem.tamanho != 0 && r->tecnologiaDestino.tamanho != 0){
                    char *stringConcatenada = concatenaStrings(r); //TESTAR
                    adicionaLista(ListaPares, stringConcatenada, r->tecnologiaOrigem.tamanho + r->tecnologiaDestino.tamanho); // Se precisar colocar um +1
                    free(stringConcatenada);
                }
            }
        }

        // Desalocando a memória
        liberaRegistro(r);

        // Precisamos saltar até o próximo registro
        byteOffset += TAM_REGISTRO;
    }

    // imprimeLista(ListaNroTecnologias);
    // imprimeLista(ListaPares);

    // printf("Numero de tecnologias: %d\n", getTamanho(ListaNroTecnologias));
    // printf("Numero de pares de tecnologias: %d\n", getTamanho(ListaPares));

}

// Funcionalidade 7
void insercaoNosArquivos(char *nomeArquivoBIN, char *nomeArquivoIND, int n){
    // Serão inseridos registros nessa ordem nomeTecnologiaOrigem grupo popularidade nomeTecnologiaDestino peso
    // Abrindo o arquivo binário
    FILE *arquivoBIN = fopen(nomeArquivoBIN, "rb+"); // Modo de leitura em binário
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
    FILE *arquivoIND = fopen(nomeArquivoIND, "rb+");
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
    leCabecalho(cabecalho, arquivoBIN); // Nao temos essa funcao ainda

    CabecalhoIndice *cabIndice = criaCabecalhoIndice();
    leCabecalhoIndice(cabIndice, arquivoIND); // Nao temos essa funcao ainda
    printf("Raiz: %d\nRRNproxNO: %d\n", cabIndice->noRaiz, cabIndice->RRNproxNo);
    
    // Variáveis que nos auxiliarão na análise do arquivo
    Lista *L = criaLista();
    Lista *ListaPares = criaLista();

    // PRECISAREMOS LER TODO O ARQUIVO DE DADOS E ARMAZENAR NESSAS DUAS LISTAS EM MEMORIA PRIMARIA
    // AQUI... nossas inserções serão no final dessa lista
    contagemTecnologiasEPares(L, ListaPares, arquivoBIN);


    // Criando um loop que irá ler as "n" linhas do teclado
    for (int k = 0; k < n; k++) {
        // -------------------- INSERÇÃO NO ARQUIVO DE DADOS -------------------------- //
        char nomeTecnologiaOrigem[100];
        char grupo[10];
        char popularidade[10];
        char nomeTecnologiaDestino[100];
        char peso[10];

        // IMPORTANTE: PELA ESPECIFICACAO AQUI TEM QUE TER ASPAS EM ORIGEM E DESTINO MAS NO RUNCODES TA SEM (VERIFICAR DPS AO SUBMETER)
        scanf("\n%[^,], %[^,], %[^,], %[^,], %[^\n]", nomeTecnologiaOrigem, grupo, popularidade, nomeTecnologiaDestino, peso);
        // printf("nomeTecnologiaOrigem: %s\n", nomeTecnologiaOrigem);
        // printf("grupo: %s\n", grupo);
        // printf("popularidade: %s\n", popularidade);
        // printf("nomeTecnologiaDestino: %s\n", nomeTecnologiaDestino);
        // printf("peso: %s\n", peso);

        // Tratando os campos

        Registro *r = criaRegistro();

        r->removido = '0';

        // Guardando o campo nomeTecnologiaOrigem
        if(!strcmp(nomeTecnologiaOrigem, "NULO")) {
            r->tecnologiaOrigem.tamanho = 0;
            r->tecnologiaOrigem.string = NULL;
        } else {
            r->tecnologiaOrigem.tamanho = strlen(nomeTecnologiaOrigem);
            r->tecnologiaOrigem.string = malloc(r->tecnologiaOrigem.tamanho + 1);
            strcpy(r->tecnologiaOrigem.string, nomeTecnologiaOrigem);
            r->tecnologiaOrigem.string[r->tecnologiaOrigem.tamanho] = '\0';
        }

        // Guardando o campo grupo
        r->grupo = strcmp(grupo, "NULO") == 0 ? -1 : atoi(grupo);

        // Guardando o campo popularidade
        r->popularidade = strcmp(popularidade, "NULO") == 0 ? -1 : atoi(popularidade);

        // Guardando o campo nomeTecnologiaDestino
        if(!strcmp(nomeTecnologiaDestino, "NULO")) {
            r->tecnologiaDestino.tamanho = 0;
            r->tecnologiaDestino.string = NULL;
        }
        else{
            r->tecnologiaDestino.tamanho = strlen(nomeTecnologiaDestino);
            r->tecnologiaDestino.string = malloc(r->tecnologiaDestino.tamanho + 1);
            strcpy(r->tecnologiaDestino.string, nomeTecnologiaDestino);
            r->tecnologiaDestino.string[r->tecnologiaDestino.tamanho] = '\0';
        }

        // Guardando o campo peso
        r->peso = strcmp(peso, "NULO") == 0 ? -1 : atoi(peso);

        // Testando se a leitura e o armazenamento estao corretos:
        //imprimeRegistro(r);

        // Logica da insercao
        if(k == 0)fseek(arquivoBIN, 0, SEEK_END); // Vai para o final do arquivo
        gravaRegistro(r, arquivoBIN); // NESSA FUNCAO PODEMOS ATUALIZAR JA O CABECALHO CONFORME ABAIXO

        /*
            // Atualizamos o nroTecnologias
            cabecalho->nroTecnologias = getTamanho(L);

            // Atualizamos o nroParesTecnologias
            cabecalho->nroParesTecnologias = getTamanho(ListaPares);
        */

        // Adicionando as tecnologias nas listas
        if(r->tecnologiaOrigem.tamanho != 0){
            adicionaLista(L, r->tecnologiaOrigem.string, r->tecnologiaOrigem.tamanho);        
            printf("String added: %s\n", r->tecnologiaOrigem.string);
        }

        if(r->tecnologiaDestino.tamanho != 0){
            adicionaLista(L, r->tecnologiaDestino.string, r->tecnologiaDestino.tamanho);
            printf("String added: %s\n", r->tecnologiaDestino.string);
        }

        if(r->tecnologiaOrigem.tamanho != 0 && r->tecnologiaDestino.tamanho != 0){
            char *stringConcatenada = concatenaStrings(r);
            adicionaLista(ListaPares, stringConcatenada, r->tecnologiaOrigem.tamanho + r->tecnologiaDestino.tamanho);
            
            DadosChave *dados = criaDadosChave();

            dados->chave = (char*)malloc(sizeof(stringConcatenada));
            strcpy(dados->chave, stringConcatenada);
            dados->PR = cabecalho->proxRRN++;
            printf("\n\nPR: %d\n\n",dados->PR);
            dados->rrnDireita = -1;

            adicionar(dados, arquivoIND, cabIndice);

            free(dados->chave);
            free(dados);
            free(stringConcatenada);
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

    cabecalho->nroTecnologias = getTamanho(L);
    cabecalho->nroParesTecnologias = getTamanho(ListaPares);

    // Finalmente, gravando o cabeçalho no início do arquivo
    gravaCabecalho(cabecalho, arquivoBIN);

    // Fazemos o mesmo com o arquivo de índices
    // Alterando o status para '1' antes de fechar o binário
    // cabIndice->status = '1';

    // Finalmente, gravando o cabeçalho no início do arquivo
    // gravaCabecalhoIndice(cabIndice, arquivoIND)

    // Liberando a memória do cabecalho e das listas encadeadas
    free(cabecalho);
    free(cabIndice);
    destroiLista(&L);
    destroiLista(&ListaPares);

    binarioNaTela(nomeArquivoBIN);
    binarioNaTela(nomeArquivoIND);
}
