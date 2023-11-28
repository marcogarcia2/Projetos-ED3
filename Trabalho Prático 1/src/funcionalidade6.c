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

#define TAM_PAGINA 205

// Funcionalidade 6

/* Pensando em voz alta (TIRAR DEPOIS)

A primeira coisa que preciso fazer eh:
. Ler o que queremos procurar, se eh:
    - chave de busca (concatenacao origem destino) (func 6) + (func 4)
    - tecnologia de origem, destino (func 3 - busca string)
    - (int) grupo - chamo a (func 3 - busca inteiro)
    - (int) popularidade - chamo a (func 3 - busca inteiro)
    - (int) peso - chamo a (func 3 - busca inteiro)

Em seguida, vamos

. Abrir o arquivo de dados 
. Checar sua consistencia
. Abrir arquivo de indices (so preciso abrir se o anterior for valido)

    Cabecalho arquivo de indices:
        - status (Checagem no início) ###
        - noRaiz (atualmente) - vou usar para começar minha busca (começo a partir dele)
        - RRNproxNo (servira para a funcao de insercao, pelo que entendi) ###

    Pagina de dados (cada nó também armazena)
        - nroChavesNo
        - alturaNo
        - RRNdoNo

*/

// Função que calcula o byte offset de uma página
void posicionaPonteiroPorRRN(int RRN, FILE *arquivoIND){
    // Função que deverá ser usada com fseek com SEEK_SET
    int byteOffset = TAM_PAGINA * (RRN + 1); 

    printf("Byte offset: %d\n", byteOffset);

    fseek(arquivoIND, byteOffset, SEEK_SET);
}

int buscaPelaChave(char *nomeChave, FILE *arquivoIND){ // Essa funcao retorna o Pr da chave buscada
    int rrnBuscado = -1;

    // Logica de busca do RRN
    int rrnRaiz;

    // Primeiro eu coloco o ponteiro do meu arquivo na raiz (no / pagina)

    // Leio onde está o RRN do nó da raiz
    fread(&rrnRaiz, sizeof(int), 1, arquivoIND);
    posicionaPonteiroPorRRN(rrnRaiz, arquivoIND);

    // Estamos na raiz, agora precisamos fazer o processamento dos dados, para saber para onde ir
    // Vou carregar para a memória principal a página da raiz
    NoArvoreB *no = criaNoArvoreB();
    leNoArvoreB(no, arquivoIND);
    imprimeNoArvoreB(no);

    printf("RRN da raiz: %d\n", rrnRaiz);

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

    // Abrindo o arquivo de índices
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
    const unsigned int tamTotal = 13 + (TAM_PAGINA * ultimoRRN);

    for(int i = 0; i < n; i++){

        // Leitura do nome do campo
        scanf("%s", nomeCampo);

        // Pula o cabeçalho do arquivo de dados
        fseek(arquivoBIN, 13, SEEK_SET);

        /* O tratamento será diferente para buscarmos uma string ou um inteiro */

        // Se o campo for string: nomeTecnologiaOrigem ou nomeTecnologiaDestino
        if(!strcmp(nomeCampo, "nomeTecnologiaOrigem") || !strcmp(nomeCampo, "nomeTecnologiaDestino")){
            
            // Leitura da entrada entre aspas e chamada da função de busca
            scan_quote_string(valorCampo); 
            buscaString(nomeCampo, valorCampo, tamTotal, arquivoBIN);
        }

        // Se o campo for a chave de busca
        else if(!strcmp(nomeCampo, "nomeTecnologiaOrigemDestino")){

            // Leitura da entrada entre aspas e chamada da função de busca
            scan_quote_string(valorCampo);

            // Guardo o valor do RRN encontrado
            rrnBuscado = buscaPelaChave(valorCampo, arquivoIND);
            printf("RRN buscado: %d\n", rrnBuscado);
            // Realizo a busca a partir da funcionalidade 4
            // if(rrnBuscado != -1){
            //     buscaPorRRN(nomeArquivoBIN, rrnBuscado);
            // }
        }

        // Se o campo for um inteiro: grupo, popularidade ou peso
        else {

            // Leitura da entrada e chamada da função de busca
            scanf("%d", &valorCampoint);
            buscaInteiro(nomeCampo, valorCampoint, tamTotal, arquivoBIN);
        }

        // Volto para o início do arquivo de dados
        fseek(arquivoBIN, 0, SEEK_SET);
    }

    fclose(arquivoBIN);
    fclose(arquivoIND);
}