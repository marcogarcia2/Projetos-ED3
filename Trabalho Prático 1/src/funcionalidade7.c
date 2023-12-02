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

// PROBLEMA: ESTA GRAVANDO NULO NO BINARIO QUANDO NAO DEVERIA GRAVAR NADA (SLA PQ)

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

                adicionaLista(ListaNroTecnologias, r->tecnologiaOrigem.string, r->tecnologiaOrigem.tamanho);
                adicionaLista(ListaNroTecnologias, r->tecnologiaDestino.string, r->tecnologiaDestino.tamanho);

                if(strcmp(r->tecnologiaOrigem.string, "") != 0 && strcmp(r->tecnologiaDestino.string, "") != 0){
                    char *stringConcatenada = concatenaStrings(r); //TESTAR
                    adicionaLista(ListaPares, stringConcatenada, r->tecnologiaOrigem.tamanho + r->tecnologiaDestino.tamanho); // Se precisar colocar um +1
                }
            }
        }

        // Desalocando a memória
        liberaRegistro(r);

        // Precisamos saltar até o próximo registro
        byteOffset += TAM_REGISTRO;
    }

    // printf("Numero de tecnologias: %d\n", getTamanho(ListaNroTecnologias));
    // printf("Numero de pares de tecnologias: %d\n", getTamanho(ListaPares));

}

void preencherRegistro(Registro *r, char *nomeTecnologiaOrigem, char *grupo, char *popularidade, char *nomeTecnologiaDestino, char *peso) {

    r->tecnologiaOrigem.tamanho = (strcmp(nomeTecnologiaOrigem, "NULO") != 0) ? strlen(nomeTecnologiaOrigem) : 0;
    r->tecnologiaOrigem.string = malloc(r->tecnologiaOrigem.tamanho + 1);
    strcpy(r->tecnologiaOrigem.string, (r->tecnologiaOrigem.tamanho != 0) ? nomeTecnologiaOrigem : "");

    r->grupo = (strcmp(grupo, "NULO") == 0) ? -1 : atoi(grupo);

    r->popularidade = (strcmp(popularidade, "NULO") == 0) ? -1 : atoi(popularidade);

    r->tecnologiaDestino.tamanho = (strcmp(nomeTecnologiaDestino, "NULO") != 0) ? strlen(nomeTecnologiaDestino) : 0;
    r->tecnologiaDestino.string = malloc(r->tecnologiaDestino.tamanho + 1);
    strcpy(r->tecnologiaDestino.string, (r->tecnologiaDestino.tamanho != 0) ? nomeTecnologiaDestino : "");

    r->peso = (strcmp(peso, "NULO") == 0) ? -1 : atoi(peso);

}

void insereArquivoDados(Registro *r, FILE *arquivoBIN, Cabecalho *cabecalho){
    gravaRegistro(r, arquivoBIN);
}

void insereArquivoIndice(DadosChave *dados, FILE *arquivoIND, CabecalhoIndice *cabIndice){
    adicionar(dados, arquivoIND, cabIndice);
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
    leCabecalho(cabecalho, arquivoBIN); 

    // Temos as informações do cabecalho do arquivo de índices
    CabecalhoIndice *cabIndice = criaCabecalhoIndice();
    leCabecalhoIndice(cabIndice, arquivoIND); // Nao temos essa funcao ainda

    // printf("RRN da raiz: %d\n", cabIndice->noRaiz);

    // Alterando o status para '0' 
    cabecalho->status = '0';
    cabIndice->status = '0';

    // Gravando o cabeçalho no início de cada arquivo
    gravaCabecalho(cabecalho, arquivoBIN);
    gravaCabecalhoIndice(cabIndice, arquivoIND);
    
    // Variáveis que nos auxiliarão na análise do arquivo
    Lista *ListaNroTecnologias = criaLista();
    Lista *ListaPares = criaLista();

    contagemTecnologiasEPares(ListaNroTecnologias, ListaPares, arquivoBIN);
    //int byteAtual = ftell(arquivoBIN);

    // Criando um loop que irá ler as "n" linhas do teclado
    for (int k = 0; k < n; k++) {
        fseek(arquivoBIN, 0, SEEK_END);
        
        char nomeTecnologiaOrigem[100], grupo[10], popularidade[10], nomeTecnologiaDestino[100], peso[10];
        scanf("%*c"); // Lendo o \n
        scanf("%[^,], %[^,], %[^,], %[^,], %[^\n]", nomeTecnologiaOrigem, grupo, popularidade, nomeTecnologiaDestino, peso);

        //Printando a entrada para testar
        // printf("Tamanho da string tecnologiaOrigem: %d\n", strlen(nomeTecnologiaOrigem));
        // printf("%s\n", nomeTecnologiaOrigem);
        // printf("%s\n", grupo);
        // printf("%s\n", popularidade);
        // printf("Tamanho da string tecnologiaDestino: %d\n", strlen(nomeTecnologiaDestino));
        // printf("%s\n", nomeTecnologiaDestino);
        // printf("%s\n", peso);

        Registro *r = criaRegistro();
        preencherRegistro(r, nomeTecnologiaOrigem, grupo, popularidade, nomeTecnologiaDestino, peso);
        
        // Printando os tamanhos das strings para testar
        // printf("Tamanho da string tecnologiaOrigem FINAL: %d\n", r->tecnologiaOrigem.tamanho);
        // printf("Tamanho da string tecnologiaDestino FINAL: %d\n", r->tecnologiaDestino.tamanho);

        // -------------------- INSERÇÃO NO ARQUIVO DE DADOS --------------------------- //

        insereArquivoDados(r, arquivoBIN, cabecalho);
        
        adicionaLista(ListaNroTecnologias, r->tecnologiaOrigem.string, r->tecnologiaOrigem.tamanho);        
        adicionaLista(ListaNroTecnologias, r->tecnologiaDestino.string, r->tecnologiaDestino.tamanho);

        DadosChave *dados = criaDadosChave();

        // Adiciona na lista de pares a string concatenada
        if(strcmp(r->tecnologiaOrigem.string, "") != 0 && strcmp(r->tecnologiaDestino.string, "") != 0){
            // Só adiciono na lista se nenhuma for nula
            dados->chave = concatenaStrings(r);
            adicionaLista(ListaPares, dados->chave, r->tecnologiaOrigem.tamanho + r->tecnologiaDestino.tamanho); // Se precisar colocar um +1
            dados->PR = cabecalho->proxRRN; // Pois o RRN do nó foi incrementado anteriormente

            insereArquivoIndice(dados, arquivoIND, cabIndice);
        }

        // ------------------------------------------------------------------------------ //

        // -------------------- INSERÇÃO NO ARQUIVO DE INDICES -------------------------- //
        
        cabecalho->proxRRN++;
        

        // ------------------------------------------------------------------------------ //

        gravaCabecalho(cabecalho, arquivoBIN);
        gravaCabecalhoIndice(cabIndice, arquivoIND);
        // Liberando a memória do registro
        liberaRegistro(r);
    }

    // printf("Numero de tecnologias: %d\n", getTamanho(ListaNroTecnologias));
    // printf("Numero de pares de tecnologias: %d\n", getTamanho(ListaPares));

    // Alterando o status para '1' antes de fechar os binários
    cabecalho->status = '1';
    cabIndice->status = '1';

    cabecalho->nroTecnologias = getTamanho(ListaNroTecnologias); // Acochambramento
    cabecalho->nroParesTecnologias = getTamanho(ListaPares);

    // Finalmente, gravando o cabeçalho no início de cada arquivo
    gravaCabecalho(cabecalho, arquivoBIN);
    gravaCabecalhoIndice(cabIndice, arquivoIND);

    // Liberando a memória do cabecalho e das listas encadeadas
    free(cabecalho);
    free(cabIndice);
    destroiLista(&ListaNroTecnologias);
    destroiLista(&ListaPares);

    // Fechando os arquivos
    fclose(arquivoBIN);
    fclose(arquivoIND);

    binarioNaTela(nomeArquivoBIN);
    binarioNaTela(nomeArquivoIND);
}
