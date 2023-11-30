/*  
*   Lucas Lima Romero - 13676325
*   Marco Antonio Gaspar Garcia - 11833581
*/

/*  
*  Fazer uma função que lê um arquivo binário definido anteriormente no trabalho introdutório, e a
*  partir disso criar um novo arquivo binário (de índice), que representa uma estrutura de árvore B.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "funcionalidades.h"
#include "registros.h"
#include "funcoesCriadas.h"
#include "funcoesFornecidas.h"
#include "funcoesIndice.h"

// Funcionalidade 5: criar um arquivo de índice a partir de um arquivo de dados binário

/* --------- PRÓXIMOS PASSOS ------------

- Precisamos fazer com que, dado um registro NÃO removido, concatene as strings dele e armazena numa variável (stringConcatenada) que será a chave de busca
- Além da string, precisaremos da informação do RRN desse registro (que será o PR)

- SUGESTÃO: Criar uma struct "dados", por exemplo, que terá essas duas informações para cada registro

- Depois, precisamos inserir essa chave e esse PR na árvore B (que será criada em memória principal). O ponteiro P dessa chave recém inserida será sempre -1.

- A princípio, vou escrever o nó raiz no arquivo de índices, e depois vou inserindo os outros nós (sempre atualizando o cabeçalho)

- A ideia seria construir uma função que:
    - Recebe esses DadosChave (chave e PR) e insere (recursivamente)
    - Se a chave já existir, não insere


*/

void geraArquivoIndice(char *nomeArquivoBIN, char *nomeArquivoIND){

    // Abre o arquivo de dados
    FILE *arquivoBIN = fopen(nomeArquivoBIN, "rb");
    if(arquivoBIN == NULL){
        printf("Falha no processamento do arquivo.\n");
        return;
    }

    // Cria o arquivo de índices
    FILE *arquivoIND = fopen(nomeArquivoIND, "wb+");
    if(arquivoIND == NULL){
        printf("Falha no processamento do arquivo.\n");
        return;
    }

    
    // Byte de controle do arquivo de dados
    int byteOffset = 0;

    // Se o arquivo de dados estiver consistente
    if(fgetc(arquivoBIN) == '1') // Pula os 13 primeiros bytes do arquivo, referentes ao cabeçalho
        byteOffset = 13;

    else{ // Se estiver inconsistente
        printf("Falha no processamento do arquivo.\n");
        fclose(arquivoBIN);
        fclose(arquivoIND);
        return;
    }

    // Cria o cabeçalho do arquivo de índices
    CabecalhoIndice *cabecalho = criaCabecalhoIndice();

    // Colocando '0' de status no arquivo (pois foi aberto)
    gravaCabecalhoIndice(cabecalho, arquivoIND);

    // Cria o registro que será lido em memória principal e um struct DadosChave
    Registro *r;
    DadosChave *dados;

    // Contador de RRN (começa no 0 e é incrementado a cada leitura ou pulo de registro)
    int ponteiroReferencia = 0;

    // Tamanho total do arquivo de dados
    const unsigned int tamTotal = calculaTamanhoTotal(arquivoBIN);
    printf("Tamanho total: %d\n", tamTotal);

    // Lê o arquivo de dados e insere na árvore B concomitantemente
    while(byteOffset < 76*25){ // Quando ocorre o split na raiz está dando erro (TAM_REGISTRO * 4)

        // Cria e lê o registro do arquivo de dados que será lido em memória principal
        r = criaRegistro(); 
        r = leRegistro(byteOffset, r, arquivoBIN);

        printf("\n");

        // A cada iteração, precisarei saber onde está a raiz
        //fseek(arquivoIND, 1, SEEK_SET);

        // Leio a posição da raiz (RRNraiz) para posteriormente dar um fseek para lá
        //fread(&cabecalho->noRaiz, sizeof(int), 1, arquivoIND);

        if(r->removido == '0'){ // Se não estiver removido, ocorrerá a inserção no arquivo de índices
            
            // Criando a chave (stringConcatenada) concatenando nomeTecnologiaOrigem e nomeTecnologiaDestino
            dados = criaDadosChave();
            dados->chave = concatenaStrings(r);
            printf("Chave: %s\n", dados->chave);

            // Preciso guardar também o que será o PR, que é o RRN dessa chave (ponteiroReferencia)
            dados->PR = ponteiroReferencia;

            // Agora vou fazer a inserção.
            // Ponteiro arquivoBIN: byteOffset + TAM_REGISTRO (pois ele leu o registro todo)
            // Ponteiro arquivoIND: 5 (pois ele leu os status (char) e o primeiro campo (int));
            adicionar(dados, arquivoIND, cabecalho);

            // Libera a chave
            free(dados->chave);
            free(dados);
        }

        // Libera o registro completo
        liberaRegistro(r);
        
        // Precisamos saltar até o próximo registro
        byteOffset += TAM_REGISTRO;

        // Precisamos incrementar o PR 
        ponteiroReferencia++;
    }

    // Grava o cabecalho no arquivo de índices
    cabecalho->status = '1';
    printf("Status: %c\n", cabecalho->status);
    gravaCabecalhoIndice(cabecalho, arquivoIND); 

    // Fecha os arquivos
    fclose(arquivoBIN);
    fclose(arquivoIND);

    binarioNaTela(nomeArquivoIND);
}
