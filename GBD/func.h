// END 0 NUMERO CAMPOS END 1 ESPAÇO LIVRE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct rid
{
    int key;
    int slot;
    int pagina;
} Rid;

typedef struct dado
{
    int flag;
    int inteiro;
    short int tamanho;
    char* varchar;
} Dado;

typedef struct campo
{
    char nome[20];
    Dado dado;
} Campo;

typedef struct tabela
{
    Campo *campos;
    char chave[20];
    char nome[20];
    struct tabela* prox;
    int ncampos;
} Tabela;

void acrescentapagina(char *nomearq);
int remover(char*nomesaida,int key,Tabela *tab,char *nometabbusca);
int buscar(char*nomesaida,int key,Tabela *tab,char *nometabbusca);
int inseretupla(Tabela* tab,char *nomeArquivo,int pagina, int *ultimapagina,char *nomearq);
int resultadobusca(char*nomesaida,int pagina,int slot,Tabela *tab,char *nometabbusca);
int atualiza(char*nomesaida,int key,Tabela *tab,char *nometabbusca, int pagina,int *ultimapagina,char *nomearq);
