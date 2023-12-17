/*  
*   Lucas Lima Romero - 13676325
*   Marco Antonio Gaspar Garcia - 11833581
*/

#include "grafo.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// <----------------------- FUNÇÕES DE CRIAÇÃO -----------------------> //

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


// <----------------------- FUNÇÕES DE INSERÇÃO -----------------------> //

// Busca binária no vetor de vértices
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

// Função que adiciona uma aresta a um grafo na posição correta para manter a ordem alfabética das arestas
void adicionaAresta(Grafo *grafo, Aresta *novaAresta, int grupo, int posOrigem, int posDestino){

    // Garantindo o grupo da tecnologia origem
    grafo->vertices[posOrigem].grupo = grupo;

    // Ponteiro para a aresta inicial
    Aresta **p = &(grafo->vertices[posOrigem].arestaInicial);

    // Percorrendo a lista de arestas do vértice de origem até encontrar a posição correta
    while (*p != NULL && strcmp(novaAresta->tecDestino, (*p)->tecDestino) > 0) {
        p = &(*p)->prox;
    }

    // Inserindo a nova aresta na posição encontrada
    novaAresta->prox = *p;
    *p = novaAresta;

    // Atualizando os atributos do vértice de origem e do vértice de destino
    grafo->vertices[posOrigem].grau++;
    grafo->vertices[posOrigem].grauSaida++;
    grafo->vertices[posOrigem].numArestas++;
    grafo->vertices[posDestino].grau++;
    grafo->vertices[posDestino].grauEntrada++;
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
    Vertice *v2 = criaVertice(r->tecnologiaDestino.string, -2); // VERIFICAR ISSO AQUI
    
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

    // Se tecnologiaOrigem ou tecnologiaDestino for nula, não inserimos nada
    if (!strcmp(r->tecnologiaOrigem.string, "") || !strcmp(r->tecnologiaDestino.string, "")) 
        return;
   
    // Se não for, vamos inserir no vértice correspondente.

    // Se o grafo estiver vazio, precisamos criar um vértice novo
    if (grafo->numVertices == 0) {
        insereGrafoVazio(r, grafo);
        return;
    }

    // Se o grafo não estiver vazio, temos mais alguns casos
    
    // Verificando se a tecOrigem já está no grafo
    int posOrigem = buscaBinariaGrafo(grafo, r->tecnologiaOrigem.string);

    // Caso o vértice de origem NÃO exista no grafo:
    if(strcmp(grafo->vertices[posOrigem].tecnologia, r->tecnologiaOrigem.string) != 0){

        // Corrigindo a Posição
        if(strcmp(r->tecnologiaOrigem.string, grafo->vertices[posOrigem].tecnologia) > 0) posOrigem++;

        // Criando um novo vértice
        Vertice *vertOrigem = criaVertice(r->tecnologiaOrigem.string, r->grupo); // VERIFICAR ISSO AQUI

        // inserimos o vértice no vetor, na posição correta
        adicionaVertice(grafo, vertOrigem, posOrigem);
    }

    // Verificando se tecDestino já está no grafo
    int posDestino = buscaBinariaGrafo(grafo, r->tecnologiaDestino.string);

    // Caso o vértice de destino NÃO exista no grafo:
    if(strcmp(grafo->vertices[posDestino].tecnologia, r->tecnologiaDestino.string) != 0){
        
        // Corrigindo a Posição
        if(strcmp(r->tecnologiaDestino.string, grafo->vertices[posDestino].tecnologia) > 0) posDestino++;

        // Criando um novo vértice
        Vertice *vertDestino = criaVertice(r->tecnologiaDestino.string, -2);

        // inserimos o vértice no vetor, na posição correta
        adicionaVertice(grafo, vertDestino, posDestino);
    }

    // Atualizando posOrigem
    posOrigem = buscaBinariaGrafo(grafo, r->tecnologiaOrigem.string);
    
    // Criando uma nova aresta
    Aresta *a = criaAresta(r->tecnologiaDestino.string, r->peso);
    adicionaAresta(grafo, a, r->grupo, posOrigem, posDestino); 
}

// Função que imprime um grafo 
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
    //(nome tecnologia origem, grupo, grauEntrada, grauSaida, grau, nome tec destino e peso dele)
    //printf("numVertices: %d\n", grafo->numVertices);
}

// Função que retorna o grafo transposto ao de entrada
Grafo *transpor(Grafo *grafo){

    Grafo *grafoTransposto = criaGrafo();

    // Alocando o espaço para o novo grafo
    grafoTransposto->numVertices = grafo->numVertices;
    grafoTransposto->vertices = (Vertice*) malloc(grafo->numVertices * sizeof(Vertice));

    // Percorrendo o vetor de vértices e adicionando os vértices
    for(int i = 0; i < grafoTransposto->numVertices; i++){

        grafoTransposto->vertices[i] = *criaVertice(grafo->vertices[i].tecnologia, grafo->vertices[i].grupo);
    }

    // Agora que todos os vértices foram adicionados, vamos inserir as arestas de forma inversa
    // Antes: No vértice tecOrigem, adicionávamos uma aresta para tecDestino
    // Agora: No vértice atual, pegamos as tecnologias das arestas e criamos uma aresta desssas tecnologias para o vértice atual da iteração
    // .:. o grupo é o mesmo da tecDestino (das arestas) 

    for(int i = 0; i < grafoTransposto->numVertices; i++){
            
        // Percorrendo a lista de arestas
        Aresta *arestaAtual = grafo->vertices[i].arestaInicial;

        while (arestaAtual != NULL){
            
            int posDestino = buscaBinariaGrafo(grafoTransposto, arestaAtual->tecDestino);

            // Criando uma nova aresta
            Aresta *novaAresta = criaAresta(grafo->vertices[i].tecnologia, arestaAtual->peso);

            // Adicionando a aresta no grafo transposto
            adicionaAresta(grafoTransposto, novaAresta, grafo->vertices[posDestino].grupo, posDestino, i);

            arestaAtual = arestaAtual->prox;
        }
    }

    return grafoTransposto;
}


// <----------------------- FUNÇÕES DE DESTRUIÇÃO -----------------------> //

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
