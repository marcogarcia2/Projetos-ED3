#include <stdio.h>
#include <stdlib.h>

#include "registros.h"

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
