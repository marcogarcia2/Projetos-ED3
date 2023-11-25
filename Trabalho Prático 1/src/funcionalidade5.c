/*  
*   Lucas Lima Romero - 13676325
*   Marco Antonio Gaspar Garcia - 11833581
*/

/*  
*  Fazer uma função que lê um arquivo binário definido anteriormente no trabalho introdutório, e a
*  partir disso criar um novo arquivo binário (de índice), que representa uma estrutura de árvore B.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "funcionalidades.h"
#include "registros.h"
#include "arvoreB.h"

// Funcionalidade 5: criar um arquivo de índice a partir de um arquivo binário

void geraArquivoIndice(char *nomeArquivoBIN, char *nomeArquivoIND){

    // Abre o arquivo de dados
    FILE *arquivoBIN = fopen(nomeArquivoBIN, 'rb');
    if(arquivoBIN == NULL){
        printf("Falha no carregamento do Arquivo Binário.\n");
        return;
    }

    // Cria o arquivo de índices
    FILE *arquivoIND = fopen(nomeArquivoIND, 'wb');
    if(arquivoIND == NULL){
        printf("Falha no carregamento do Arquivo Binário.\n");
        return;
    }

    // Criando o cabeçalho do arquivo de índices
    CabecalhoIndice *cabecalho = criaCabecalho();

    // Grava o cabecalho no arquivo de índices
    gravaCabecalhoIndice(cabecalho, arquivoIND);

    // Algoritmo:

    // Cria a árvore B
    // Lê o arquivo de dados e insere na árvore B
    // Grava a árvore B no arquivo de índices

    // Fecha os arquivos
    fclose(arquivoBIN);
    fclose(arquivoIND);

    binarioNaTela(nomeArquivoIND);

}






