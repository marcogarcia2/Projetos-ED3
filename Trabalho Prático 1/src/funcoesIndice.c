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

    return dados;
}

// Função que grava um nó inteiro num arquivo de árvore B
void gravaNoArvoreB(NoArvoreB *no, FILE *arquivoIND){
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

// 
bool noFolha(NoArvoreB *no){

    return no->P[0] == -1 ? true : false;
}

// Ocorrerá quando o arquivo estiver vazio
void insereNaRaiz(DadosChave *dados, FILE *arquivoIND){

    // Isso só acontece uma vez, quando o arquivo está vazio
    fseek(arquivoIND, TAM_PAGINA, SEEK_SET);
    NoArvoreB *no = criaNoArvoreB();

    no->nroChavesNo = 1;
    no->RRNdoNo = 0;
    
    strcpy(no->C[0], dados->chave);
    no->PR[0] = dados->PR;

    //gravaDadosIndice(dados, arquivoIND, ftell(arquivoIND));
    gravaNoArvoreB(no, arquivoIND);

    imprimeNoArvoreB(no);
    liberaNoArvoreB(no);
}

void insereSemSplit(NoArvoreB *no, DadosChave *dados, FILE *arquivoIND, int byteInicial){
    int flagInseriu = 0;
    

    // Se o nó não estiver cheio, insere no nó
    for (int i = 0; i < no->nroChavesNo; i++){ // Só entrará aqui se o nó tiver no máximo 2 chaves
        
        if(strcmp(dados->chave, no->C[i]) <= 0){

            if(strcmp(dados->chave, no->C[i]) == 0){
                // Achamos a chave, retornamos seu ponteiro de referência para o arquivo de dados
                printf("Elemento repetido!"); // Não é para ocorrer!
                break;
            }

            else{ // Ou seja, se for menor na ordem alfabética, vamos inserir na sua esquerda
                // Fazendo um shift dos elementos para a direita
                printf("Inserindo no meio\n");
                printf("Chave: %s\n", dados->chave);

                for(int j = no->nroChavesNo - 1; j >= i; j--){
                    strcpy(no->C[j + 1], no->C[j]);
                    no->PR[j+1] = no->PR[j];
                }
                // Inserindo a chave e o PR
                strcpy(no->C[i], dados->chave);
                no->PR[i] = dados->PR;

                flagInseriu = 1;
                imprimeNoArvoreB(no);
                break;
            }
        }

        else // Se não for menor nem igual, vou para a próxima chave!
            continue;
    }

    // Fora do loop, precisamos saber se houve inserção (se não, inserimos no final)
    if(flagInseriu == 0){
        printf("Inserindo no final\n");
        printf("Chave: %s\n", dados->chave);
        imprimeNoArvoreB(no);
        strcpy(no->C[no->nroChavesNo], dados->chave);
        no->PR[no->nroChavesNo] = dados->PR;
        flagInseriu = 1;
    }

    if(flagInseriu == 1){
        // Incrementando o nro de chaves
        no->nroChavesNo++;
        fseek(arquivoIND, byteInicial, SEEK_SET);
        gravaNoArvoreB(no, arquivoIND);
    }
}

// Função que insere uma chave dentro do nó
void insereRecursivamente(DadosChave *dados, FILE *arquivoIND, CabecalhoIndice *cabecalho, int RRNno){
    // Aqui conterá a lógica de inserção recursiva que poderá ou não ter split
    // Se tiver, será chamada uma função para efetuar o split

    // Pula para o nó desejado (o primeiro será a raiz)
    fseek(arquivoIND, TAM_PAGINA + (RRNno * TAM_PAGINA), SEEK_SET); // Vou pro começo do nó da "raiz"
    int byteInicial = ftell(arquivoIND);
    // Carrega o nó da raiz em memória principal
    NoArvoreB *no = criaNoArvoreB();
    leNoArvoreB(no, arquivoIND);

    // Leu a raiz, vai pra onde agora, filho? 
    if (!noFolha(no)){

        for(int i = 0; i < no->nroChavesNo; i++){

            if(strcmp(dados->chave, no->C[i]) <= 0){

                if(strcmp(dados->chave, no->C[i]) == 0){
                    printf("Deu pau!\n"); 
                    break;
                }
                else{ // Ou seja, se for menor na ordem alfabética, vamos para a esquerda
                    if (no->P[i] != -1){ // existe subárvore
                        fseek(arquivoIND, TAM_PAGINA + (no->P[i] * TAM_PAGINA), SEEK_SET);
                        insereRecursivamente(dados, arquivoIND, cabecalho, no->P[i]);
                    }
                }
            }
            else // Se não for menor nem igual, vou para a próxima chave!
                continue;
        }

        insereRecursivamente(dados, arquivoIND, cabecalho, no->P[no->nroChavesNo]);
    }

    // Se sair do for sem entrar no if, significa que a 
    // chave é maior que todas as chaves do nó atual, então passamos P[3] = P[nroChavesNo]


    // Se é um nó folha, a inserção ocorre diretamente
    if (noFolha(no)){
        // Vou fazer a checagem, se consigo colocar no nó atual
        if(no->nroChavesNo < ORDEM_M - 1){ 
            //printf("Inserindo no nó\n");
            insereSemSplit(no, dados, arquivoIND, byteInicial);
            liberaNoArvoreB(no);
            return;
        }

        else{ // Se o nó estiver cheio, insere no nó e faz o split
            
            /*
            no->nroChavesNo == 3;
            achamos o nó que queremos, mas se inserirmos vai ficar com 4 chaves

                ASP AZU SQL  ->> ASP AZU ENT SQL

                  AZU
                /      \
              ASP    ENT SQL

            Limpar o primeiro nó e deixar só ASP e gravar no arquivo
            Criar a nova raiz, alterar o RRN no cabeçalho e gravar no arquivo
            Criar o nó folha com ENT e SQL e gravar no arquivo
            */

            NoSplit noAtual;

            // Copiando informacoes do no para o nosplit
            int i;
            for (i = 0; i < ORDEM_M-1; i++){
                strcpy(noAtual.Csplit[i], no->C[i]);
                noAtual.PRsplit[i] = no->PR[i];
            }
            strcpy(noAtual.Csplit[i], dados->chave);
            noAtual.PRsplit[i] = dados->PR;

            // Depois de tudo copiado, ordena as chaves (Bubble Sort)
            for (int i = 0; i < ORDEM_M; i++){
                for (int j = i + 1; j < ORDEM_M; j++){
                    if(strcmp(noAtual.Csplit[i], noAtual.Csplit[j]) > 0){
                        char aux[55];
                        strcpy(aux, noAtual.Csplit[i]);
                        strcpy(noAtual.Csplit[i], noAtual.Csplit[j]);
                        strcpy(noAtual.Csplit[j], aux);

                        int aux2 = noAtual.PRsplit[i];
                        noAtual.PRsplit[i] = noAtual.PRsplit[j];
                        noAtual.PRsplit[j] = aux2;
                    }
                }
            }

            // Agora as chaves estão ordenadas, escolheremos qual vai pra cada lado
            // O do meio menor vai para a raiz (AZU) == noAtual.Csplit[1]
            // O da esquerda (ASP) permanece no nó atual, temos que limpá-lo
            // Os da direita (ENT e SQL) vão para o nó irmão

            // Criando o nó irmão
            NoArvoreB *noIrmao = criaNoArvoreB();
            noIrmao->nroChavesNo = ORDEM_M/2;           //SEMPRE
            noIrmao->RRNdoNo = cabecalho->RRNproxNo;
            cabecalho->RRNproxNo++;
            strcpy(noIrmao->C[0], noAtual.Csplit[2]);
            noIrmao->PR[0] = noAtual.PRsplit[2];
            strcpy(noIrmao->C[1], noAtual.Csplit[3]);
            noIrmao->PR[1] = noAtual.PRsplit[3];

            NoArvoreB *noRaiz = NULL;
            if (no->RRNdoNo == cabecalho->noRaiz){
                // Criando o nó raiz
                noRaiz = criaNoArvoreB();
                noRaiz->nroChavesNo = 1;
                noRaiz->RRNdoNo = cabecalho->RRNproxNo;
                cabecalho->RRNproxNo++;
                noRaiz->alturaNo = no->alturaNo + 1;
                strcpy(noRaiz->C[0], noAtual.Csplit[1]);
                noRaiz->PR[0] = noAtual.PRsplit[1];
                // P deve apontar corretamente para os filhos
                noRaiz->P[0] = no->RRNdoNo;
                noRaiz->P[1] = noIrmao->RRNdoNo;
            }

            // Limpando o nó atual
            resetaNo(no);
            no->nroChavesNo = 1;
            strcpy(no->C[0], noAtual.Csplit[0]);
            no->PR[0] = noAtual.PRsplit[0];

            // Gravando os nós no arquivo
            fseek(arquivoIND, byteInicial, SEEK_SET);
            gravaNoArvoreB(no, arquivoIND);

            fseek(arquivoIND, TAM_PAGINA + (noIrmao->RRNdoNo * TAM_PAGINA), SEEK_SET);
            gravaNoArvoreB(noIrmao, arquivoIND);
            
            if (noRaiz){
                fseek(arquivoIND, TAM_PAGINA + (noRaiz->RRNdoNo * TAM_PAGINA), SEEK_SET);
                gravaNoArvoreB(noRaiz, arquivoIND);
                // Atualizando o cabeçalho
                cabecalho->noRaiz = noRaiz->RRNdoNo;
            }

            gravaCabecalhoIndice(cabecalho, arquivoIND);

            liberaNoArvoreB(no);
            liberaNoArvoreB(noIrmao);
            liberaNoArvoreB(noRaiz);

            return;
        }
    }
}

// Função que efetua o split
void splitNoArvore(DadosChave *dados, FILE *arquivoIND){

}

// Função que insere no arquivo de índices
void insereArquivoIndice(DadosChave *dados, CabecalhoIndice *cabecalho, FILE *arquivoIND){
    /* --------- TIPOS DE INSERÇÃO ---------
        1- Inserção na raiz (RRNraiz == -1)
        2- Inserção sem divisão (quando tem espaço)
        3- Inserção com divisão (quando está cheio)
    */
   // chave = WPF

    printf("---------------------\n");
    printf("nó Raiz: %d\n", cabecalho->noRaiz);
    printf("Chave a ser inserida: %s\n", dados->chave);
    printf("---------------------\n");

    // NoArvoreB *no = criaNoArvoreB();
    //printf("Dado a ser inserido: %s\n", dados->chave);
    if(cabecalho->noRaiz == -1){
        // Se o arquivo estiver vazio, vamos inserir na raiz
        insereNaRaiz(dados, arquivoIND);
        cabecalho->noRaiz = 0;
        cabecalho->RRNproxNo = 1;
        gravaCabecalhoIndice(cabecalho, arquivoIND);
    }
    else{
        // Se não estiver vazio, vamos inserir recursivamente
        insereRecursivamente(dados, arquivoIND, cabecalho, cabecalho->noRaiz);
    }
}
