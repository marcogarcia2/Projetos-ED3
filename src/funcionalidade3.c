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
#include "funcoesFornecidas.h"

// Funcionalidade 3: imprime os registros associados a um dado campo e seu respectivo valor

void buscaPorCampo(char *nomeArquivoBIN, int n){
    
    // Variáveis para a leitura da entrada
    char nomeCampo[30], valorCampo[30];
    int valorCampoint;

    // Flag que indica se encontrou algum registro
    int flagEncontrouRegistro; 

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

    // 13 + (ultimoRRN * TAM_REGISTRO) é o número total de bytes deste arquivo, será a condição de parada
    int tamTotal = 13 + (ultimoRRN * TAM_REGISTRO);

    // Loop mestre, n é dado pela entrada
    for (int i = 0; i < n; i++){

        flagEncontrouRegistro = 0; // Inicializo a flag como 0
        scanf("%s", nomeCampo); // Leitura do nome do campo

        /// O tratamento será diferente, se o campo for string ou int

        // Se o campo for string: utilizaremos a função fornecida scan_quote_string
        if(!strcmp(nomeCampo, "nomeTecnologiaOrigem") || !strcmp(nomeCampo, "nomeTecnologiaDestino")){ 
            
            scan_quote_string(valorCampo); // Leitura de um valor de campo
            fseek(arquivoBIN, 13, SEEK_SET); // Pula o cabeçalho

            // Caso 1: campo escolhido é nomeTecnologiaOrigem
            if(!strcmp(nomeCampo, "nomeTecnologiaOrigem")){
                
                // Guarda em que byte começa o registro, primeira vez = 13
                unsigned long byteOffset = ftell(arquivoBIN);

                // Fazendo uma busca sequencial em todo o arquivo binário
                while(byteOffset < tamTotal){ 

                    // Só leremos se o registro não tiver sido removido
                    if(fgetc(arquivoBIN) == '0'){

                        // Pula os campos grupo, popularidade e peso (3 ints)
                        fseek(arquivoBIN, 3 * sizeof(int), SEEK_CUR);

                        // Criando um registro para auxiliar na leitura e impressão
                        Registro *r = criaRegistro(); 

                        // Lendo o tamanho da string e a própria string desejada
                        fread(&r->tecnologiaOrigem.tamanho, sizeof(int), 1, arquivoBIN);
                        r->tecnologiaOrigem.string = (char *)malloc(r->tecnologiaOrigem.tamanho+1);
                        fread(r->tecnologiaOrigem.string, r->tecnologiaOrigem.tamanho, 1, arquivoBIN);
                        
                        // Comparando a string com o valorCampo. Se true, encontramos um registro!
                        if(!strcmp(r->tecnologiaOrigem.string, valorCampo)){
                            
                            free(r->tecnologiaOrigem.string);
                            // Armazenando o registro em variáveis e imprimindo-o
                            r = leRegistro(byteOffset, r, arquivoBIN);
                            imprimeRegistro(r);
                            flagEncontrouRegistro = 1;
                        }

                        // Se não encontrou, preciso dar fseek pro próximo registro
                        else fseek(arquivoBIN, TAM_REGISTRO - 17 - r->tecnologiaOrigem.tamanho, SEEK_CUR);


                        byteOffset += TAM_REGISTRO;
                        liberaRegistro(r);
                    }
                }

                // Caso depois de rodar todo o while eu não achar nenhum grupo (flagEncontrou = 0), eu printo
                if(!flagEncontrouRegistro) printf("Registro inexistente.\n");
            }
            
            // Caso 2: campo escolhido é nomeTecnologiaDestino
            else if (!strcmp(nomeCampo, "nomeTecnologiaDestino")){
                
                // Guarda em que byte começa o registro, primeira vez = 13
                unsigned long byteOffset = ftell(arquivoBIN);

                // Fazendo uma busca sequencial em todo o arquivo binário
                while(byteOffset < tamTotal){ 

                    // Só leremos se o registro não tiver sido removido
                    if(fgetc(arquivoBIN) == '0'){ 

                        // Pula os campo grupos grupo, popularidade e peso (3 ints)
                        fseek(arquivoBIN, 3 * sizeof(int), SEEK_CUR);

                        // Criando um registro para auxiliar na leitura e impressão
                        Registro *r = criaRegistro();

                        // Lendo um int de 4 bytes, e pulando a string com o valor do int
                        fread(&r->tecnologiaOrigem.tamanho, sizeof(int), 1, arquivoBIN);
                        fseek(arquivoBIN, r->tecnologiaOrigem.tamanho, SEEK_CUR);

                        // Lendo o tamanho da string e a própria string desejada
                        fread(&r->tecnologiaDestino.tamanho, sizeof(int), 1, arquivoBIN);
                        r->tecnologiaDestino.string = (char *)malloc(r->tecnologiaDestino.tamanho+1);
                        fread(r->tecnologiaDestino.string, r->tecnologiaDestino.tamanho, 1, arquivoBIN);
                        
                        // Comparando a string com o valorCampo. Se true, encontramos um registro!
                        if(!strcmp(r->tecnologiaDestino.string, valorCampo)){
                            
                            free(r->tecnologiaDestino.string);
                            // Armazenando o registro em variáveis e imprimindo-o
                            r = leRegistro(byteOffset, r, arquivoBIN);
                            imprimeRegistro(r);
                            flagEncontrouRegistro = 1;
                        }

                        // Se não encontrou, preciso dar fseek pro próximo registro
                        else fseek(arquivoBIN, TAM_REGISTRO - 21 - r->tecnologiaOrigem.tamanho - r->tecnologiaDestino.tamanho, SEEK_CUR);
                        

                        byteOffset += TAM_REGISTRO;
                        liberaRegistro(r);
                    }
                }

                // Caso depois de rodar todo o while eu não achar nenhum grupo (flagEncontrou = 0), eu printo
                if(!flagEncontrouRegistro) printf("Registro inexistente.\n");
            }
        }

        // Caso não for nenhuma das duas strings, o valor desejado é um inteiro
        else {

            scanf("%d", &valorCampoint); // Escaneando seu valor
            fseek(arquivoBIN, 13, SEEK_SET); // Pula o cabeçalho

            // Caso 3: Grupo
            if(!strcmp(nomeCampo, "grupo")){ 

                // Guarda em que byte começa o registro, primeira vez = 13
                unsigned long byteOffset = ftell(arquivoBIN);

                // Fazendo uma busca sequencial no arquivo binário
                while(byteOffset < tamTotal){
                    
                    // Só leremos se não estiver removido
                    if(fgetc(arquivoBIN) == '0'){ 
                        
                        // Criando um registro para auxiliar na leitura e impressão
                        Registro *r = criaRegistro();

                        // Lendo o grupo
                        fread(&r->grupo, sizeof(int), 1, arquivoBIN);

                        // Comparando o int com grupo. Se true, encontramos um registro!
                        if(r->grupo == valorCampoint){

                            // Armazenando o registro em variáveis e imprimindo-o
                            r = leRegistro(byteOffset, r, arquivoBIN);
                            imprimeRegistro(r);
                            flagEncontrouRegistro = 1;
                        }

                        // Se não encontrou, preciso dar fseek pro próximo registro
                        else fseek(arquivoBIN, TAM_REGISTRO - 5, SEEK_CUR);


                        byteOffset += TAM_REGISTRO;
                        liberaRegistro(r);
                    }
                }

                // Caso depois de rodar todo o while eu não achar nenhum grupo (flagEncontrou = 0), eu printo
                if(!flagEncontrouRegistro) printf("Registro inexistente.\n");
            }

            // Caso 4: popularidade
            else if(!strcmp(nomeCampo, "popularidade")){

                // Guarda em que byte começa o registro, primeira vez = 13
                long byteOffset = ftell(arquivoBIN);

                // Fazendo uma busca sequencial no arquivo binário
                while(byteOffset < tamTotal){ 

                    // Só leremos se não estiver removido
                    if(fgetc(arquivoBIN) == '0'){ 

                        // Pulando o campo grupo (1 int)
                        fseek(arquivoBIN, sizeof(int), SEEK_CUR);

                        // Criando um registro para auxiliar na leitura e impressão
                        Registro *r = criaRegistro();

                        // Lendo a popularidade
                        fread(&r->popularidade, sizeof(int), 1, arquivoBIN);

                        // Comparando o int com o grupo. Se true, encontramos um registro!
                        if(r->popularidade == valorCampoint){

                            // Armazenando o registro em variáveis e imprimindo-o
                            r = leRegistro(byteOffset, r, arquivoBIN);
                            imprimeRegistro(r);
                            flagEncontrouRegistro = 1;
                        }

                        // Se não encontrou, preciso dar fseek pro próximo registro
                        else fseek(arquivoBIN, TAM_REGISTRO - 9, SEEK_CUR);
                        

                        byteOffset += TAM_REGISTRO;
                        liberaRegistro(r);
                    }
                }

                // Caso depois de rodar todo o while eu não achar nenhum grupo (flagEncontrou = 0), eu printo
                if(!flagEncontrouRegistro) printf("Registro inexistente.\n");
            }

            // Caso 5: peso
            else if(!strcmp(nomeCampo, "peso")){ 

                // Guarda em que byte começa o registro, primeira vez = 13
                long byteOffset = ftell(arquivoBIN); 

                // Fazendo uma busca sequencial no arquivo binário
                while(byteOffset < tamTotal){ 

                    // Só leremos se não estiver removido
                    if(fgetc(arquivoBIN) == '0'){ 
                        // Pula o campo grupo e popularidade (2 ints)
                        fseek(arquivoBIN, 2 * sizeof(int), SEEK_CUR);

                        // Criando um registro para auxiliar na leitura e impressão
                        Registro *r = criaRegistro();

                        // Lendo o peso
                        fread(&r->peso, sizeof(int), 1, arquivoBIN);

                        // Comparando o int com o grupo. Se true, encontramos um registro!
                        if(r->peso == valorCampoint){ 

                            // Armazenando o registro em variáveis e imprimindo-o
                            r = leRegistro(byteOffset, r, arquivoBIN);
                            imprimeRegistro(r);
                            flagEncontrouRegistro = 1;
                        }

                        // Se não encontrou, preciso dar fseek pro próximo registro
                        else fseek(arquivoBIN, TAM_REGISTRO - 13, SEEK_CUR);

                        
                        byteOffset += TAM_REGISTRO;
                        liberaRegistro(r);
                    }
                }

                // Caso depois de rodar todo o while eu não achar nenhum grupo (flagEncontrou = 0), eu printo
                if(!flagEncontrouRegistro) printf("Registro inexistente.\n");
                
            }

            else{
                printf("ERRO! Esse campo não existe.\n");
                return;
            }

        }
        
        // Volto para o início do arquivo
        fseek(arquivoBIN, 0, SEEK_SET);
    
    
    } // fim loop mestre


    // Fechando o arquivo
    fclose(arquivoBIN); 
}

