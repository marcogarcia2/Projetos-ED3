/*  
*   Lucas Lima Romero - 13676325
*   Marco Antonio Gaspar Garcia - 11833581
*/

#include "grafo.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

//
typedef struct _aresta{
    char tecDestino[30];
    int peso;
    struct _aresta *prox;
} Aresta;

// 
typedef struct _vertice{
    char tecnologia[30];
    int grupo;
    int grau, grauEntrada, grauSaida;
    Aresta *arestaInicial;
    unsigned int numArestas;
} Vertice;

// Um grafo é composto de vértices
typedef struct _grafo{
    Vertice *vertices;
    unsigned int numVertices;
} Grafo;

// Função que cria uma aresta
Aresta *criaAresta(char *tecDestino, int peso){
    
    Aresta *a = (Aresta*) malloc(sizeof(Aresta));
    strcpy(a->tecDestino, tecDestino);
    a->peso = peso;
    a->prox = NULL;

    return a;
}

// Função que cria um vértice
Vertice *criaVertice(char *tecnologia, int grupo){

    Vertice *v = (Vertice*) malloc(sizeof(Vertice));
    strcpy(v->tecnologia, tecnologia);
    v->grupo = grupo;
    v->grau = 0;
    v->grauEntrada = 0;
    v->grauSaida = 0;
    v->arestaInicial = NULL;

    return v;
}

// Função que cria um grafo
Grafo *criaGrafo(void){

    Grafo *G = (Grafo*) malloc(sizeof(Grafo));

    G->vertices = NULL;
    G->numVertices = 0;
    
    return G;
}

void bubbleSort(Grafo *grafo){

    // Variável auxiliar
    Vertice aux;
    bool flag = false;

    // Percorre o vetor de vértices
    for (int i = 0; i < grafo->numVertices; i++){

        // Percorre o vetor de vértices
        for (int j = 0; j < grafo->numVertices - 1 - i; j++){

            // Se o vértice j vier depois do vértice j+1 na ordem alfabética, trocamos
            if (strcmp(grafo->vertices[j].tecnologia, grafo->vertices[j+1].tecnologia) > 0){
                aux = grafo->vertices[j];
                grafo->vertices[j] = grafo->vertices[j+1];
                grafo->vertices[j+1] = aux;
                flag = true;
            }
        }

        if (!flag) break;
    }
}

void insertionSort(Grafo *grafo) {

    // Variável auxiliar
    Vertice aux;
    int j;

    // Percorre o vetor de vértices
    for (int i = 1; i < grafo->numVertices; i++) {
        aux = grafo->vertices[i];
        j = i - 1;

        // Move os elementos do vetor[0..i-1], que são maiores que a chave, para uma posição à frente de sua posição atual
        while (j >= 0 && strcmp(grafo->vertices[j].tecnologia, aux.tecnologia) > 0) {
            grafo->vertices[j + 1] = grafo->vertices[j];
            j = j - 1;
        }
        grafo->vertices[j + 1] = aux;
    }
}

int escaneiaGrafo(Grafo *grafo, char *tec){

    int i;

    for (i = 0; i < grafo->numVertices; i++)
        if (!strcmp(grafo->vertices[i].tecnologia, tec))
            return i;

    return -1;
}

int buscaBinariaGrafoRecursiva(Grafo *grafo, int inf, int sup, char *chave){
    // Critério de parada é quando inf é igual, busca em vetor unitario ou parametros inf e sup invalidos
    if(inf >= sup)
        return inf;
    
    int meio = inf + (sup - inf) / 2;
    if (!strcmp(chave, grafo->vertices[meio].tecnologia))
        return meio;

    // Busca recursivamente na metade inferior ou superior
    return (strcmp(chave, grafo->vertices[meio].tecnologia) < 0) ? buscaBinariaGrafoRecursiva(grafo, inf, meio - 1, chave) : buscaBinariaGrafoRecursiva(grafo, meio + 1, sup, chave);
}

int buscaBinariaGrafo(Grafo *grafo, char *chave){
    return buscaBinariaGrafoRecursiva(grafo, 0, grafo->numVertices - 1, chave);
}

// Função que adiciona uma aresta a um vértice
void adicionaAresta(Grafo *grafo, Aresta *novaAresta, int posOrigem, int posDestino){

    // Adicionando a aresta
    while(1) {
        
        Aresta *arestaAtual = grafo->vertices[posOrigem].arestaInicial;

        // Se a aresta já existe, não precisamos adicionar // (PRECISA?)
        if (!strcmp(novaAresta->tecDestino, arestaAtual->tecDestino)) return;

        // Enquanto isso aqui der < 0, significa que estamos caminhando
        // No momento que der > 0, significa que encontramos o lugar para inserir
        else if (arestaAtual->prox != NULL || strcmp(novaAresta->tecDestino, arestaAtual->tecDestino) < 0){
            novaAresta->prox = arestaAtual->prox;
            arestaAtual->prox = novaAresta;
            break;
        }

        arestaAtual = arestaAtual->prox;
    }

    // Atualizando os atributos do vértice
    grafo->vertices[posOrigem].grau++;
    grafo->vertices[posOrigem].grauSaida++;
    grafo->vertices[posOrigem].numArestas++;
    grafo->vertices[posDestino].grau++;
    grafo->vertices[posDestino].grauEntrada++;
}

// Função que procura o local correto de um grafo para inserir o novo vértice
int procuraGrafo(Grafo *grafo, char *tecnologia){
    /////////////////////////// CONFERIRRRRR
    int i = 0;

    for(i = 0; i < grafo->numVertices; i++)
        if(strcmp(grafo->vertices[i].tecnologia, tecnologia) > 0)
            break;

    return i;
}

// Função que adiciona um vértice a um grafo
void adicionaVertice(Grafo *grafo, Vertice *v, int local){

    // Local é a posição no vetor em que o vértice deve ser inserido

    // Realocando memória para o novo vértice
    grafo->vertices = (Vertice*) realloc(grafo->vertices, (grafo->numVertices + 1) * sizeof(Vertice));
    grafo->numVertices++;

    // Shiftando os vértices para a direita
    for (int i = grafo->numVertices; i > local+1; i--)
        grafo->vertices[i] = grafo->vertices[i-1];

    // Inserindo o novo vértice
    grafo->vertices[local] = *v;


}

// Função que insere um registro em um grafo vazio
void insereGrafoVazio(Registro *r, Grafo *grafo){

    // Quando o grafo está vazio, iremos inserir dois novos vértices
    grafo->vertices = (Vertice*) malloc(2 * sizeof(Vertice));

    // Criando um novo vértice
    Vertice *v1 = criaVertice(r->tecnologiaOrigem.string, r->grupo);

    // Atribuindo o vértice ao grafo
    grafo->vertices[0] = *v1;
    grafo->numVertices++;

    // Antes de adicionar a aresta, precisamos criar um novo vértice, o da tecDestino

    // Verificando se a string não é nula
    if (!strcmp(r->tecnologiaDestino.string, "") || !strcmp(r->tecnologiaDestino.string, r->tecnologiaOrigem.string)) return;

    Vertice *v2 = criaVertice(r->tecnologiaDestino.string, r->grupo); // VERIFICAR ISSO AQUI
    
    grafo->vertices[1] = *v2;
    grafo->numVertices++;


    // Criando a aresta
    Aresta *aresta = criaAresta(r->tecnologiaDestino.string, r->peso);

    // Atualizando na mão os atributos
    grafo->vertices[0].arestaInicial = aresta;
    grafo->vertices[0].numArestas++;
    grafo->vertices[0].grau++;
    grafo->vertices[0].grauSaida++;
    grafo->vertices[1].grau++;
    grafo->vertices[1].grauEntrada++;

    // Entretanto, v2 pode vir antes de v1 na ordem alfabética, ordenemos (existe essa palavra?)
    if(strcmp(grafo->vertices[0].tecnologia, grafo->vertices[1].tecnologia) > 0){
        Vertice aux = grafo->vertices[0];
        grafo->vertices[0] = grafo->vertices[1];
        grafo->vertices[1] = aux;
    }
}

// Função que adiciona um registro todo a um grafo
void insereGrafo(Grafo *grafo, Registro *r){

    // Se tecnologiaOrigem for nula, não inserimos nada
    if (!strcmp(r->tecnologiaOrigem.string, "")) return;

    // Se não for, vamos inserir no vértice correspondente.

    // Se o grafo estiver vazio, precisamos criar um vértice novo
    if (grafo->numVertices == 0) insereGrafoVazio(r, grafo);

    // Se o grafo não estiver vazio, temos mais alguns casos
    else{
        
        // Verificando se as tecnologias já estão no grafo
        int posOrigem = buscaBinariaGrafo(grafo, r->tecnologiaOrigem.string);
        int posDestino = buscaBinariaGrafo(grafo, r->tecnologiaDestino.string);
        
        // Caso o vértice de destino NÃO exista no grafo:
        if(strcmp(grafo->vertices[posDestino].tecnologia, r->tecnologiaDestino.string) != 0){

            // Criando um novo vértice
            Vertice *v = criaVertice(r->tecnologiaDestino.string, r->grupo); // VERIFICAR ISSO AQUI

            // inserimos o vértice no vetor, na posição correta
            adicionaVertice(grafo, v, posDestino);
        }

        // Caso o vértice de origem NÃO exista no grafo:
        if(strcmp(grafo->vertices[posOrigem].tecnologia, r->tecnologiaOrigem.string) != 0){

            // Criando um novo vértice
            Vertice *v = criaVertice(r->tecnologiaOrigem.string, r->grupo); // VERIFICAR ISSO AQUI

            // inserimos o vértice no vetor, na posição correta
            adicionaVertice(grafo, v, posOrigem);
        }

        // Criando uma nova aresta
        Aresta *a = criaAresta(r->tecnologiaDestino.string, r->peso);
        adicionaAresta(grafo, a, posOrigem, posDestino);    
    }
}

// Função que imprime um grafo (nome tecnologia origem, grupo, grauEntrada, grauSaida, grau, nome tec destino e peso dele)
/* nomeTecnologia do elemento i, grupo do elemento i, grau de entrada do 
elemento  i,  grau  de  saída  do  elemento  i,  grau  do  elemento  i, 
nomeTecnologia do elemento j e peso do elemento j.*/
void imprimeGrafo(Grafo *grafo){
    
        // Percorrendo o vetor de vértices
        for (int i = 0; i < grafo->numVertices; i++){
    
            // Imprimindo os atributos do vértice
            printf("%s, %d, %d, %d, %d, ", grafo->vertices[i].tecnologia, grafo->vertices[i].grupo, grafo->vertices[i].grauEntrada, grafo->vertices[i].grauSaida, grafo->vertices[i].grau);
    
            // Percorrendo a lista de arestas
            Aresta *a = grafo->vertices[i].arestaInicial;
            while (a != NULL){
                printf("%s, %d", a->tecDestino, a->peso);
                a = a->prox;
            }
    
            printf("\n");
        }
}

// Função que destrói vértice
void destroiVertice(Vertice *v){

    Aresta *aux1 = v->arestaInicial;
    Aresta *aux2 = NULL;
    
    while(aux1 != NULL){
        aux2 = aux1;
        aux1 = aux1->prox;
        free(aux2);
    }

    //free(v);
}

// Função que destrói um grafo
void destroiGrafo(Grafo *grafo){

    for (int i = 0; i < grafo->numVertices; i++){
        destroiVertice(&grafo->vertices[i]);
    }

    free(grafo->vertices);
    free(grafo);
}


// void destroiGrafo(Grafo *grafo){

//     // Percorrendo o vetor de vértices
//     for (int i = 0; i < grafo->numVertices; i++){

//         // Percorrendo a lista de arestas
//         Aresta *a = grafo->vertices[i].arestaInicial;
//         while (a != NULL){
//             Aresta *aux = a;
//             a = a->prox;
//             free(aux);
//         }
//     }

//     // Liberando o vetor de vértices
//     free(grafo->vertices);

//     // Liberando o grafo
//     free(grafo);
// }


