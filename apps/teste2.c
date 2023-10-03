#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main(int argc, char const *argv[])
{
    
    char *str1 = (char *) malloc(30 *sizeof(char));
    char *str2 = (char *) malloc(30 *sizeof(char));
    //char *conc = (char *) malloc(60 *sizeof(char));
    
    scanf("%s %s", str1, str2);
    //conc = strcat(str1, str2);
    printf("%s | Tamanho: %d\n", strcat(str1, str2), 2);
    printf("%s %s\n", str1, str2);

    return 0;
}
