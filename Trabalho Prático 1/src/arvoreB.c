/*  
*   Lucas Lima Romero - 13676325
*   Marco Antonio Gaspar Garcia - 11833581
*/

#include "arvoreB.h"

#define STRING_TAM 55

typedef struct _No{
    // Deve ter mais coisa, mas isso aqui é o que tem que ter pelo trabalho
    int P1, PR1;
    char C1[55]; // malloc ???

    int P2, PR2; // pelo que eu entendi, Px e PRx são os RRNs do arquivo binário normal e do arquivo de índice, respectivamente
    char C2[55]; // Strings concatenadas

    int P3, PR3;
    char C3[55];

    int P4;

} No;