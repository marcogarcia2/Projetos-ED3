#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "funcionalidade1.h"
#include "registros.h"
#include "funcoesCriadas.h"
#include "funcoesFornecidas.h"

// Funcionalidade 1
void criaTabela(char *nomeArquivoCSV, char *nomeArquivoBIN){ 
    
    // Abrindo o arquivo CSV
    FILE *arquivoCSV = fopen(nomeArquivoCSV, "r");
    if(arquivoCSV == NULL){
        printf("Falha no carregamento do Arquivo CSV.\n");
        return;
    }

    // Criando um arquivo binário
    FILE *arquivoBIN = fopen(nomeArquivoBIN, "wb");
    if(arquivoBIN == NULL){
        printf("Falha no carregamento do Arquivo Binário.\n");
        return;
    }

    Cabecalho *cabecalho = criaCabecalho();
    // printf("Status: %c\n", cabecalho->status);
    // printf("proxRRN: %d\n", cabecalho->proxRRN);
    // printf("nroTecnologias: %d\n", cabecalho->nroTecnologias);
    // printf("nroParesTecnologias: %d\n", cabecalho->nroParesTecnologias);
    // Reservando os 13 primeiros bytes do arquivo para o cabecalho
    gravaCabecalho(cabecalho, arquivoBIN);

    // Descartando a primeira linha do arquivo (deve ser revisto para analisar a ordem das informações)
    fscanf(arquivoCSV, "%*[^\n]\n");
    
    Registro *r = criaRegistro();

    char str1[30], str2[30];

    // A lógica é criar um loop que irá ler cada linha do arquivo CSV
    while (1) {   
        int resultado = fscanf(arquivoCSV, "%[^,],%d,%d,%[^,],%d\n", str1, &r->grupo, &r->popularidade, str2, &r->peso); // Talvez nao de certo trabalhar com fscanf
        

        if (resultado == EOF){ // Chegou ao fim do arquivo
            break;
        }

        else if (resultado != 5) { // Significa que algum dos campos não foi lido
            // TRATAR CAMPOS NULOS
            //printf("%d  ", resultado);
            if(resultado == 0)
                printf("Erro Tec Origem\n");
            if (resultado == 1){
                printf("Erro Grupo\n");
                continue;
            }
            else if (resultado == 2){
                printf("Erro Popularidade\n");
                continue;
            }
            else if (resultado == 3){
                printf("Erro Tec Destino\n");
                continue;
            }
            else if (resultado == 4){
                printf("Erro Peso\n");
                continue;
            }
        }

        else{ // Leitura bem-sucedida

            r->tecnologiaOrigem.string = str1;
            r->tecnologiaOrigem.tamanho = strlen(str1);
            // buscar str1 em tecnologias, cabecalho->nroTecnologias++;

            r->tecnologiaDestino.string = str2;
            r->tecnologiaDestino.tamanho = strlen(str2);
            
            /*
            printf("r->tecnologiaOrigem.string = %s\n", r->tecnologiaOrigem.string);
            printf("r->tecnologiaOrigem.tamanho = %d\n\n", r->tecnologiaOrigem.tamanho);
            printf("r->tecnologiaDestino.string = %s\n", r->tecnologiaDestino.string);
            printf("r->tecnologiaDestino.tamanho = %d\n\n", r->tecnologiaDestino.tamanho);
            */

            // Escreve os valores lidos no arquivo binário
            gravaRegistro(r, arquivoBIN);
            r = resetaRegistro(r);
            cabecalho->proxRRN++; 
        }

    }

    //printf("%d ", cabecalho->proxRRN);
    


    
    // fseek para voltar ao inicio e gravar o cabecalho

    // 1) Alterando o status para '1' antes de fechar o binário (criar uma função pra isso, fazendo as demais atualizaçãoes necessárias)
    fseek(arquivoBIN, 0, SEEK_SET); // Ponteiro aponta para o inicio do arquivo
    //fwrite("1", sizeof(char), 1, arquivoBIN);
    cabecalho->status = '1';
    fputc(cabecalho->status, arquivoBIN);
    fwrite(&cabecalho->proxRRN, sizeof(int), 1, arquivoBIN);
    
    fclose(arquivoCSV);
    fclose(arquivoBIN);

    //free(r); // Aqui não precisa dar free() pois o registro não foi alocado dinamicamente
    free(cabecalho);

    binarioNaTela(nomeArquivoBIN);

}
