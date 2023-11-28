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
#include "funcoesCriadas.h"
#include "funcoesFornecidas.h"
#include "funcoesIndice.h"
#include "arvoreB.h"

// Funcionalidade 5: criar um arquivo de índice a partir de um arquivo binário

void geraArquivoIndice(char *nomeArquivoBIN, char *nomeArquivoIND){

    
    // Abre o arquivo de dados
    FILE *arquivoBIN = fopen(nomeArquivoBIN, "rb");
    if(arquivoBIN == NULL){
        printf("Falha no processamento do arquivo.\n");
        return;
    }

    // Cria o arquivo de índices
    FILE *arquivoIND = fopen(nomeArquivoIND, "wb");
    if(arquivoIND == NULL){
        printf("Falha no processamento do arquivo.\n");
        return;
    }

    // Verifica a consistência do arquivo de dados
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
    printf("byteOffset: %d\n", byteOffset);

    // Cria o cabeçalho do arquivo de índices
    CabecalhoIndice *cabecalho = criaCabecalhoIndice();

    // Colocando '0' de status no arquivo (pois foi aberto)
    gravaCabecalhoIndice(cabecalho, arquivoIND);

    // Cria um nó da árvore B (raiz) em memória principal
    //NoArvoreB *noArv = criaNoArvoreB();

    // Cria o registro que será lido em memória principal
    Registro *r;

    // Tamanho total do arquivo de dados
    const unsigned int tamTotal = calculaTamanhoTotal(arquivoBIN);
    printf("Tamanho total: %d\n", tamTotal);

    // Lê o arquivo de dados e insere na árvore B concomitantemente
    while(byteOffset < tamTotal){

        // Cria e lê o registro do arquivo de dados que será lido em memória principal
        r = criaRegistro(); 
        r = leRegistro(byteOffset, r, arquivoBIN);

        // Se acabaram os registros, chegou ao final do arquivo
        // if (r == NULL) {
        //     free(r); // Libera o registro
        //     break;
        // }
        if(r->removido == '0'){ // Se não estiver removido
            // Insere o registro no arquivo de índices
            // Criando a chave (stringConcatenada) concatenando nomeTecnologiaOrigem e nomeTecnologiaDestino
            char *stringConcatenada = concatenaStrings(r); 
            //printf("String inserida: %s\n", stringConcatenada); 


        }

        // OBS.: O registro só poderá ser inserido se ele não estiver removido (TESTAR O 204)
        // if não removido eu insiro, se não eu continuo e dou byteOffset += TAM_REGISTRO; normalmente

        

        // Libera o registro completo e a string alocada
        liberaRegistro(r);
        free(stringConcatenada);

        // Precisamos saltar até o próximo registro
        byteOffset += TAM_REGISTRO;
    }

    // Grava a árvore B no arquivo de índices (ORDEM ALFABÉTICA PARA TESTAR)
    //gravaArvore(raiz, arquivoIND);

    //imprimeArvoreB(raiz);

    // Grava o cabecalho no arquivo de índices
    cabecalho->status = '1';
    gravaCabecalhoIndice(cabecalho, arquivoIND); 

    // Desalocando a memória da Árvore B
    //destroiArvoreB(raiz);

    // Fecha os arquivos
    fclose(arquivoBIN);
    fclose(arquivoIND);

    binarioNaTela(nomeArquivoIND);

}






