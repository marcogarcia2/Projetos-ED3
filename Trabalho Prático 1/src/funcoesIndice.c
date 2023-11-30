/*  
*   Lucas Lima Romero - 13676325
*   Marco Antonio Gaspar Garcia - 11833581
*/

#include "funcoesIndice.h"

// Função que cria um Cabeçalho de Árvore, inserido no início do arquivo binário de índice
CabecalhoIndice *criaCabecalhoIndice(void){

    // Alocando a memória dinamicamente
    CabecalhoIndice *cabIndice = (CabecalhoIndice*) malloc(sizeof(CabecalhoIndice));

    // Inicializando com os valores padrão
    cabIndice->status = '0';
    cabIndice->noRaiz = -1;
    cabIndice->RRNproxNo = 0;

    return cabIndice;
}

// Função que grava o Cabeçalho de Árvore no início do arquivo
void gravaCabecalhoIndice(CabecalhoIndice *cabIndice, FILE *arquivoIND){

    // Voltando ao início do arquivo
    fseek(arquivoIND, 0, SEEK_SET);

    // Escrevendo os valores no arquivo
    fwrite(&cabIndice->status, sizeof(char), 1, arquivoIND);
    fwrite(&cabIndice->noRaiz, sizeof(int), 1, arquivoIND);
    fwrite(&cabIndice->RRNproxNo, sizeof(int), 1, arquivoIND);
    
    // Completando com lixo o restante do cabeçalho
    for(int i = 0; i < 196; i++)
        fwrite(LIXO, sizeof(char), 1, arquivoIND);
}

void liberaNoArvoreB(NoArvoreB *no){
    free(no);
}
// ------------------ FUNÇÕES AUXILIARES ------------------ //
// As funções a seguir, foram criadas, a priori, para a funcionalidade 6

NoArvoreB *criaNoArvoreB(void){
    NoArvoreB *no = (NoArvoreB *) malloc(sizeof(NoArvoreB));

    no->nroChavesNo = 0;
    no->alturaNo = 1;
    no->RRNdoNo = -1;

    for (int i = 0; i < 4; i++) {
        no->P[i] = -1;
        if (i < 3) {
            strcpy(no->C[i], "");
            no->PR[i] = -1;
        }
    }

    return no;
}

// Função auxiliar da leNoArvoreB
void leInformacaoUtil(FILE *arquivoIND, char *Ci) {
    // Lê todos os 55 bytes (preciso disso, para não ser necessário dar fseeks)
    fread(Ci, sizeof(char), 55, arquivoIND);

    // Encontra e substitui o primeiro '$' por '\0'
    for (int i = 0; i < 55; i++) {
        if (Ci[i] == '$') {
            Ci[i] = '\0';
            break;
        }
    }
}

// Função que lê um nó da árvore B do arquivo de índices e grava em um NoArvoreB
void leNoArvoreB(NoArvoreB *noArvB, FILE *arquivoIND){
    fread(&noArvB->nroChavesNo, sizeof(int), 1, arquivoIND);
    fread(&noArvB->alturaNo, sizeof(int), 1, arquivoIND);
    fread(&noArvB->RRNdoNo, sizeof(int), 1, arquivoIND);

    for (int i = 0; i < 4; i++) {
        fread(&noArvB->P[i], sizeof(int), 1, arquivoIND);
        if (i < 3) {
            leInformacaoUtil(arquivoIND, noArvB->C[i]);
            fread(&noArvB->PR[i], sizeof(int), 1, arquivoIND);
        }
    }
}

void imprimeNoArvoreB(NoArvoreB *no){
    printf("Nro de chaves: %d\n", no->nroChavesNo);
    printf("Altura do no: %d\n", no->alturaNo);
    printf("RRN do no: %d\n\n", no->RRNdoNo);

    int i;
    for (i = 0; i < 3; i++) {
        printf("P %d: %d\n", i+1, no->P[i]);
        printf("Chave %d: %s\n", i+1, no->C[i]);
        printf("PR %d: %d\n", i+1, no->PR[i]);
    }
    printf("P %d: %d\n\n\n", i+1, no->P[i]);
}

// ------------------ FUNÇÕES FUNCIONALIDADE 5 ------------------ //

// Função que cria um registro
DadosChave *criaDadosChave(void){
    DadosChave *dados = (DadosChave *) malloc(sizeof(DadosChave));

    dados->chave = NULL;
    dados->PR = -1;
    dados->rrnDireita = -1;

    return dados;
}

// Função que grava um nó inteiro num arquivo de árvore B
void gravaNoArvoreB(NoArvoreB *no, FILE *arquivoIND, int byteInicial){

    // Posicionando o ponteiro no arquivo
    fseek(arquivoIND, byteInicial, SEEK_SET);

    // Gravando o nó no arquivo
    fwrite(&no->nroChavesNo, sizeof(int), 1, arquivoIND); // Inserindo o nro de chaves
    fwrite(&no->alturaNo, sizeof(int), 1, arquivoIND); // Inserindo a altura
    fwrite(&no->RRNdoNo, sizeof(int), 1, arquivoIND); // Inserindo o RRN do nó

    for (int i = 0; i < 4; i++) {
        fwrite(&no->P[i], sizeof(int), 1, arquivoIND);
        if (i < 3) {
            fwrite(no->C[i], sizeof(char), strlen(no->C[i]), arquivoIND);
            int len = strlen(no->C[i]);
            for (int j = len; j < 55; j++) {
                fputc('$', arquivoIND);
            }

            fwrite(&no->PR[i], sizeof(int), 1, arquivoIND);
        }
    }
}

// Função que grava os dados de apenas uma chave
void gravaDadosIndice(DadosChave *dados, FILE *arquivoIND, int byteOffset){
    fseek(arquivoIND, byteOffset, SEEK_SET);

    int P = -1; // Indicação de nó folha

    // Gravando o ponteiro
    fwrite(&P, sizeof(int), 1, arquivoIND);

    // Gravando chave
    fwrite(dados->chave, sizeof(char), strlen(dados->chave), arquivoIND);

    // Lógica para gravar o lixo
    int len = strlen(dados->chave);
    while (len < 55) {
        fputc('$', arquivoIND);
        len++;
    }

    // Gravando PR
    fwrite(&dados->PR, sizeof(int), 1, arquivoIND);
}

void resetaNo(NoArvoreB *no){

    no->nroChavesNo = 0;
    no->alturaNo = 1; ////////////////////VERDE POIS///////////////////////////////////////////

    for (int i = 0; i < ORDEM_M; i++) {
        no->P[i] = -1;
        if (i < ORDEM_M - 1) {
            strcpy(no->C[i], "");
            no->PR[i] = -1;
        }
    }
}

int buscaBinariaRecursiva(NoArvoreB *no, int inf, int sup, char *chave){
    if(inf >= sup)
        return inf; // Retorna a posição onde a chave deveria ser inserida
    
    int meio = inf + (sup - inf) / 2;

    return (strcmp(chave, no->C[meio]) < 0) ? buscaBinariaRecursiva(no, inf, meio - 1, chave) : buscaBinariaRecursiva(no, meio + 1, sup, chave);
}

int buscaBinaria(NoArvoreB *no, char *chave){
    return buscaBinariaRecursiva(no, 0, no->nroChavesNo - 1, chave);
}

// 
bool noFolha(NoArvoreB *no){
    return no->P[0] == -1;
}

bool cabeNo(NoArvoreB *no){
    return no->nroChavesNo < ORDEM_M - 1;
}

void insereNo(NoArvoreB *no, int pos, DadosChave *dados, FILE *arquivoIND){

    // Fazendo um shift dos elementos para a direita
    for(int i = no->nroChavesNo - 1; i >= pos; i--){
        strcpy(no->C[i + 1], no->C[i]);
        no->PR[i+1] = no->PR[i];
        no->P[i+2] = no->P[i+1];
    }
    // Inserindo a chave e o PR
    strcpy(no->C[pos], dados->chave);
    no->PR[pos] = dados->PR;
    no->P[pos+1] = dados->rrnDireita;

    // Incrementando o nro de chaves
    no->nroChavesNo++;

    // Gravando o nó no arquivo
    gravaNoArvoreB(no, arquivoIND, TAM_PAGINA + (TAM_PAGINA * no->RRNdoNo));

    /*
             B        C       D
        P0 /   \ PX     \ P1    \ P2

    */

}

// Função que efetua o split
DadosChave *splitNoArvoreB(DadosChave *dados, FILE *arquivoIND, CabecalhoIndice *cabecalho, NoArvoreB *no){
    
    // Criar um nó irmão
    NoArvoreB *noIrmao = criaNoArvoreB();
    noIrmao->nroChavesNo = 1;
    noIrmao->alturaNo = no->alturaNo;
    noIrmao->RRNdoNo = cabecalho->RRNproxNo++;

    // Ordenar as chaves
    NoSplit vetor;
    int i;
    for (i = 0; i < ORDEM_M-1; i++){
        strcpy(vetor.Csplit[i], no->C[i]);
        vetor.PRsplit[i] = no->PR[i];
        vetor.Psplit[i] = no->P[i];
    }
    vetor.Psplit[i] = no->P[i];

    // Inserir a chave e o PR no vetor
    strcpy(vetor.Csplit[i], dados->chave);
    vetor.PRsplit[i] = dados->PR;
    vetor.Psplit[4] = dados->rrnDireita;
    
    // Ordena Bubble Sort
    for (int i = 0; i < ORDEM_M; i++) {
        for (int j = i + 1; j < ORDEM_M; j++) {
            if (strcmp(vetor.Csplit[i], vetor.Csplit[j]) > 0) {
                char aux[55];
                strcpy(aux, vetor.Csplit[i]);
                strcpy(vetor.Csplit[i], vetor.Csplit[j]);
                strcpy(vetor.Csplit[j], aux);

                int aux2 = vetor.PRsplit[i];
                vetor.PRsplit[i] = vetor.PRsplit[j];
                vetor.PRsplit[j] = aux2;

                aux2 = vetor.Psplit[i+1];
                vetor.Psplit[i+1] = vetor.Psplit[j+1];
                vetor.Psplit[j+1] = aux2;
            }
        }
    }

    // Nó antigo
    no->P[0] = vetor.Psplit[0];
    strcpy(no->C[0], vetor.Csplit[0]);
    no->PR[0] = vetor.PRsplit[0];

    no->P[1] = vetor.Psplit[1];
    strcpy(no->C[1], vetor.Csplit[1]);
    no->PR[1] = vetor.PRsplit[1];

    no->P[2] = vetor.Psplit[2];

    // Gravando o nó
    gravaNoArvoreB(no, arquivoIND, TAM_PAGINA + (TAM_PAGINA * no->RRNdoNo));


    // Irmão
    noIrmao->P[0] = vetor.Psplit[3];
    strcpy(noIrmao->C[0], vetor.Csplit[3]);
    noIrmao->PR[0] = vetor.PRsplit[3];
    noIrmao->P[1] = vetor.Psplit[4];

    // Gravando o nó irmão
    gravaNoArvoreB(noIrmao, arquivoIND, TAM_PAGINA + (TAM_PAGINA * noIrmao->RRNdoNo));
    
        
    // Gravar nós ////////////////////////////////////////////////////////////////////


    // Chave promovida
    DadosChave *promovido = criaDadosChave();
    strcpy(promovido->chave, vetor.Csplit[2]);
    promovido->PR = vetor.PRsplit[2];
    promovido->rrnDireita = noIrmao->RRNdoNo;
    

    return promovido;
}

DadosChave *adicionarRecursivo(FILE *arquivoIND, DadosChave *dados, int RRN, CabecalhoIndice *cabecalho){
    
    NoArvoreB *no;
    
    if(RRN == -1){ // Condição de parada da recursão
        dados->rrnDireita = -1;
        return dados;
    } else {
        no = criaNoArvoreB();
        fseek(arquivoIND, TAM_PAGINA + (TAM_PAGINA * RRN), SEEK_SET);
        leNoArvoreB(no, arquivoIND);
    }

    int pos = buscaBinaria(no, dados->chave);
    if (strcmp(dados->chave, no->C[pos]) > 0) pos++;
    
    DadosChave *promovido = adicionarRecursivo(arquivoIND, dados, no->P[pos], cabecalho);

    if(!promovido){
        return NULL;
    } else{
        if(cabeNo(no)){
            insereNo(no, pos, dados, arquivoIND);

            return NULL;
        } else{
            return splitNoArvoreB(dados, arquivoIND, cabecalho, no); // Nó novo que fica à direita (1 chave)
        }
    } 
}

// 3 Informações: chave e o pr, rrn no a ser inserido 
void adicionar(DadosChave *dados, FILE *arquivoIND, CabecalhoIndice *cabecalho){

    NoArvoreB *noRaiz = criaNoArvoreB();
    if(cabecalho->noRaiz == -1){ // Arquivo vazio
        noRaiz->RRNdoNo = 0;
        gravaNoArvoreB(noRaiz, arquivoIND, TAM_PAGINA);
    }else {
        fseek(arquivoIND, TAM_PAGINA + (TAM_PAGINA * cabecalho->noRaiz), SEEK_SET);
        leNoArvoreB(noRaiz, arquivoIND);
    }

    int pos;
    // RRN = Busca binária (raiz e a chave)
    pos = buscaBinaria(noRaiz, dados->chave);
    if (strcmp(dados->chave, noRaiz->C[pos]) > 0) pos++;

    DadosChave *promovido = adicionarRecursivo(arquivoIND, dados, pos, cabecalho); // Passamos a posição como argumento

    if(promovido){
        if(cabeNo(noRaiz)){

            // Inserir sem split
            insereNo(noRaiz, pos, promovido, arquivoIND);
            gravaCabecalhoIndice(cabecalho, arquivoIND);
            liberaNoArvoreB(noRaiz);
            free(promovido);
        }

        else{
            
            DadosChave *novaChave = criaDadosChave();
            novaChave = splitNoArvoreB(dados, arquivoIND, cabecalho, noRaiz);
            NoArvoreB *novoNo = criaNoArvoreB();
            strcpy(novoNo->C[0], novaChave->chave);
            novoNo->PR[0] = novaChave->PR;

            novoNo->RRNdoNo = cabecalho->RRNproxNo++;
            novoNo->P[0] = noRaiz->RRNdoNo;
            novoNo->P[1] = novaChave->rrnDireita;

            cabecalho->noRaiz = novoNo->RRNdoNo;
            gravaNoArvoreB(novoNo, arquivoIND, TAM_PAGINA + (TAM_PAGINA * novoNo->RRNdoNo));

            liberaNoArvoreB(novoNo);
            free(novaChave);

            gravaCabecalhoIndice(cabecalho, arquivoIND);
        }
    }
}



