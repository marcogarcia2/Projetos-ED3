#define TAM_REGISTRO 76
#define TAM_REGISTRO_FIXO 21
#define LIXO '$'



typedef struct{ // tamanho fixo de 13 bytes? Cabelho
    char status;
    int proxRRN;
    int nroTecnologias; // indica o nro de tecnologias unicas de origem
    int nroParesTecnologias;
} Cabecalho;

typedef struct{
    int tamanho;
    char* string;
} StringVariavel;

typedef struct{
    char removido;

    int grupo;
    int popularidade;
    int peso;
    StringVariavel tecnologiaOrigem;
    StringVariavel tecnologiaDestino;
} Registro;

// A primeira função é criar a tabela de dados
// DEpois (?) tem uma funçao que gera um binario aleatorio para checar se criou certo
// quanndo o campo do registro for vazio, colocar nulo

// comando linux para comparar binario cmp bin1.bin bin2.bin
