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
#include "grafo.h"

// Funcionalidade 10: lista todas as tecnologias de origem associadas a "n" tecnologias de destino dadas pelo usuário
void buscaTecnologia(Grafo *grafoTransposto, char *nomeTecnologiaDestino){

    // Calculando a posição desejada no grafo
    int posDestino = buscaBinariaGrafo(grafoTransposto, nomeTecnologiaDestino);

    // O vértice não existe no grafo
    if(strcmp(grafoTransposto->vertices[posDestino].tecnologia, nomeTecnologiaDestino) != 0){
        printf("Registro inexistente.\n");
        return;
    }

    // Se não, quer dizer que existe e precisamos listar as arestas associadas (tecnologias de origem)

    // Criando a aresta que percorrerá o grafo, começando na inicial do vértice desejado
    Aresta *arestaAtual = grafoTransposto->vertices[posDestino].arestaInicial;

    // Imprimindo o nome da tecnologia de destino
    printf("%s:", nomeTecnologiaDestino);

    // Imprimindo as tecnologias de origem
    while(arestaAtual != NULL){
        printf(" %s", arestaAtual->tecDestino);
        if(arestaAtual->prox != NULL) {
            printf(",");
        }
        arestaAtual = arestaAtual->prox;
    }

    // Pulando uma linha para a próxima execução
    printf("\n\n");
}

void listaTecnologias(char *nomeArquivoBIN, int n){

    // Gera um grafo a partir do arquivo binário
    // Abrindo o arquivo binário
    FILE *arquivoBIN = fopen(nomeArquivoBIN, "rb");
    if (arquivoBIN == NULL){
        printf("Falha no processamento do arquivo.\n");
        return;
    }

    // Lê o cabeçalho do arquivo binário
    Cabecalho *cabecalho = criaCabecalho();
    leCabecalho(cabecalho, arquivoBIN);

    // Verifica se o arquivo está consistente
    if (cabecalho->status == '0'){
        printf("Falha no processamento do arquivo.\n");
        free(cabecalho);
        fclose(arquivoBIN);
        return;
    }

    // Criando as estruturas que iremos utilizar
    Grafo *grafo = criaGrafo();
    Registro *r;

    // Variáveis para percorrer o arquivo
    const unsigned int tamTotal = 13 + TAM_REGISTRO * (cabecalho->proxRRN - 1);
    unsigned int byteOffset = 13;

    // Percorre o arquivo binário lendo os registros
    while (byteOffset <= tamTotal){
        
        // Lendo o registro
        r = criaRegistro();
        r = leRegistro(byteOffset, r, arquivoBIN);

        // Verifica se o registro foi removido
        if (r->removido == '1'){
            byteOffset += TAM_REGISTRO;
            free(r);
            continue;
        }

        // Insere o registro no grafo
        insereGrafo(grafo, r);

        liberaRegistro(r);


        byteOffset += TAM_REGISTRO;
    }

    // Até aqui, igual a funcionalidade 8.
    // Agora que temos o grafo, vamos transpô-lo.
    Grafo *grafoTransposto = transpor(grafo);
    char nomeTecnologiaDestino[30];

    for(int i = 0; i < n; i++){

        // Lê a tecnologia de destino
        scan_quote_string(nomeTecnologiaDestino);

        // Busca a tecnologia de destino no grafo transposto e recupera as de origem
        buscaTecnologia(grafoTransposto, nomeTecnologiaDestino);
    }

    // nomeTecnologia, grupo, grauEntrada, grauSaida, grau, nomeTecnologiaDestino, peso
    destroiGrafo(grafo);
    destroiGrafo(grafoTransposto);

    fclose(arquivoBIN);
    free(cabecalho);
}