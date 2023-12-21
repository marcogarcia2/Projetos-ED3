/*  
*   Lucas Lima Romero - 13676325
*   Marco Antonio Gaspar Garcia - 11833581
*/

#include "funcoesIndice.h"

// ------------------ Funções do cabecalho de índice ------------------ //

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

void leCabecalhoIndice(CabecalhoIndice *cabIndice, FILE *arquivoIND){

    // Voltando ao início do arquivo
    fseek(arquivoIND, 0, SEEK_SET);

    // Lendo os valores adequadamente
    fread(&cabIndice->status, sizeof(char), 1, arquivoIND);
    fread(&cabIndice->noRaiz, sizeof(int), 1, arquivoIND);
    fread(&cabIndice->RRNproxNo, sizeof(int), 1, arquivoIND);

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


// ---------------- Funções dos nós e chaves da árvore B --------------- //

void liberaNoArvoreB(NoArvoreB *no){
    free(no);
}

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

// Função para testes
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


// ------------------------ Funções auxiliares ------------------------ //

// Busca binária recursiva
int buscaBinariaRecursiva(NoArvoreB *no, int inf, int sup, char *chave){
    if(inf >= sup) // Se os índices se cruzaram, é por que o elemento deverá ser comparado com esse índice
        return inf; // Retorna a posição onde a chave deveria ser inserida
    
    int meio = inf + (sup - inf) / 2;

    if (strcmp(chave, no->C[meio]) == 0)
        return meio;
    
    return (strcmp(chave, no->C[meio]) < 0) ? buscaBinariaRecursiva(no, inf, meio - 1, chave) : buscaBinariaRecursiva(no, meio + 1, sup, chave);
}

// Função que busca uma chave em um nó
int buscaBinaria(NoArvoreB *no, char *chave){
    return buscaBinariaRecursiva(no, 0, no->nroChavesNo - 1, chave);
}

// Função que verifica se um nó é folha
bool noFolha(NoArvoreB *no){
    return no->P[0] == -1;
}

// Função que verifica se cabe uma chave em um nó 
bool cabeNo(NoArvoreB *no){
    return no->nroChavesNo < ORDEM_M - 1;
}


// ----------------------- Funções de inserção ------------------------ //

// Função que grava a primeira chave em um arquivo de índices (arquivo vazio)
void primeiraInsercaoNaRaiz(NoArvoreB *noRaiz, CabecalhoIndice *cabecalho, DadosChave *dados, FILE *arquivoIND) {
    // Colocamos os valores iniciais de nó na raiz
    noRaiz->RRNdoNo = 0;
    noRaiz->nroChavesNo = 1;
    noRaiz->alturaNo = 1;
    
    // Atualizamos o cabecalho
    cabecalho->RRNproxNo++; // Incrementamos o RRN do próximo nó (passa a ser 1)
    cabecalho->noRaiz = 0; // O no da raiz passa a ser o 0 (-1 +1)

    // Grava o dados na raiz, como ela está vazia, vai para o primeiro slot
    noRaiz->P[0] = -1; // Já é -1
    strcpy(noRaiz->C[0], dados->chave);
    noRaiz->PR[0] = dados->PR;

    // printf("Gravando primeira chave: %s\n", noRaiz->C[0]);

    // Grava o nó no arquivo
    gravaCabecalhoIndice(cabecalho, arquivoIND);
    gravaNoArvoreB(noRaiz, arquivoIND, TAM_PAGINA);
}

// Função que insere uma chave em um nó
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

    // Inserindo o ponteiro (se for um nó folha, será -1, se não será o RRN do nó à direita oriundo do split)
    no->P[pos+1] = dados->rrnDireita;

    // Incrementando o nro de chaves
    no->nroChavesNo++;

    // Gravando o nó no arquivo
    gravaNoArvoreB(no, arquivoIND, TAM_PAGINA + (TAM_PAGINA * no->RRNdoNo));
}

// Função que efetua o split
DadosChave *splitNoArvoreB(DadosChave *dados, FILE *arquivoIND, CabecalhoIndice *cabecalho, NoArvoreB *no){
    
    // Criar um nó irmão
    NoArvoreB *noIrmao = criaNoArvoreB();
    noIrmao->nroChavesNo = 1; // O split deixa o nó irmão com apenas uma chave
    noIrmao->alturaNo = no->alturaNo; // A altura do nó irmão é a mesma do nó original
    noIrmao->RRNdoNo = cabecalho->RRNproxNo++; // O RRN do nó irmão é o próximo RRN disponível que é atualizado no cabeçalho

    // Iremos colocar as nossas informações em um vetor auxiliar de split, para facilitar a ordenação
    NoSplit *vetor = malloc(sizeof(NoSplit));
    int i;
    for (i = 0; i < ORDEM_M - 1; i++){
        strcpy(vetor->Csplit[i], no->C[i]);
        vetor->PRsplit[i] = no->PR[i];
        vetor->Psplit[i] = no->P[i];
    }
    vetor->Psplit[i] = no->P[i];

    // Inserir a chave e o PR no vetor
    strcpy(vetor->Csplit[i], dados->chave);
    vetor->PRsplit[i] = dados->PR;
    vetor->Psplit[4] = dados->rrnDireita;
    
    // Ordena Bubble Sort nesse vetor auxiliar para decidir o caminho dos dados na hora de splitar
    for (int i = 0; i < ORDEM_M; i++) {
        for (int j = i + 1; j < ORDEM_M; j++) {
            if (strcmp(vetor->Csplit[i], vetor->Csplit[j]) > 0) {
                char aux[55];
                strcpy(aux, vetor->Csplit[i]);
                strcpy(vetor->Csplit[i], vetor->Csplit[j]);
                strcpy(vetor->Csplit[j], aux);

                int aux2 = vetor->PRsplit[i];
                vetor->PRsplit[i] = vetor->PRsplit[j];
                vetor->PRsplit[j] = aux2;

                aux2 = vetor->Psplit[i+1];
                vetor->Psplit[i+1] = vetor->Psplit[j+1];
                vetor->Psplit[j+1] = aux2;
            }
        }
    }

    // Atualização dos valores do nó antigo ("irmão mais velho")
    no->P[0] = vetor->Psplit[0];

    strcpy(no->C[0], vetor->Csplit[0]);
    no->PR[0] = vetor->PRsplit[0];

    no->P[1] = vetor->Psplit[1];
    
    strcpy(no->C[1], vetor->Csplit[1]);
    no->PR[1] = vetor->PRsplit[1];

    no->P[2] = vetor->Psplit[2];

    no->nroChavesNo = 2;
    
    // Tirando o valor do irmão mais velho que será reposicionado no irmão mais novo
    strcpy(no->C[2], "");
    no->PR[2] = -1;

    no->P[3] = -1;

    // Gravando o nó com os valores atualizados 
    gravaNoArvoreB(no, arquivoIND, TAM_PAGINA + (TAM_PAGINA * no->RRNdoNo));

    // Atualização dos valores do nó irmão
    noIrmao->P[0] = vetor->Psplit[3];

    strcpy(noIrmao->C[0], vetor->Csplit[3]);
    noIrmao->PR[0] = vetor->PRsplit[3];

    noIrmao->P[1] = vetor->Psplit[4];

    noIrmao->nroChavesNo = 1;

    // Gravando o nó irmão
    gravaNoArvoreB(noIrmao, arquivoIND, TAM_PAGINA + (TAM_PAGINA * noIrmao->RRNdoNo));
        
    // Chave promovida
    DadosChave *promovido = criaDadosChave();

    promovido->chave = malloc(strlen(vetor->Csplit[2]) + 1); // +1 para o caractere nulo '\0'

    // Verifica se a alocação foi bem-sucedida
    if(promovido->chave == NULL) {
        // Trata erro de alocação
        printf("Erro de alocação\n");
    }

    strcpy(promovido->chave, vetor->Csplit[2]);
    promovido->PR = vetor->PRsplit[2];
    promovido->rrnDireita = noIrmao->RRNdoNo; // Linha importante

    // Liberar a memória alocada para o vetor
    free(vetor);
    liberaNoArvoreB(noIrmao);
    
    gravaCabecalhoIndice(cabecalho, arquivoIND);

    return promovido;
}

// Função que adiciona uma chave em um nó recursivamente
DadosChave *adicionarRecursivo(FILE *arquivoIND, DadosChave *dados, int RRN, CabecalhoIndice *cabecalho){
    
    NoArvoreB *no;
    
    if(RRN == -1){ // Condição de parada da recursão, indica que chegou em um nó folha
        dados->rrnDireita = -1;
        return dados;
    } else { // Estaremos na caminhada para chegar nesse nó folha, seguindo a ordem dos nós
        no = criaNoArvoreB();
        fseek(arquivoIND, TAM_PAGINA + (TAM_PAGINA * RRN), SEEK_SET);
        leNoArvoreB(no, arquivoIND);
    }

    // Calculando a posição da chave a ser adicionada
    int pos = buscaBinaria(no, dados->chave);
    if (strcmp(dados->chave, no->C[pos]) > 0) pos++;
    
    DadosChave *promovido = adicionarRecursivo(arquivoIND, dados, no->P[pos], cabecalho);

    if(!promovido){ // Se não houver promoção, retorna NULL
        return NULL;
    } else{
        if(cabeNo(no)){ // Se houver promoção e couber no nó, basta inserir
            insereNo(no, pos, promovido, arquivoIND);
            liberaNoArvoreB(no);
            return NULL;

        } else{ // Se houver promoção e não couber no nó, é necessário fazer o split
            return splitNoArvoreB(promovido, arquivoIND, cabecalho, no);
        }
    } 
}

// Função que trata os diferentes tipos de inserção em um arquivo de índices
void adicionar(DadosChave *dados, FILE *arquivoIND, CabecalhoIndice *cabecalho){
    
    // Lendo a raiz, pois a recursão começa a partir dela
    NoArvoreB *noRaiz = criaNoArvoreB();

    if(cabecalho->noRaiz == -1){ // Arquivo vazio, criamos gravamos uma raiz inicial
        primeiraInsercaoNaRaiz(noRaiz, cabecalho, dados, arquivoIND);
        liberaNoArvoreB(noRaiz);
        return;
    }
    else { // Se o arquivo não estiver vazio, precisamos ler a raiz
        fseek(arquivoIND, TAM_PAGINA + (TAM_PAGINA * cabecalho->noRaiz), SEEK_SET);
        leNoArvoreB(noRaiz, arquivoIND);
    }

    // Calculando a posição da chave a ser adicionada
    int pos = buscaBinaria(noRaiz, dados->chave);
    
    // Podemos colocar isso dentro do busca binária
    if (strcmp(dados->chave, noRaiz->C[pos]) > 0) pos++;

    // A função adicionarRecursivo retorna NULL se não houver promoção
    DadosChave *promovido = adicionarRecursivo(arquivoIND, dados, noRaiz->P[pos], cabecalho); 

    // Só entrará aqui para fazer as inserções na raiz (que é o único caso em que promovido != NULL)
    if(promovido){

        // Se o promovido subiu e cabe na raiz, basta inseri-lo sem a necessidade de split
        if(cabeNo(noRaiz)){

            insereNo(noRaiz, pos, promovido, arquivoIND);
        }

        else{  
            // Tratamento do split na raiz

            // Criando a chave da nova raiz
            DadosChave *chaveDaNovaRaiz = criaDadosChave();

            // A função split retorna qual das chaves será promovida
            chaveDaNovaRaiz = splitNoArvoreB(promovido, arquivoIND, cabecalho, noRaiz);

            // Criando a nova raiz
            NoArvoreB *novaRaiz = criaNoArvoreB();

            // Atualizando os valores da nova raiz com o promovido pelo split e o cabeçalho
            strcpy(novaRaiz->C[0], chaveDaNovaRaiz->chave);
            novaRaiz->PR[0] = chaveDaNovaRaiz->PR;
            novaRaiz->P[0] = noRaiz->RRNdoNo;
            novaRaiz->P[1] = chaveDaNovaRaiz->rrnDireita; 

            novaRaiz->nroChavesNo = 1;
            novaRaiz->RRNdoNo = cabecalho->RRNproxNo++; 
            novaRaiz->alturaNo = noRaiz->alturaNo + 1;

            cabecalho->noRaiz = novaRaiz->RRNdoNo;

            // Gravando a nova raiz
            gravaNoArvoreB(novaRaiz, arquivoIND, TAM_PAGINA + (TAM_PAGINA * novaRaiz->RRNdoNo));

            free(chaveDaNovaRaiz->chave);
            free(chaveDaNovaRaiz);
        }
        
        liberaNoArvoreB(noRaiz);
    }
}


// -------------------------------------------------------------------- //
