/*  
*   Lucas Lima Romero - 13676325
*   Marco Antonio Gaspar Garcia - 11833581
*/

#ifndef ARVORE_B_H
#define ARVORE_B_H

#define M 4
#define STRING_TAM 55

// Estrutura que define um Nó (ou página) de Árvore B
typedef struct _no_arv {
    int nroChavesNo, alturaNo, RRNdoNo;
    char *C[M+1]; // chaves de busca  // a 0 não é utilizada, e a 4 usamos temporariamente para o split
                                      // portanto o número máximo de chaves por nó é 3
    int P[M], PR[M-1];                // P guarda o RRN do arquivo de índice do nó filho
                                      // R guarda o RRN da chave no arquivo binário anterior
    struct _no_arv *ligacoes[M+1];
} NoArv;

// Estrutura do Cabeçalho do Arquivo de Índice da Árvore-B
typedef struct _cab_indice{
    char status;
    int noRaiz;
    int RRNproxNo;
} CabecalhoIndice;

// Função que insere uma chave em uma Árvore B
NoArv *inserirChave(char *C, NoArv *raiz);

// Função que busca uma chave em uma Árvore B
void buscarChave(char *C, int *pos, NoArv *no);

// Função que imprime todos as chaves da Árvore B em ordem alfabética
void imprimeArvoreB(NoArv *no);

// Função que desaloca a memória de uma Árvore B
void destroiArvoreB(NoArv *no);

#endif
