/*  
*   Lucas Lima Romero - 13676325
*   Marco Antonio Gaspar Garcia - 11833581
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "funcionalidade2.h"
#include "registros.h"
#include "funcoesCriadas.h"

// Funcionalidade 2: imprimir todo o arquivo binário na tela
void recuperaDados(const char* nomeArquivoBIN){

    FILE *arquivoBIN = fopen(nomeArquivoBIN, "rb");
    if (arquivoBIN == NULL) {
        printf("Falha no carregamento do Arquivo Binário.");
        return;
    }

    // Pular os bytes do cabecalho // fseek????????
    char aux[13];
    fread(aux, 1, 13, arquivoBIN);

    // Lendo todos os registros e escrevendo-os
    while(1){

        Registro *r = criaRegistro();
        
        // O byte do campo removido é o primeiro a ser lido, servirá de flag para saber se chegou ao fim do arquivo
        int byte = fread(&r->removido, sizeof(char), 1, arquivoBIN);

        // Se o fread não leu nada, chegamos ao fim do arquivo
        if (byte == 0) {
            free(r);
            break;
        }

        // Se este não é o fim do arquivo, leremos todos os campos na ordem em que foram escritos

        fread(&r->grupo, sizeof(int), 1, arquivoBIN);
        fread(&r->popularidade, sizeof(int), 1, arquivoBIN);
        fread(&r->peso, sizeof(int), 1, arquivoBIN);

        // para ler as strings, é necessário alocar memória 

        fread(&r->tecnologiaOrigem.tamanho, sizeof(int), 1, arquivoBIN);
        r->tecnologiaOrigem.string = (char *)malloc(r->tecnologiaOrigem.tamanho+1);
        fread(r->tecnologiaOrigem.string, r->tecnologiaOrigem.tamanho, 1, arquivoBIN);

        fread(&r->tecnologiaDestino.tamanho, sizeof(int), 1, arquivoBIN);
        r->tecnologiaDestino.string = (char *)malloc(r->tecnologiaDestino.tamanho+1);
        fread(r->tecnologiaDestino.string, r->tecnologiaDestino.tamanho, 1, arquivoBIN);
        
        // Escrevendo na ordem correta e tratando os casos nulos
        r->tecnologiaOrigem.string[r->tecnologiaOrigem.tamanho] = '\0';
        r->tecnologiaDestino.string[r->tecnologiaDestino.tamanho] = '\0';

        // Campo 1: Tecnologia Origem (string)
        if (r->tecnologiaOrigem.tamanho > 0) printf("%s, ", r->tecnologiaOrigem.string);
        else printf("NULO, ");

        // Campo 2: Grupo (int)
        if (r->grupo != -1) printf("%d, ", r->grupo);
        else printf("NULO, ");

        // Campo 3: Popularidade (int)
        if(r->popularidade != -1) printf("%d, ", r->popularidade);
        else printf("NULO, ");

        // Campo 4: Tecnologia Destino (string)
        if (r->tecnologiaDestino.tamanho > 0) printf("%s, ", r->tecnologiaDestino.string);
        else printf("NULO, ");

        // Campo 5: Peso (int)
        if(r->peso != -1) printf("%d\n", r->peso);
        else printf("NULO\n");

        
        // Precisamos saltar até o próximo registro, calculando o quanto de lixo ainda resta
        int restante = TAM_REGISTRO - (TAM_REGISTRO_FIXO + r->tecnologiaOrigem.tamanho + r->tecnologiaDestino.tamanho);
        char lixos[restante];
        fread(lixos, 1, restante, arquivoBIN);
    
        liberaRegistro(r);
    }

    fclose(arquivoBIN);
}
