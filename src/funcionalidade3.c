/*  
*   Lucas Lima Romero - 13676325
*   Marco Antonio Gaspar Garcia - 11833581
*/

#define BYTE_GRUPO 1
#define BYTE_POPULARIDADE 5
#define BYTE_PESO 9

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "funcionalidade3.h"
#include "registros.h"
#include "funcoesCriadas.h"
#include "funcoesFornecidas.h"

// Funcionalidade 3: imprime o registro associado a um dado campo

void buscaPorCampo(char *nomeArquivoBIN, int n){ // Imprime os registros que possuem o campo dado
    Registro *r = criaRegistro(); // Criei o registro que irei retornar

    char nomeCampo[30], valorCampo[30];
    int valorCampoint;

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
            if(fgetc(arquivoBIN) == '1'){
                printf("Registro inexistente.\n");
                return;
            }

            if(!strcmp(nomeCampo, "grupo")){ // Se é grupo
                // Estou no byte 1
                int byteInicial = 13; // Guarda em que byte começa o registro, primeira vez = 13
                //printf("%lu", ftell(arquivoBIN)); // 14
                while(1){
                    if(fgetc(arquivoBIN) == '0'){ // Se não tiver removido
                        fread(&r->grupo, sizeof(int), 1, arquivoBIN); // Leio o grupo
                        if(r->grupo == valorCampoint){ // Achei um dos registros!
                            // Vou armazenar o registro em variáveis e printá-lo no terminal
                            r = leRegistro(arquivoBIN, byteInicial, r);
                            imprimeRegistro(r);
                            break; // imagine que seja só pra printar o primeiro, mas não ent tirarei esse break
                        }

                    }
                }
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

