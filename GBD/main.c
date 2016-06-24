#include <stdio.h>
#include <stdlib.h>
#include "func.h"

int main()
{
    char nomeentrada[20],nometabela[20], nomesaida[20],nomedb[20];
    //char nometabela2[20],  nomesaida2[20];
    int aux='*',pagina;
    Tabela *tab;
    printf("====SISTEMA DE GERENCIAMENTO DE BANCO DE DADOS====");
    //printf("\n\n\n\n %c Digite o nome do arquivo com informacoes da tabela: ",aux);
    //gets(nomeentrada);
    strcpy(nomeentrada,"tabela.txt");

    fflush(stdin);
    printf("\n %c Digite o nome do arquivo de saida: ",aux);
    gets(nomesaida);
    fflush(stdin);
    //printf("\n %c Digite o nome do arquivo de saida2: ",aux);
   // gets(nomesaida2);
    //fflush(stdin);

    fflush(stdin);
    strcpy(nomedb,criaBD());
    tab = defineBD(nomeentrada);

    //printf("\n %c Digite o nome do arquivo com tuplas da Tabela: ",aux);
    //gets(nometabela);
    strcpy(nometabela,"Entrada.txt");

    //printf("\n %c Digite o nome do arquivo com tuplas da Tabela 2: ",aux);
    //gets(nometabela2);
    //strcpy(nometabela2,"Entrada2.txt");
    //fflush(stdin);
// ========================================================aqui pra baixo importante===========================
    inicializaarquivo(aux,nomesaida);
    int pagina2=0;
    carregaTabela(tab,nometabela,0,&pagina,nomesaida);
    //carregaTabela(tab,nometabela2,0,&pagina2,nomesaida2);
    int escolha=10,keyparabusca;
    char nometabbusca[20];
    char nometabaux1[20];
    char nometabaux2[20];
    strcpy(nometabaux1,"TABELA");
    strcpy(nometabaux2,"TABELA2");
    //if(aux==0)system("pause");
    //exit(0);

    //if(aux==0)system("pause");
    while(escolha>0)
    {
        system("cls");
        printf("---Menu---\nDigite:\n");
        printf("1 - Para Buscar;\n");
        printf("2 - Para Inserir;\n");
        printf("3 - Para Remover;\n");
        printf("4 - Para atualizar;\n");
        printf("5 - Para sair;\n");
        printf("Escolha: ");
        scanf("%d",&escolha);
        if(escolha==5)exit(0);
        if(escolha==1)
        {
            system("cls");
            strcpy(nometabbusca,"TABELAHUGO");
            fflush(stdin);
            printf("\n\nDigite a chave da tupla que deseja buscar: ");
            scanf("%d",&keyparabusca);
            buscar(nomesaida,keyparabusca,tab,nometabbusca);
            system("pause");
        }
        if(escolha==2)
        {
            system("cls");
            strcpy(nometabbusca,"TABELAHUGO");
            fflush(stdin);
            inseretupla(tab,nometabbusca,pagina,&pagina,nomesaida);
            system("pause");
        }
        if(escolha==3)
        {
            system("cls");
            //printf("\n\nDigite o nome da tabela: ");
            //scanf("%s",nometabbusca);
            strcpy(nometabbusca,"TABELAHUGO");
            fflush(stdin);
            printf("\n\nDigite a chave da tupla que deseja remover: ");
            scanf("%d",&keyparabusca);
            remover(nomesaida,keyparabusca,tab,nometabbusca);
            system("pause");
        }
        if(escolha == 4)
        {
            system("cls");
            strcpy(nometabbusca,"TABELAHUGO");
            fflush(stdin);
            printf("\n\nDigite a chave da tupla que deseja atualizar: ");
            scanf("%d",&keyparabusca);
            atualiza(nomesaida,keyparabusca,tab,nometabbusca,pagina,&pagina,nomesaida);

            system("pause");
        }
    }
    return 0;
}
