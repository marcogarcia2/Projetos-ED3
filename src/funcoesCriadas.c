/*  
*   Lucas Lima Romero - 13676325
*   Marco Antonio Gaspar Garcia - 11833581
*/

#include <stdio.h>
#include <stdlib.h>

#include "registros.h"
#include "funcoesCriadas.h"

Cabecalho *criaCabecalho(void){ // Inicializa o cabecalho
    
    Cabecalho *c = (Cabecalho *) malloc(sizeof(Cabecalho));
    c->status = '0';
    c->proxRRN = 0;
    c->nroTecnologias = 0;
    c->nroParesTecnologias = 0;

    return c;
}

// Gravando o cabecalho em 13 bytes
void gravaCabecalho(Cabecalho *c, FILE *arquivoBIN){
    fseek(arquivoBIN, 0, SEEK_SET);
    fwrite(&c->status, sizeof(char), 1, arquivoBIN);
    fwrite(&c->proxRRN, sizeof(int), 1, arquivoBIN);
    fwrite(&c->nroTecnologias, sizeof(int), 1, arquivoBIN);
    fwrite(&c->nroParesTecnologias, sizeof(int), 1, arquivoBIN);
}

Registro *criaRegistro(void){ // Aloca-se memória para um registro
    Registro *r = (Registro *) malloc(sizeof(Registro));

    r->removido = '0';
    r->grupo = -1;
    r->popularidade = -1;
    r->peso = -1;

    r->tecnologiaOrigem.string = NULL;
    r->tecnologiaOrigem.tamanho = 0; 

    r->tecnologiaDestino.string = NULL;
    r->tecnologiaDestino.tamanho = 0; 
    
    return r;
}

Registro *resetaRegistro(Registro *r){ // Será chamada para resetar os parâmetros do registro
    r->grupo = -1;
    r->popularidade = -1;
    r->peso = -1;

    r->tecnologiaOrigem.string = NULL;
    r->tecnologiaOrigem.tamanho = 0; 

    r->tecnologiaDestino.string = NULL;
    r->tecnologiaDestino.tamanho = 0;

    return r;
}

void liberaRegistro(Registro *r){ // Libera a memória alocada para o registro

    free(r->tecnologiaOrigem.string);
    free(r->tecnologiaDestino.string);
    free(r);

    return;
}

void gravaRegistro(Registro *r, FILE *arquivoBIN){ // grava o Registro criado no arquivo binário
    fwrite(&r->removido, sizeof(char), 1, arquivoBIN);
    fwrite(&r->grupo, sizeof(int), 1, arquivoBIN);
    fwrite(&r->popularidade, sizeof(int), 1, arquivoBIN);
    fwrite(&r->peso, sizeof(int), 1, arquivoBIN);
    fwrite(&r->tecnologiaOrigem.tamanho, sizeof(int), 1, arquivoBIN);
    fwrite(r->tecnologiaOrigem.string, r->tecnologiaOrigem.tamanho, 1, arquivoBIN);
    fwrite(&r->tecnologiaDestino.tamanho, sizeof(int), 1, arquivoBIN);
    fwrite(r->tecnologiaDestino.string, r->tecnologiaDestino.tamanho, 1, arquivoBIN);

    //Colocando o lixo nos caracteres restantes
    // char *lixo = (char*)'$';
    // int restante = TAM_REGISTRO - (TAM_REGISTRO_FIXO + r->tecnologiaOrigem.tamanho + r->tecnologiaDestino.tamanho);
    // while (restante > 0){
    //     fwrite(&lixo, sizeof(char), 1, arquivoBIN);
    //     restante--;
    // }

    int tamLixo = TAM_REGISTRO - (TAM_REGISTRO_FIXO + r->tecnologiaOrigem.tamanho + r->tecnologiaDestino.tamanho);
    for(int i = 0; i < tamLixo; i++)
        fwrite("$", sizeof(char), 1, arquivoBIN);

}

Registro *leRegistro(FILE *arquivoBIN, int byteInicial, Registro *r){ // Função que lê o registro do arquivo binário
    // O byteInicial é o byte do campo removido
    fseek(arquivoBIN, byteInicial, SEEK_SET); // Vou para o byte inicial do registro

    r->removido = fgetc(arquivoBIN);
    fread(&r->grupo, sizeof(int), 1, arquivoBIN);
    fread(&r->popularidade, sizeof(int), 1, arquivoBIN);
    fread(&r->peso, sizeof(int), 1, arquivoBIN);

    fread(&r->tecnologiaOrigem.tamanho, sizeof(int), 1, arquivoBIN);
    r->tecnologiaOrigem.string = (char *)malloc(r->tecnologiaOrigem.tamanho+1);
    fread(r->tecnologiaOrigem.string, r->tecnologiaOrigem.tamanho, 1, arquivoBIN);

    fread(&r->tecnologiaDestino.tamanho, sizeof(int), 1, arquivoBIN);
    r->tecnologiaDestino.string = (char *)malloc(r->tecnologiaDestino.tamanho+1);
    fread(r->tecnologiaDestino.string, r->tecnologiaDestino.tamanho, 1, arquivoBIN);

    r->tecnologiaOrigem.string[r->tecnologiaOrigem.tamanho] = '\0';
    r->tecnologiaDestino.string[r->tecnologiaDestino.tamanho] = '\0';

    // O ponteiro do arquivo está no final do registro
    //printf("%lu\n", ftell(arquivoBIN)); // Teste pra ver se o ponteiro está no final do registro

    return r;
}

void imprimeRegistro(Registro *r){
    (r->tecnologiaOrigem.tamanho != 0) ? printf("%s, ", r->tecnologiaOrigem.string) : printf("NULO, ");
    (r->grupo != -1) ? printf("%d, ", r->grupo) : printf("NULO, ");
    (r->popularidade != -1) ? printf("%d, ", r->popularidade) : printf("NULO, ");
    (r->tecnologiaDestino.tamanho != 0) ? printf("%s, ", r->tecnologiaDestino.string) : printf("NULO, ");
    (r->peso != -1) ? printf("%d\n", r->peso) : printf("NULO\n");
}
