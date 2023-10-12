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

// Funcionalidade 2: imprimir todo um arquivo binário na tela

void recuperaDados(const char* nomeArquivoBIN){

    // Abrindo o arquivo binario no modo de escrita
    FILE *arquivoBIN = fopen(nomeArquivoBIN, "rb");
    if (arquivoBIN == NULL) {
        printf("Falha no processamento do arquivo.");
        return;
    }

    // Se seu status for igual a zero, o arquivo está inconsistente
    if(fgetc(arquivoBIN) == '0'){
        printf("Falha no processamento do arquivo.\n");
        fclose(arquivoBIN);
        return;
    }

    // Pula os 13 primeiros bytes do arquivo, referentes ao cabeçalho
    int byteOffset = 13;
    fseek(arquivoBIN, byteOffset, SEEK_SET);

    // Lendo todos os registros e escrevendo-os
    while(1){

        // Cria e lê um registro
        Registro *r = criaRegistro();
        r = leRegistro(byteOffset, r, arquivoBIN);

        // Se acabaram os registros, encerra o ciclo, senão imprime-o na tela
        if (r == NULL) break;
        else imprimeRegistro(r);
        
        // Desalocando a memória
        liberaRegistro(r);

        // Precisamos saltar até o próximo registro
        byteOffset += TAM_REGISTRO;
    }

    // Fechando o arquivo
    fclose(arquivoBIN);
}
