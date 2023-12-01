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

// Funcionalidade 6



// Função que calcula o byte offset de uma página
void posicionaPonteiroPorRRN(int RRN, FILE *arquivoIND){
    // Função que deverá ser usada com fseek com SEEK_SET
    int byteOffset = TAM_PAGINA * (RRN + 1); 

    //printf("Byte offset: %d\n", byteOffset);

    fseek(arquivoIND, byteOffset, SEEK_SET);
}

int buscaRecursivaPelaChave(char *nomeChave, FILE *arquivoIND, int proxRRN, char *nomeArquivoBIN){
    // Retorna um inteiro que é o RRN desejado ou -1 se não achou

    // Condição de parada
    if(proxRRN == -1){
        return -1;
    }

    // Posicionando o ponteiro do arquivo no início do RRN
    posicionaPonteiroPorRRN(proxRRN, arquivoIND);
    
    // Logica de busca do RRN
    NoArvoreB *no = criaNoArvoreB();
    leNoArvoreB(no, arquivoIND);

         int pos = buscaBinaria(no, nomeChave);

    imprimeNoArvoreB(no);
    printf("Posicao: %d\n", pos);

    if(strcmp(nomeChave, no->C[pos]) == 0){
        return(no->PR[pos]);
    }
    else{
        if (strcmp(nomeChave, no->C[pos]) > 0){
            pos++;
        }
        // printf("Posicao: %d\n", pos);
        return buscaRecursivaPelaChave(nomeChave, arquivoIND, no->P[pos], nomeArquivoBIN);
    }

    //Vamos verificar se a chave está no nó atual
    for(int i = 0; i < no->nroChavesNo; i++){
        if(strcmp(nomeChave, no->C[i]) <= 0){
            if(strcmp(nomeChave, no->C[i]) == 0){
                // Achamos a chave, retornamos seu ponteiro de referência para o arquivo de dados
                return no->PR[i];
            }
            else{ // Ou seja, se for menor na ordem alfabética, vamos para a esquerda
                return buscaRecursivaPelaChave(nomeChave, arquivoIND, no->P[i], nomeArquivoBIN);
            }
        }
        else // Se não for menor nem igual, vou para a próxima chave!
            continue;
    }

    // Se chegamos aqui, significa que a chave é maior que todas as chaves do nó atual, então passamos P[3] = P[nroChavesNo]
    return buscaRecursivaPelaChave(nomeChave, arquivoIND, no->P[no->nroChavesNo], nomeArquivoBIN);
}

int buscaPelaChave(char *nomeChave, FILE *arquivoIND, char *nomeArquivoBIN){ // Essa funcao retorna o Pr da chave buscada
    int rrnBuscado = -1;

    // Logica de busca do RRN
    int rrnRaiz;

    // Primeiro eu coloco o ponteiro do meu arquivo na raiz (no / pagina)
    

    // Leio onde está o RRN do nó da raiz
    fread(&rrnRaiz, sizeof(int), 1, arquivoIND);
    // printf("RRN da raiz: %d\n", rrnRaiz);
    
    // Estamos na raiz, agora chamaremos a recursão para tentar encontrar a chave
    rrnBuscado = buscaRecursivaPelaChave(nomeChave, arquivoIND, rrnRaiz, nomeArquivoBIN);


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

    // Variáveis auxiliares
    char nomeCampo[30], valorCampo[30];
    int valorCampoint;
    int rrnBuscado;

    // Calculando o tamanho total do arquivo
    int ultimoRRN;
    fread(&ultimoRRN, sizeof(int), 1, arquivoBIN); 
    ultimoRRN--;

    // Calculando o tamanho total do arquivo
    const unsigned int tamTotal = 13 + (TAM_REGISTRO * ultimoRRN);

    for(int i = 0; i < n; i++){

        // Leitura do nome do campo
        scanf("%s", nomeCampo);

        // Pula o cabeçalho do arquivo de dados
        fseek(arquivoBIN, 13, SEEK_SET);
        fseek(arquivoIND, 1, SEEK_SET);

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
            rrnBuscado = buscaPelaChave(valorCampo, arquivoIND, nomeArquivoBIN);
            //printf("RRN buscado: %d\n", rrnBuscado);

            // Realizo a busca a partir da funcionalidade 4
            if(rrnBuscado != -1){
                buscaPorRRN(nomeArquivoBIN, rrnBuscado);
            }
            else{
                printf("Registro inexistente.\n");
            }
        }

        // Se o campo for um inteiro: grupo, popularidade ou peso
        else {
            // Leitura da entrada e chamada da função de busca
            scanf("%d", &valorCampoint);
            buscaInteiro(nomeCampo, valorCampoint, tamTotal, arquivoBIN);
        }

        // Volto para o início do arquivo de dados
        //fseek(arquivoBIN, 0, SEEK_SET);
    }

    fclose(arquivoBIN);
    fclose(arquivoIND);
}
