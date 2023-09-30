#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*  
*   Lucas Lima Romero - 13676325
*   Marco Antonio Gaspar Garcia - 11833581
*/

#include "funcionalidade1.h"
#include "registros.h"
#include "funcoesCriadas.h"
#include "funcoesFornecidas.h"
#include "lista.h"

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
    gravaCabecalho(cabecalho, arquivoBIN);

    // Descartando a primeira linha do arquivo (deve ser revisto para analisar a ordem das informações)
    fscanf(arquivoCSV, "%*[^\n]\n");
    
    // Variáveis que nos auxiliarão na análise do arquivo
    Registro *r = criaRegistro();
    char str1[30], str2[30];
    Lista *L = criaLista();

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

            r->tecnologiaDestino.string = str2;
            r->tecnologiaDestino.tamanho = strlen(str2);

            // Escreve os valores lidos no arquivo binário
            gravaRegistro(r, arquivoBIN);
            r = resetaRegistro(r);
            cabecalho->proxRRN++; 
            
            adicionaLista(L, str1, strlen(str1));
        }
    }

    // nroTecnologias é == o tamanho da lista
    cabecalho->nroTecnologias = getTamanho(L);
    //printf("%d ", cabecalho->proxRRN);
    
    //imprimeLista(L);
    printf("Número de tecnologias: %d\n", getTamanho(L));

    
    // fseek para voltar ao inicio e gravar o cabecalho

    // 1) Alterando o status para '1' antes de fechar o binário (criar uma função pra isso, fazendo as demais atualizaçãoes necessárias)
    fseek(arquivoBIN, 0, SEEK_SET); // Ponteiro aponta para o inicio do arquivo
    //fwrite("1", sizeof(char), 1, arquivoBIN);
    cabecalho->status = '1';
    fputc(cabecalho->status, arquivoBIN);
    fwrite(&cabecalho->proxRRN, sizeof(int), 1, arquivoBIN);

    

    
    fclose(arquivoCSV);
    fclose(arquivoBIN);

    liberaRegistro(r); // Aqui não precisa dar free() pois o registro não foi alocado dinamicamente
    free(cabecalho);
    destroiLista(&L);

    binarioNaTela(nomeArquivoBIN);

}
