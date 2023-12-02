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

// Funcionalidade 5: criar um arquivo de índice a partir de um arquivo de dados binário

void geraArquivoIndice(char *nomeArquivoBIN, char *nomeArquivoIND){

    // Abre o arquivo de dados
    FILE *arquivoBIN = fopen(nomeArquivoBIN, "rb");
    if(arquivoBIN == NULL){
        printf("Falha no processamento do arquivo.\n");
        return;
    }

    // Cria o arquivo de índices
    FILE *arquivoIND = fopen(nomeArquivoIND, "wb+");
    if(arquivoIND == NULL){
        printf("Falha no processamento do arquivo.\n");
        return;
    }

    
    // Byte de controle do arquivo de dados
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

    // Cria o cabeçalho do arquivo de índices
    CabecalhoIndice *cabecalho = criaCabecalhoIndice();

    // Colocando '0' de status no arquivo (pois foi aberto)
    gravaCabecalhoIndice(cabecalho, arquivoIND);

    // Cria o registro que será lido em memória principal e um struct DadosChave
    Registro *r;
    DadosChave *dados;

    // Contador de RRN (começa no 0 e é incrementado a cada leitura ou pulo de registro)
    int ponteiroReferencia = 0;

    // Tamanho total do arquivo de dados
    const unsigned int tamTotal = calculaTamanhoTotal(arquivoBIN);

    // Lê o arquivo de dados e insere na árvore B concomitantemente
    while(byteOffset <= tamTotal){ // Quando ocorre o split na raiz está dando erro (TAM_REGISTRO * 4)

        // Cria e lê o registro do arquivo de dados que será lido em memória principal
        r = criaRegistro(); 
        r = leRegistro(byteOffset, r, arquivoBIN);

        if(r->removido == '0' && (strcmp(r->tecnologiaOrigem.string, "") && strcmp(r->tecnologiaDestino.string, ""))){ // Se não estiver removido, ocorrerá a inserção no arquivo de índices
            
            // Criando a chave (stringConcatenada) concatenando nomeTecnologiaOrigem e nomeTecnologiaDestino
            dados = criaDadosChave();
            dados->chave = concatenaStrings(r);

            // Preciso guardar também o que será o PR, que é o RRN dessa chave (ponteiroReferencia)
            dados->PR = ponteiroReferencia;

            // Agora preciso inserir essa chave na árvore B
            adicionar(dados, arquivoIND, cabecalho);

            // Libera a chave
            free(dados->chave);
            free(dados);
        }

        // Libera o registro completo
        liberaRegistro(r);
        
        // Precisamos saltar até o próximo registro
        byteOffset += TAM_REGISTRO;

        // Precisamos incrementar o PR 
        ponteiroReferencia++;
    }

    // Grava o cabecalho no arquivo de índices
    cabecalho->status = '1';
    gravaCabecalhoIndice(cabecalho, arquivoIND); 

    // Fecha os arquivos
    fclose(arquivoBIN);
    fclose(arquivoIND);

    binarioNaTela(nomeArquivoIND);
}
