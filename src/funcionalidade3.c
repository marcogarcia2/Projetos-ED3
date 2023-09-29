#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "funcionalidade4.h"
#include "registros.h"
#include "funcoesCriadas.h"
#include "funcoesFornecidas.h"

// Funcionalidade 3: imprime o registro associado a um dado campo

void buscaPorCampo(char *nomeArquivoBIN, int n){ // Imprime os registros que possuem o campo dado
    //Registro *r = criaRegistro(); // Criei o registro que irei retornar

    char nomeCampo[30], valorCampo[30];
    int valorCampoint;
    Registro *r = criaRegistro();

    // Agora vou abrir o arquivo binário
    FILE *arquivoBIN = fopen(nomeArquivoBIN, "rb"); // Modo de leitura em binário
    if (arquivoBIN == NULL){ // Se o arquivo não existir, erro
        printf("Falha no processamento do arquivo.\n");
        return;
    }

    for (int i = 0; i < n; i++){
        scanf("%s", nomeCampo); // Leitura do nome do campo
        if(strcmp(nomeCampo, "nomeTecnologiaOrigem") == 0 || strcmp(nomeCampo, "nomeTecnologiaDestino") == 0){ // Se forem iguais utiliza-se a função de leitura de string com aspas
            scan_quote_string(valorCampo); // Leitura de um valor de campo
            printf("nomeCampo: %s\nValor Campo: %s\n", nomeCampo, valorCampo);

            if(strcmp(nomeCampo, "nomeTecnologiaOrigem") == 0){
                
            }else{

            }

        }
        else{ // Caso não for nenhuma das duas strings, o próximo valor é um inteiro
            scanf("%d", &valorCampoint);
            printf("nomeCampo: %s\nValor Campo: %d\n", nomeCampo, valorCampoint);

            fseek(arquivoBIN, 13, SEEK_SET); // Pula o cabeçalho
            if(fgetc(arquivoBIN) == '1'){ // Se o arquivo estiver inconsistente
                printf("Registro inexistente.\n");
                return;
            }

            if(!strcmp(nomeCampo, "grupo")){ // Se é grupo
                
            }
            else if(!strcmp(nomeCampo, "popularidade")){ // Se é popularidade
                
            }
            else if(!strcmp(nomeCampo, "peso")){ // Se é peso
                
            }
            else{
                printf("ERRO! Esse campo não existe.\n");
                return;
            }
            


        }

    }

    // liberaRegistro(r);
    fclose(arquivoBIN); // Fechando o arquivo
}

