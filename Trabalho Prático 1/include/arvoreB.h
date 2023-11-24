/*  
*   Lucas Lima Romero - 13676325
*   Marco Antonio Gaspar Garcia - 11833581
*/

#ifndef ARVORE_B_H
#define ARVORE_B_H

#define STRING_TAM 55

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

// Função que cria um Nó de árvore B
No *criaNo(void);




#endif
