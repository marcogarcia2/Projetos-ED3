/*  
*   Lucas Lima Romero - 13676325
*   Marco Antonio Gaspar Garcia - 11833581
*/

#ifndef LISTA_H
#define LISTA_H

// Estrutura que define o nó de uma lista
typedef struct _no No;

// Estrutura que define a lista
typedef struct _lista Lista;

// Funcao que cria um nó, dada uma string
No *criaNo(char *string, int tamString);

// Função que cria uma lista de nós
Lista *criaLista(void);

// Função que libera toda a memória
void destroiLista(Lista **L_ref);

// Função que adiciona elementos ao fim de uma lista
void adicionaLista(Lista *L, char *string, int tamString);

// Função que retorna a quantidade de Nós de uma lista
int getTamanho(const Lista *L);

// Função que exibe os elementos da lista
void imprimeLista(const Lista *L);

#endif
