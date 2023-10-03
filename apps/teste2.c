#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void readline(char* string){
    char c = 0;

    do{
        c = (char) getchar();

    } while(c == '\n' || c == '\r');

    int i = 0;

    do{
        string[i] = c;
        i++;
        c = getchar();
    } while(c != '\n' && c != '\r');

    string[i]  = '\0';
}

int main(int argc, char const *argv[])
{
    FILE *arquivo;

    // Abra o arquivo para leitura
    arquivo = fopen("teste.csv", "r");
    // Testando a função readline
    

    return 0;
}
