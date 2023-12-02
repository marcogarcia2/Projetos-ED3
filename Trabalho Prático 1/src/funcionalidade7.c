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
            // Se o registro não está removido, adicionamos as tecnologias às listas
            if (r->removido == '0'){

                adicionaLista(ListaNroTecnologias, r->tecnologiaOrigem.string, r->tecnologiaOrigem.tamanho);
                adicionaLista(ListaNroTecnologias, r->tecnologiaDestino.string, r->tecnologiaDestino.tamanho);

                if(strcmp(r->tecnologiaOrigem.string, "") != 0 && strcmp(r->tecnologiaDestino.string, "") != 0){
                    char *stringConcatenada = concatenaStrings(r); //TESTAR
                    adicionaLista(ListaPares, stringConcatenada, r->tecnologiaOrigem.tamanho + r->tecnologiaDestino.tamanho); // Se precisar colocar um +1
                    free(stringConcatenada);
                }
            }
        }

        // Desalocando a memória
        liberaRegistro(r);

        // Precisamos saltando até o próximo registro
        byteOffset += TAM_REGISTRO;
    }
}

// Função que recebe cinco strings e preenche um registro
void preencherRegistro(Registro *r, char *nomeTecnologiaOrigem, char *grupo, char *popularidade, char *nomeTecnologiaDestino, char *peso) {

    // Aqui verificamos se o campo é nulo e atribuímos o valor adequado

    // Tecnologia Origem
    r->tecnologiaOrigem.tamanho = (strcmp(nomeTecnologiaOrigem, "NULO") != 0) ? strlen(nomeTecnologiaOrigem) : 0;
    r->tecnologiaOrigem.string = malloc(r->tecnologiaOrigem.tamanho + 1);
    strcpy(r->tecnologiaOrigem.string, (r->tecnologiaOrigem.tamanho != 0) ? nomeTecnologiaOrigem : "");

    // Grupo
    r->grupo = (strcmp(grupo, "NULO") == 0) ? -1 : atoi(grupo);

    // Popularidade
    r->popularidade = (strcmp(popularidade, "NULO") == 0) ? -1 : atoi(popularidade);

    // Tecnologia Destino
    r->tecnologiaDestino.tamanho = (strcmp(nomeTecnologiaDestino, "NULO") != 0) ? strlen(nomeTecnologiaDestino) : 0;
    r->tecnologiaDestino.string = malloc(r->tecnologiaDestino.tamanho + 1);
    strcpy(r->tecnologiaDestino.string, (r->tecnologiaDestino.tamanho != 0) ? nomeTecnologiaDestino : "");

    // Peso
    r->peso = (strcmp(peso, "NULO") == 0) ? -1 : atoi(peso);
}

// Chamando a função de gravação de um registro no arquivo de dados
void insereArquivoDados(Registro *r, FILE *arquivoBIN, Cabecalho *cabecalho){
    gravaRegistro(r, arquivoBIN);
}

// Chamando a função que insere no arquivo de índices
void insereArquivoIndice(DadosChave *dados, FILE *arquivoIND, CabecalhoIndice *cabIndice){
    adicionar(dados, arquivoIND, cabIndice);
}

// Funcionalidade 7: Inserir registros no arquivo de dados e sua chave no arquivo de índices
void insercaoNosArquivos(char *nomeArquivoBIN, char *nomeArquivoIND, int n){

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

    // Lendo as informações do cabecalho do arquivo de índices
    CabecalhoIndice *cabIndice = criaCabecalhoIndice();
    leCabecalhoIndice(cabIndice, arquivoIND); 

    // Alterando o status para '0' e gravando os cabeçalhos (inconsistentes)
    cabecalho->status = '0';
    cabIndice->status = '0';
    gravaCabecalho(cabecalho, arquivoBIN);
    gravaCabecalhoIndice(cabIndice, arquivoIND);
    
    // Listas para guardar o número de tecnologias
    Lista *ListaNroTecnologias = criaLista();
    Lista *ListaPares = criaLista();

    // Lendo o arquivo de dados e guardando o número de tecnologias e pares de tecnologias
    contagemTecnologiasEPares(ListaNroTecnologias, ListaPares, arquivoBIN);

    // Criando um loop que irá ler as "n" linhas do teclado
    for (int k = 0; k < n; k++) {

        // Posicionando o ponteiro no final do arquivo
        if(k == 0)
            fseek(arquivoBIN, 0, SEEK_END);
        
        // Lendo as entradas do teclado
        char nomeTecnologiaOrigem[100], grupo[10], popularidade[10], nomeTecnologiaDestino[100], peso[10];
        scanf("%*c"); // Lendo o \n
        scanf("%[^,], %[^,], %[^,], %[^,], %[^\n]", nomeTecnologiaOrigem, grupo, popularidade, nomeTecnologiaDestino, peso);
       
        // Criando um registro e preenchendo-o com as entradas
        Registro *r = criaRegistro();
        preencherRegistro(r, nomeTecnologiaOrigem, grupo, popularidade, nomeTecnologiaDestino, peso);

        // Agora que temos um registro preenchido, vamos gravá-lo no arquivo de dados
        insereArquivoDados(r, arquivoBIN, cabecalho);
        
        // Contabilizando as tecnologias
        adicionaLista(ListaNroTecnologias, r->tecnologiaOrigem.string, r->tecnologiaOrigem.tamanho);        
        adicionaLista(ListaNroTecnologias, r->tecnologiaDestino.string, r->tecnologiaDestino.tamanho);


        // Se ambas NÃO forem nulas, adicionamos o par na lista de pares e no arquivo de índices
        if(strcmp(r->tecnologiaOrigem.string, "") != 0 && strcmp(r->tecnologiaDestino.string, "") != 0){
            
            // Criando um dado para inserção no arquivo de índices
            DadosChave *dados = criaDadosChave();
            dados->chave = concatenaStrings(r);
            dados->PR = cabecalho->proxRRN;

            // Contabilizando o par de tecnologias lista de pares
           adicionaLista(ListaPares, dados->chave, r->tecnologiaOrigem.tamanho + r->tecnologiaDestino.tamanho); // Se precisar colocar um +1

            // Inserindo no arquivo de índices
            insereArquivoIndice(dados, arquivoIND, cabIndice);

            // Liberando a memória alocada para a chave
            free(dados->chave);
            free(dados);
        }

        // Atualizando o cabeçalho do arquivo de dados
        cabecalho->proxRRN++;

        // Liberando a memória do registro
        liberaRegistro(r);
    }

    // Alterando o status para '1' antes de fechar os binários
    cabecalho->status = '1';
    cabIndice->status = '1';

    // Atualizando o número de tecnologias e pares de tecnologias
    cabecalho->nroTecnologias = getTamanho(ListaNroTecnologias);
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
