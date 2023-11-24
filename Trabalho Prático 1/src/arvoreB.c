/*  
*   Lucas Lima Romero - 13676325
*   Marco Antonio Gaspar Garcia - 11833581
*/

#include "arvoreB.h"

// Estrutura que define um Nó (ou página) de Árvore B
typedef struct _No{ 
    
    int nroChavesNo, alturaNo, RRNdoNo;

    int P[5];       
    int PR[4];
    char C[4][STRING_TAM];  // Tornar dinâmico posteriormente

    // De acordo com a especificação do projeto:
    // P1 == P[0]; P2 == P[1]; P3 == P[2]; P4 == P[3]
    // PR1 == PR[0] ...
    // C1 == C[0] ...
    // e assim por diante...
    // Os últimos elementos serão apenas para guardar temporariamente para fazer os splits
    
} No;

// Função que cria um Cabeçalho de Nó, inserido antes de cada nó
No *criaNo(void){

    // Alocando a memória dinamicamente
    No *no = (No*) malloc(sizeof(No));

    // Iniciando com valores nulos, precisam ser lidos da Árvore B
    no->nroChavesNo = -1;
    no->alturaNo = -1;
    no->RRNdoNo = -1;
    for (int i = 0; i < 4; i++){
        for(int j = 0; j < STRING_TAM; j++){

            // Preenchendo com lixo, para facilitar posteriormente a escrita
            no->C[i][j] = '$'; //LIXO???
        }
    }

    return no;
}
