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

// Funcionalidade 4: imprimir o registro de um dado RRN

void buscaPorRRN(char *nomeArquivoBIN, int rrn){ 

    // Abrindo o arquivo binário
    FILE *arquivoBIN = fopen(nomeArquivoBIN, "rb"); // Modo de leitura em binário
    if (arquivoBIN == NULL){ // Se o arquivo não existir, erro
        printf("Falha no processamento do arquivo.\n");
        return;
    }
    
    // Se ele está inconsistente, encerra-se a função
    if(fgetc(arquivoBIN) == '0'){
        printf("Falha no processamento do arquivo.\n");
        fclose(arquivoBIN);
        return;
    }

    // Descobrindo qual é o último RRN deste arquivo
    int ultimoRRN = 0; 
    fread(&ultimoRRN, sizeof(int), 1, arquivoBIN);
    ultimoRRN--;

    // Se a entrada de RRN não existir no arquivo, encerra-se a função
    if(rrn > ultimoRRN || rrn < 0){
        printf("Registro inexistente.\n");
        return;
    }

    // Calculando o byte offset do registro desejado
    int byteOffset = 13 + (rrn * TAM_REGISTRO); 

    // Apontando o ponteiro para o início do registro desejado
    fseek(arquivoBIN, byteOffset, SEEK_SET); 
    
    // Se este registro estiver removido, encerra-se a função
    if(fgetc(arquivoBIN) == '1'){ 
        printf("Registro inexistente.\n");
        return;
    }

    // Finalmente: Criando, lendo, printando e desalocando o registro
    Registro *r = criaRegistro(); 
    r = leRegistro(byteOffset, r, arquivoBIN);
    imprimeRegistro(r);
    liberaRegistro(r);

    // Fechando o arquivo binário
    fclose(arquivoBIN);
}
