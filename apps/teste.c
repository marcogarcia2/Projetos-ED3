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
        // Use o strtok para separar a linha em tokens com base na vírgula
        //char *token;
        //char *rest = linha;
        
        char tecnologiaOrigem[100] = ""; // Inicialize com uma string vazia
        char tecnologiaDestino[100] = ""; // Inicialize com uma string vazia
        int grupo, popularidade, peso;
        grupo = popularidade = peso = -1; // Um valor inválido para indicar campo vazio
        
        char aux[100];
        int j = 0;
        for (int i = 0; i < 5; i++){
            int k = 0;
            while (linha[j] != ','){
                aux[k++] = linha[j++];
            }
            j++; // pular a linha

            if (i == 0) strcpy(tecnologiaOrigem, aux);
            else if (i == 1 && aux[0] != '\0') grupo = atoi(aux);
            else if (i == 2 && aux[0] != '\0') popularidade = atoi(aux);
            if (i == 3) strcpy(tecnologiaDestino, aux);
            else if (i == 4 && aux[0] != '\0') peso = atoi(aux);


        }
        
        
        
        /*
        token = strtok_r(rest, ",", &rest);
        if (token != NULL) strcpy(tecnologiaOrigem, token);

        token = strtok_r(rest, ",", &rest);
        if (token != NULL) grupo = atoi(token);

        token = strtok_r(rest, ",", &rest);
        if (token != NULL) popularidade = atoi(token);
        
        token = strtok_r(rest, ",", &rest);
        if (token != NULL) strcpy(tecnologiaDestino, token);

        token = strtok_r(rest, ",", &rest);
        if (token != NULL) peso = atoi(token);
        */

        // Agora, você tem 'tecnologiaOrigem' e 'grupo' preenchidos
        // e pode fazer algo com esses valores.
        printf("LINHA: %s\n\n", linha);
        printf("Tecnologia de Origem: %s\n", tecnologiaOrigem);
        printf("Grupo: %d\n", grupo);
        printf("Popularidade: %d\n", popularidade);
        printf("Tecnologia Destino: %s\n", tecnologiaDestino);
        printf("Peso: %d\n", peso);
    }

    // Feche o arquivo
    fclose(arquivo);

    return 0;
}
