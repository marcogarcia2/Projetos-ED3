#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char const *argv[])
{
    FILE *arquivo;

    // Abra o arquivo para leitura
    arquivo = fopen("teste.csv", "r");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return 1;
    }

    char *texto = (char*) malloc(100 * sizeof(char));
    // Usando a função fgets para receber o texto do arquivo
    fgets(texto, 100, arquivo);

    const char delim[] = ",";
    char *token = strtok(texto, delim);

    // O objetivo aqui é ler o arquivo e tratar da maneira correta os campos nulos
    // A primeira tentativa é usar a função fgets para ler linha a linha dentro de um loop
        // A cada loop, uma linha será lida e tratada com a função strtok
    while (token != NULL) {
        printf("Token: %s\n", token);
        token = strtok(NULL, delim);
    }

    fgets(texto, 100, arquivo);
    char *token2 = strtok(texto, delim);
    while (token2 != NULL) {
        printf("Token: %s\n", token2);
        token2 = strtok(NULL, delim);
    }
    
    fgets(texto, 100, arquivo);
    char *token3 = strtok(texto, delim);
    while (token3 != NULL) {
        printf("Token: %s\n", token3);
        token3 = strtok(NULL, delim);
    }

    // while(1){ // Lendo todas as linhas do meu csv

    // }

    return 0;
}
