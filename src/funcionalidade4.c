#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "funcionalidade4.h"
#include "registros.h"
#include "funcoesCriadas.h"

// Funcionalidade 4: imprimir o registro de um dado RRN

void buscaPorRRN(char *nomeArquivoBIN, int rrn){ // Imprime um registro após uma leitura num binário dado
    //Registro *r = criaRegistro(); // Criei o registro que irei retornar

    // Agora vou abrir o arquivo binário
    FILE *arquivoBIN = fopen(nomeArquivoBIN, "rb"); // Modo de leitura em binário
    if (arquivoBIN == NULL){ // Se o arquivo não existir, erro
        printf("Falha no processamento do arquivo.\n");
        return;
    }

    fseek(arquivoBIN, 1, SEEK_SET); // Posiciono meu cursor no byte offset 1, onde há a info de proxRRN do cabeçalho
    int ultimoRRN = 0; 
    fread(&ultimoRRN, sizeof(int), 1, arquivoBIN);
    ultimoRRN--;

    if(rrn > ultimoRRN || rrn < 0){
        printf("Registro inexistente.\n");
        return;
    }

    // printf("%d", ultimoRRN); // no tec.bin é pra dar 2 DEU!!

    int byteOffset = 13 + (rrn * TAM_REGISTRO); // Calculo o byte offset do registro que quero
    fseek(arquivoBIN, byteOffset, SEEK_SET); // Agora estou onde meu registro está

    // printf("Byte Offset: %d ", byteOffset);

    if(fgetc(arquivoBIN) == '1'){ // Se meu registro estiver removido, eu saio também, quero testar isso num futuro, removendo algum registro forçadamente
        printf("Registro inexistente.\n");
        return;
    }

    Registro *r = criaRegistro(); // Leio o registro

    // Agora vou ler o registro
    fread(&r->grupo, sizeof(int), 1, arquivoBIN);
    fread(&r->popularidade, sizeof(int), 1, arquivoBIN);
    fread(&r->peso, sizeof(int), 1, arquivoBIN);
    
    fread(&r->tecnologiaOrigem.tamanho, sizeof(int), 1, arquivoBIN);
    r->tecnologiaOrigem.string = (char *)malloc(r->tecnologiaOrigem.tamanho+1);
    fread(r->tecnologiaOrigem.string, r->tecnologiaOrigem.tamanho, 1, arquivoBIN);
    // printf("%d ", r->tecnologiaOrigem.tamanho);

    fread(&r->tecnologiaDestino.tamanho, sizeof(int), 1, arquivoBIN);
    r->tecnologiaDestino.string = (char *)malloc(r->tecnologiaDestino.tamanho+1);
    fread(r->tecnologiaDestino.string, r->tecnologiaDestino.tamanho, 1, arquivoBIN);
    // printf("%d\n", r->tecnologiaDestino.tamanho);

    r->tecnologiaOrigem.string[r->tecnologiaOrigem.tamanho] = '\0';
    r->tecnologiaDestino.string[r->tecnologiaDestino.tamanho] = '\0';

    (r->tecnologiaOrigem.tamanho != 0) ? printf("%s, ", r->tecnologiaOrigem.string) : printf("NULO, ");

    (r->grupo != -1) ? printf("%d, ", r->grupo) : printf("NULO, ");

    (r->popularidade != -1) ? printf("%d, ", r->popularidade) : printf("NULO, ");

    (r->tecnologiaDestino.tamanho != 0) ? printf("%s, ", r->tecnologiaDestino.string) : printf("NULO, ");

    (r->peso != -1) ? printf("%d\n", r->peso) : printf("NULO\n");

    //printf("%s, %d, %d, %s, %d", r->tecnologiaOrigem.string, r->grupo, r->popularidade, r->tecnologiaDestino.string, r->peso);

    free(r->tecnologiaOrigem.string);
    free(r->tecnologiaDestino.string);
    free(r);

    fclose(arquivoBIN); // Fechando o arquivo
}

