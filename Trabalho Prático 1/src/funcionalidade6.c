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

// Função que calcula o byte offset de uma página
void posicionaPonteiroPorRRN(int RRN, FILE *arquivoIND){
    // Função que deverá ser usada com fseek com SEEK_SET
    int byteOffset = TAM_PAGINA * (RRN + 1); 

    //printf("Byte offset: %d\n", byteOffset);

    fseek(arquivoIND, byteOffset, SEEK_SET);
}

// Função recursiva que busca a chave em um arquivo de índices
int buscaRecursivaPelaChave(char *nomeChave, FILE *arquivoIND, int proxRRN){

    // Retorna um inteiro que é o RRN desejado ou -1 se não achou
    // *** Percorremos o arquivo de índices recursivamente ***

    // Condição de parada: Não existe uma sub-árvore
    if(proxRRN == -1){
        return -1;
    }

    // Posicionando o ponteiro do arquivo no início do RRN
    posicionaPonteiroPorRRN(proxRRN, arquivoIND);
    
    // Criando e lendo o nó
    NoArvoreB *no = criaNoArvoreB();
    leNoArvoreB(no, arquivoIND);

    // Fazemos uma busca binária nas chaves do nó para encontrar o RRN da sub-árvore
    int pos = buscaBinaria(no, nomeChave);

    // Se a chave da posição "pos" for igual a buscada, encontramos
    if(strcmp(nomeChave, no->C[pos]) == 0){

        return no->PR[pos];
    }

    // Senão, buscamos na sub-árvore correta
    else{
        if (strcmp(nomeChave, no->C[pos]) > 0) pos++;
        
        return buscaRecursivaPelaChave(nomeChave, arquivoIND, no->P[pos]);
    }

}

// Função que chama a recursão para buscar a chave em um arquivo de índices e retorna seu RRN no arquivo de dados (PR)
int buscaPelaChave(char *nomeChave, FILE *arquivoIND){ 

    int rrnBuscado = -1;

    // Lendo o cabeçalho do arquivo de índices
    CabecalhoIndice *cabIndice = criaCabecalhoIndice();
    leCabecalhoIndice(cabIndice, arquivoIND);
    
    // Chamaremos a recursão a partir da raiz para buscar pela chave
    rrnBuscado = buscaRecursivaPelaChave(nomeChave, arquivoIND, cabIndice->noRaiz);

    // Liberando a memória alocada para o cabeçalho
    free(cabIndice);

    return rrnBuscado;
}   

// Funcionalidade 6: busca um campo no arquivo de dados ou busca uma chave em um arquivo de índices
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
    const unsigned int tamTotal = 13 + (TAM_REGISTRO * ultimoRRN);

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
        else if(!strcmp(nomeCampo, "nomeTecnologiaOrigemDestino")){

            // Leitura da entrada entre aspas e chamada da função de busca
            scan_quote_string(valorCampo);

            // Guardo o valor do RRN encontrado
            rrnBuscado = buscaPelaChave(valorCampo, arquivoIND);

            // Com o valor do RRN em mãos, realizamos a busca a partir da funcionalidade 4
            if(rrnBuscado != -1){
                buscaArquivoDados(arquivoBIN, rrnBuscado);
            }
            // Se a função retornou -1, significa que a chave não foi encontrada
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
    }

    // Fechando ambos os arquivos
    fclose(arquivoBIN);
    fclose(arquivoIND);
}
