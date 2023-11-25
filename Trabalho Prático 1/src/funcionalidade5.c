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
    CabecalhoIndice *cabecalho = criaCabecalhoIndice();

    // Grava o cabecalho no arquivo de índices
    gravaCabecalhoIndice(cabecalho, arquivoIND);

    // Algoritmo:

    // 1. Verifica a consistência do arquivo de dados
    int byteOffset = 0;

    // Se o arquivo de dados estiver consistente
    if(fgetc(arquivoBIN) == '1') // Pula os 13 primeiros bytes do arquivo, referentes ao cabeçalho
        byteOffset = 13;
        
    else{ // Se estiver inconsistente
        printf("Falha no processamento do arquivo.\n");
        fclose(arquivoBIN);
        fclose(arquivoIND);
        return;
    }

    // 2. Cria a árvore B (só depois de verificar a consistência)
    // Aqui...

    // 3. Lê o arquivo de dados e insere na árvore B concomitantemente (?)

    while(1){
        // Cria o registro que será lido
        Registro *r = criaRegistro();
        r = leRegistro(byteOffset, r, arquivoBIN);

        // Se acabaram os registros, chegou ao final do arquivo
        if (r == NULL) {
            free(r); // Libera o registro
            break;
        }

        // Criando a chave (stringConcatenada) concatenando nomeTecnologiaOrigem e nomeTecnologiaDestino
        char *stringConcatenada = concatenaStrings(r); // Lembrar de dar free nessa string alocada depois

        // Insere stringConcatenada como C (chave) na árvore B
        // Aqui...

        // Libera o registro completo
        liberaRegistro(r);

        // Precisamos saltar até o próximo registro
        byteOffset += TAM_REGISTRO;
    }

    // 4. Grava a árvore B no arquivo de índices
    // Aqui...

    // Fecha os arquivos
    fclose(arquivoBIN);
    fclose(arquivoIND);

    binarioNaTela(nomeArquivoIND);

}






