#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main (int argc, char *argv[]){


    FILE *arquivo;

    char linha[100];
    

    // Abra o arquivo para leitura
    arquivo = fopen("teste.csv", "r");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return 1;
    }

    while (fgets(linha, sizeof(linha), arquivo)) {

        
        char *tecnologiaOrigem; // Inicialize com uma string vazia
        char *tecnologiaDestino; // Inicialize com uma string vazia
        int grupo, popularidade, peso, t1, t2;
        grupo = popularidade = peso = -1; // Um valor inválido para indicar campo vazio
        
        int j = 0; // percorre a linha
        for (int i = 0; i < 5; i++){

            int k = 0; // percorre cada campo
            char aux[100] = "";

            if (i != 4) {

                // Percorrendo a linha e alocando em aux
                while (linha[j] != ','){
                    aux[k++] = linha[j++];
                }
                j++; // pular a vírgula

                if (i == 0){

                    t1 = strlen(aux);
                    tecnologiaOrigem = (char*)malloc(t1+1);
                    strcpy(tecnologiaOrigem, aux);
                    tecnologiaOrigem[t1] = '\0';
                    printf("Tecnologia Origem: %s // Tamanho = %d\n", tecnologiaOrigem, t1);
                }

                else if (i == 1){

                    grupo = aux[0] == '\0' ? -1 : atoi(aux);
                    printf("Grupo: %d\n", grupo);
                }

                else if (i == 2){

                    popularidade = aux[0] == '\0' ? -1 : atoi(aux);
                    printf("Popularidade: %d\n", popularidade);
                }

                else if (i == 3){

                    t2 = strlen(aux);
                    tecnologiaDestino = (char*)malloc(t2+1);
                    strcpy(tecnologiaDestino, aux);
                    tecnologiaDestino[t2] = '\0';
                    printf("Tecnologia Destino: %s\nTamanho = %d\n", tecnologiaDestino, t2);
                }

            }
            // i == 4
            else {
                while(linha[j] != '\n' && linha[j] != EOF){
                    aux[k++] = linha[j++];
                }

                peso = aux[0] == '\0' ? -1 : atoi(aux);
                printf("Peso = %d\n\n\n", peso);
            }
        }
        
        free (tecnologiaDestino);
        free (tecnologiaOrigem);
    }

    // Feche o arquivo
    fclose(arquivo);

    return 0;
}
