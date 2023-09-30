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
    char linha[100];
    Lista *L = criaLista();

    // A lógica é criar um loop que irá ler cada linha do arquivo CSV
    while (fgets(linha, sizeof(linha), arquivoCSV)) { 

        Registro *r = criaRegistro();

        int i = 0; // Variável que percorre a linha

        // Serão cinco iterações, uma para ler cada campo
        for (int iter = 1; iter <= 5; iter++){

            int j = 0;            // Variável que percorre o campo
            char aux[100] = "";   // Variável que guardará os campos

            // Tratando os 4 primeiros campos
            if (iter != 5) {

                // Percorrendo a linha e atribuindo em aux
                while (linha[i] != ','){
                    aux[j++] = linha[i++];
                }
                i++; // Pula a vírgula

                if (iter == 1){

                    r->tecnologiaOrigem.tamanho = strlen(aux);
                    r->tecnologiaOrigem.string = (char*)malloc(r->tecnologiaOrigem.tamanho + 1);
                    strcpy(r->tecnologiaOrigem.string, aux);
                    r->tecnologiaOrigem.string[r->tecnologiaOrigem.tamanho] = '\0';
                    //printf("Tecnologia Origem: %s || Tamanho = %d\n", r->tecnologiaOrigem.string, r->tecnologiaOrigem.tamanho);
                }

                else if (iter == 2) {

                    r->grupo = aux[0] == '\0' ? -1 : atoi(aux);
                    //printf("Grupo =  %d\n", r->grupo);
                }

                else if (iter == 3) {

                    r->popularidade = aux[0] == '\0' ? -1 : atoi(aux);
                    //printf("Popularidade = %d\n", r->popularidade);
                }

                else if (iter == 4) {

                    r->tecnologiaDestino.tamanho = strlen(aux);
                    r->tecnologiaDestino.string = (char*)malloc(r->tecnologiaDestino.tamanho + 1);
                    strcpy(r->tecnologiaDestino.string, aux);
                    r->tecnologiaDestino.string[r->tecnologiaDestino.tamanho] = '\0';
                    //printf("Tecnologia Destino: %s || Tamanho = %d\n", r->tecnologiaDestino.string, r->tecnologiaDestino.tamanho);
                }
            }

            // iter == 5, deve ser tratada de forma diferente pois não termina em ','
            else {

                while(linha[i] != '\n' && linha[i] != EOF){
                    aux[j++] = linha[i++];
                }
                // nesse caso, se o campo peso estiver vazio, precisammos tratar de outra forma

                r->peso = aux[0] == '\0' ? -1 : atoi(aux);
                printf("Peso = %d\n", r->peso);
                printf("aux[0] = %c\n\n", aux[0]);
            }
        }

        gravaRegistro(r, arquivoBIN);
        cabecalho->proxRRN++;
        adicionaLista(L, r->tecnologiaOrigem.string, r->tecnologiaOrigem.tamanho);        
        adicionaLista(L, r->tecnologiaDestino.string, r->tecnologiaDestino.tamanho);        

        liberaRegistro(r);
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

    free(cabecalho);
    destroiLista(&L);

    binarioNaTela(nomeArquivoBIN);
}
