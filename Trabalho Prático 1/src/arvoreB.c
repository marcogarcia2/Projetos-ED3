/*  
*   Lucas Lima Romero - 13676325
*   Marco Antonio Gaspar Garcia - 11833581
*/

#include "arvoreB.h"

// Função que cria um Cabeçalho de Nó, inserido antes de cada nó
Node *criaNode(void){

    // Alocando a memória dinamicamente
    Node *node = (Node*) malloc(sizeof(Node));

    // Iniciando com valores nulos, precisam ser lidos da Árvore B
    node->nroChavesNo = -1;
    node->alturaNo = -1;
    node->RRNdoNo = -1;
    for (int i = 0; i < 4; i++){
        for(int j = 0; j < STRING_TAM; j++){

            // Preenchendo com lixo, para facilitar posteriormente a escrita
            node->C[i][j] = '$'; //LIXO???
        }
    }

    return node;
}

