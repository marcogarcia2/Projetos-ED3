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
/*
void adicionaAresta(Grafo *grafo, Aresta *novaAresta, int posOrigem, int posDestino){
    // Adicionando a aresta
    int i = 0;
    while(1) {
        printf("iteracao %d\n", i++);
    
        Aresta *arestaAtual = grafo->vertices[posOrigem].arestaInicial;

        // Se a aresta já existe, não precisamos adicionar // (PRECISA?)
        if (!strcmp(novaAresta->tecDestino, arestaAtual->tecDestino)) {
            return;
        }
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

*/

//
void adicionaAresta(Grafo *grafo, Aresta *novaAresta, int posOrigem, int posDestino){
    // Adicionando a aresta
    Aresta **p = &(grafo->vertices[posOrigem].arestaInicial);
    while (*p != NULL && strcmp(novaAresta->tecDestino, (*p)->tecDestino) > 0) {
        p = &(*p)->prox;
    }
    novaAresta->prox = *p;
    *p = novaAresta;

    // Atualizando os atributos do vértice
    grafo->vertices[posOrigem].grau++;
    grafo->vertices[posOrigem].grauSaida++;
    grafo->vertices[posOrigem].numArestas++;
    grafo->vertices[posDestino].grau++;
    grafo->vertices[posDestino].grauEntrada++;

    printf("Aresta %s adicionada ao vértice %s na posição [%d]\n", novaAresta->tecDestino, grafo->vertices[posOrigem].tecnologia, posOrigem);
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
    Vertice* temp = realloc(grafo->vertices, (grafo->numVertices + 1) * sizeof(Vertice));
   
    if(temp == NULL) {
        // Falha na realocação de memória
        return;
    }

    grafo->vertices = temp;
    grafo->numVertices++;

    // Shiftando os vértices para a direita
    for (int i = grafo->numVertices - 1; i > local; i--){
        grafo->vertices[i] = grafo->vertices[i-1];
    }
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
    if (grafo->numVertices == 0) {
        insereGrafoVazio(r, grafo);
    }
    

    // Se o grafo não estiver vazio, temos mais alguns casos
    else{

        // Verificando se as tecnologias já estão no grafo
        int posOrigem = buscaBinariaGrafo(grafo, r->tecnologiaOrigem.string);
        int posDestino = buscaBinariaGrafo(grafo, r->tecnologiaDestino.string);

        // Caso o vértice de origem NÃO exista no grafo:
        if(strcmp(grafo->vertices[posOrigem].tecnologia, r->tecnologiaOrigem.string) != 0){

            // Criando um novo vértice
            Vertice *vertOrigem = criaVertice(r->tecnologiaOrigem.string, r->grupo); // VERIFICAR ISSO AQUI

            // inserimos o vértice no vetor, na posição correta
            adicionaVertice(grafo, vertOrigem, posOrigem);
            printf("Vértice Criado: %s [%d]\n", r->tecnologiaOrigem.string, posOrigem);
        }

        // Caso o vértice de destino NÃO exista no grafo:
        if(strcmp(grafo->vertices[posDestino].tecnologia, r->tecnologiaDestino.string) != 0){

            // Criando um novo vértice
            Vertice *vertDestino = criaVertice(r->tecnologiaDestino.string, r->grupo); // VERIFICAR ISSO AQUI

            // inserimos o vértice no vetor, na posição correta
            adicionaVertice(grafo, vertDestino, posDestino);
            printf("Vértice Criado: %s [%d]\n", r->tecnologiaDestino.string, posDestino);
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
    
    
            // Percorrendo a lista de arestas
            Aresta *a = grafo->vertices[i].arestaInicial;

            while (a != NULL){
                // Imprimindo os atributos do vértice
                printf("%s %d %d %d %d ", grafo->vertices[i].tecnologia, grafo->vertices[i].grupo, grafo->vertices[i].grauEntrada, grafo->vertices[i].grauSaida, grafo->vertices[i].grau);
                printf("%s %d", a->tecDestino, a->peso);
                printf("\n");
                a = a->prox;
            }
    
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
