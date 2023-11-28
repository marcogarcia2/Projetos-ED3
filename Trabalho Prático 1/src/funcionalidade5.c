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
    printf("byteOffset: %d\n", byteOffset);

    // 2. Cria o cabeçalho do arquivo de índices
    CabecalhoIndice *cabecalho = criaCabecalhoIndice();
    // Presumo que aqui já deveríamos colocar '0' no arquivo 
    // fwrite(&cabecalho->status, sizeof(char), 1, arquivoIND);

    // 3. Cria a árvore B (só depois de verificar a consistência)
    // Aqui...
    NoArv *raiz = NULL;

    // 4. Lê o arquivo de dados e insere na árvore B concomitantemente (?)

    while(1){

        // Cria o registro que será lido
        Registro *r = criaRegistro();

        // Lê o registro do arquivo de dados
        r = leRegistro(byteOffset, r, arquivoBIN);

        // Se acabaram os registros, chegou ao final do arquivo
        if (r == NULL) {
            free(r); // Libera o registro
            break;
        }

        // OBS.: O registro só poderá ser inserido se ele não estiver removido (TESTAR O 204)
        // if(r->removido == '1') 
            // fseek(arquivoBIN, 204, SEEK_CUR); // Se estiver removido, pula para o próximo registro

        // Criando a chave (stringConcatenada) concatenando nomeTecnologiaOrigem e nomeTecnologiaDestino
        char *stringConcatenada = concatenaStrings(r); 
        //printf("String inserida: %s\n", stringConcatenada);
        // Insere stringConcatenada como C (chave) na árvore B
        raiz = inserirChave(stringConcatenada, raiz);
        // ALTERAR ESTA FUNÇÃO, DE MODO QUE OS ATRIBUTOS SEJAM PREENCHIDOS
        // RRN DO NÓ E ALTURA DO NÓ???
        // P, PR???


        // Libera o registro completo e a string alocada
        liberaRegistro(r);
        free(stringConcatenada);

        // Precisamos saltar até o próximo registro
        byteOffset += TAM_REGISTRO;
    }

    // 5. Grava a árvore B no arquivo de índices (ORDEM ALFABÉTICA PARA TESTAR)
    gravaArvore(raiz, arquivoIND);

    //imprimeArvoreB(raiz);

    // Grava o cabecalho no arquivo de índices
    gravaCabecalhoIndice(cabecalho, arquivoIND);

    // Desalocando a memória da Árvore B
    destroiArvoreB(raiz);

    // Fecha os arquivos
    fclose(arquivoBIN);
    fclose(arquivoIND);

    binarioNaTela(nomeArquivoIND);

}






