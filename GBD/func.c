#include "func.h"
unsigned char* insereInteiro4bits(unsigned char *pagina,int *posicao,int numero)
{
    int a = numero>>24;
    int b = numero>>16;
    int c = numero>>8;
    int d = numero;
    pagina[*posicao]= d - (c<<8);

    pagina[*posicao+1]= c - (b<<8);

    pagina[*posicao+2]= b - (a<<8);

    pagina[*posicao+3]= a;

    *posicao = *posicao+4;
    return pagina;
}

unsigned char* insereInteiro2bits(unsigned char*pagina,int *posicao,short int numero)
{
    pagina[*posicao+1]= numero>>8;
    pagina[*posicao]= numero - ((numero>>8)<<8);
    *posicao = *posicao+2;
    return pagina;
}

unsigned char* inserestring(unsigned char*pagina,int *posicao,char *stringa,int tamanho)
{
    int i=*posicao;
    for(i=0; i<tamanho; i++)pagina[i+(*posicao)]=stringa[i];
    *posicao+=i;
    return pagina;
}

int lerinteiro4bits(unsigned char* pagina,int *posicao)
{
    int aux=0;
    aux += pagina[*posicao];
    aux += (pagina[*posicao+1]<<8);
    aux += (pagina[*posicao+2]<<16);
    aux += (pagina[*posicao+3]<<24);
    *posicao+=4;
    return aux;
}
int lerinteiro4bitsEsp(unsigned char* pagina,short int *posicao)
{
    int aux=0;
    aux += pagina[*posicao];
    aux += (pagina[*posicao+1]<<8);
    aux += (pagina[*posicao+2]<<16);
    aux += (pagina[*posicao+3]<<24);
    *posicao+=4;
    return aux;
}

unsigned char* inserechar(unsigned char*pagina,int *posicao,char stringa)
{
    pagina[*posicao]=stringa;
    *posicao+=1;
    return pagina;
}

char lerchar(unsigned char*pagina,int *posicao)
{

    *posicao+=1;
    return pagina[*posicao-1];
}

short int lerinteiro2bits(unsigned char*pagina,int *posicao)
{

    short int aux=0;
    aux += pagina[(*posicao)];
    aux += (pagina[(*posicao)+1]<<8);
    *posicao+=2;
    return aux;
}


short int lerinteiro2bitsEsp(unsigned char*pagina,short int *posicao)
{

    short int aux=0;
    aux += pagina[(*posicao)];
    aux += (pagina[(*posicao)+1]<<8);
    *posicao+=2;
    return aux;
}

char* lerstring(unsigned char*pagina,int *posicao,short int tamanho)
{
    char *aux = (char*)malloc(sizeof(char)*(tamanho+1));
    int i;
    for(i=0; i<tamanho; i++)
        aux[i]=pagina[(*posicao)+i];
    aux[i]='\0';
    *posicao+=tamanho;
    return aux;
}

char* lerstringEsp(unsigned char*pagina,short int *posicao,short int tamanho)
{
    char *aux = (char*)malloc(sizeof(char)*(tamanho+1));
    int i;
    for(i=0; i<tamanho; i++)
        aux[i]=pagina[(*posicao)+i];
    aux[i]='\0';
    *posicao+=tamanho;
    return aux;
}
char* criaBD()
{
    FILE* fp;
    char *nome;
    nome = (char*)malloc(sizeof(char)*20);
    strcpy(nome,"Schema_BD_v1");
    fp = fopen("Catalogo.txt","w");
    if (fp == NULL) return NULL;
    fprintf(fp,nome);
    fclose(fp);
    return nome;

}

Tabela* defineBD(char* nomearq)
{
    FILE* fp,*cat;
    Tabela *tabela, *ant = NULL;
    int flag = 0, i = 0;
    char aux[20];
    fp = fopen(nomearq,"r");
    if (fp == NULL) return NULL;
    cat = fopen("Catalogo.txt","r+w");
    if (cat == NULL) return NULL;
    fseek(cat,0,SEEK_END);
    while(!feof(fp))
    {
        fscanf(fp,"%s",aux);
        fprintf(cat,"\n%s",aux);
        tabela = (Tabela*)malloc(sizeof(Tabela));
        strcpy(tabela->nome,aux);
        fscanf(fp,"%d",&tabela->ncampos);
        fprintf(cat," %d",tabela->ncampos);
        tabela->campos = (Campo*)malloc(sizeof(Campo)*tabela->ncampos);
        tabela->prox=ant;
        for(i = 0; i < tabela->ncampos; i++)
        {
            fscanf(fp,"%s",tabela->campos[i].nome);
            fprintf(cat,"\n%s ",tabela->campos[i].nome);
            fscanf(fp,"%s",aux);
            fprintf(cat," %s",aux);
            if(strcmp(aux,"varchar")==0) tabela->campos[i].dado.flag = 0;
            else if(strcmp(aux,"int")==0) tabela->campos[i].dado.flag = 1;
            else flag = 1;
            if (flag == 1)return NULL;;
        }
        ant = tabela;
    }
    fclose(cat);
    return tabela;
}

int carregaTabela(Tabela* tab,char *nomeArquivo,int pagina, int *ultimapagina,char *nomearq)
{
    FILE *entrada,*test;
    if((entrada=fopen(nomeArquivo,"r+"))==NULL)return -1;
    char corrigir[20];
    corrigir[0]='A';
    corrigir[1]='\0';
    strcat(corrigir,nomearq);
    if((test=fopen(nomearq,"wb+"))==NULL)return -1;
    Dado* info = (Dado*)malloc(sizeof(Dado)*(tab->ncampos));
    short int tamanho=0;
    Tabela* tb=tab;
    int calculo,i;
    char aux='0';
    char teste[50];
    int confirm;
    fscanf(entrada,"%s",teste);
    while(tb!=NULL && strcmp(tb->nome,teste)!=0)
    {
        tb=tab->prox;
    }
    unsigned char* PAG =(unsigned char*)malloc(sizeof(unsigned char)*512);
    for(i=0; i<512; i++)PAG[i]=0;
    int posicao=0;
    int posicaoaux=508;
    short int newend[2]= {0,0};
    if(tb==NULL) return -1;
    while(!feof(entrada))
    {
        tamanho = 0;
        for(i=0; i<(tb->ncampos); i++)
        {
            if (tb->campos[i].dado.flag == 1)
            {
                fscanf(entrada,"%d",&info[i].inteiro);
                tamanho+=4;
            }
            if (tb->campos[i].dado.flag == 0)
            {
                fscanf(entrada,"%hd",&info[i].tamanho);
                tamanho+=info[i].tamanho+1+sizeof(short int);       //2 do short int e 1 do /0
                info[i].varchar = (char*)malloc((sizeof(char)*info[i].tamanho)+1);
                fscanf(entrada,"%s",info[i].varchar);
            }
        }

        do
        {
            confirm=0;
            posicaoaux = 508;
            newend[0]=lerinteiro2bits(PAG,&posicaoaux);
            newend[1]=lerinteiro2bits(PAG,&posicaoaux);
            calculo=508-newend[1]-((newend[0]+1)*sizeof(int)*2)-tamanho;
            if(calculo<0)
            {
                if((test=fopen(nomearq,"r+w"))==NULL)return -1;
                fseek(test,(512*pagina),SEEK_SET);
                fwrite(PAG,1,sizeof(unsigned char)*512,test);
                fclose(test);
                pagina++;
                for(i=0; i<512; i++)PAG[i]=0;
                posicaoaux=508;
                newend[0]=0;
                newend[1]=0;
                confirm=1;
            }
        }
        while(confirm==1);
        posicao=newend[1];

        for(i=0; i<tb->ncampos; i++)
        {
            if(tb->campos[i].dado.flag==1)
            {
                PAG=insereInteiro4bits(PAG,&posicao,info[i].inteiro);
            }
            if(tb->campos[i].dado.flag==0)
            {
                PAG=insereInteiro2bits(PAG,&posicao,info[i].tamanho);
                PAG=inserestring(PAG,&posicao,info[i].varchar,info[i].tamanho);
                free(info[i].varchar);
            }
        }
        posicaoaux=505-(newend[0])*3;
        PAG = insereInteiro2bits(PAG,&posicaoaux,newend[1]);
        PAG = inserechar(PAG,&posicaoaux,aux);
        newend[0]=newend[0]+1;
        newend[1]=newend[1]+tamanho;
        posicaoaux=508;
        PAG=insereInteiro2bits(PAG,&posicaoaux,newend[0]);
        PAG=insereInteiro2bits(PAG,&posicaoaux,newend[1]);
        *ultimapagina=pagina;
    }
    if((test=fopen(nomearq,"rb+"))==NULL)return -1;
    fseek(test,(512*pagina),SEEK_SET);
    fwrite(PAG,1,sizeof(unsigned char)*512,test);
    fclose(test);
    return 0;
}

void inicializaarquivo(int n,char *nomearq)
{
    n=1;
    FILE* arq;
    arq=fopen(nomearq,"w+");
    int i;
    short int aux[2]= {0,0};
    for(i=0; i<n; i++)
    {
        fseek(arq,(512*i)+508,SEEK_SET);
        fwrite(&aux,sizeof(aux),1,arq);
    }
    fclose(arq);
}

void acrescentapagina(char *nomearq)
{
    FILE* arq;
    arq=fopen(nomearq,"r+");
    short int aux[2]= {0,0};
    fseek(arq,508,SEEK_END);
    fwrite(&aux,sizeof(aux),1,arq);
    fclose(arq);
}

int buscar(char*nomesaida,int key,Tabela *tab,char *nometabbusca)
{
    int tamanhoarq,npag,chave,i,j,aux,inteiro;
    short int nreg, inicioreg,tamanho;
    char *varchar;
    unsigned char* PAG = (unsigned char*)malloc(sizeof(unsigned char)*512);
    FILE *arq;
    if((arq=fopen(nomesaida,"rb+"))==NULL)return -1;
    Tabela*tb=tab;
    while(tb!=NULL && strcmp(tb->nome,nometabbusca)!=0)
    {
        tb=tab->prox;
    }

    fseek(arq,0,SEEK_END);
    tamanhoarq = ftell(arq);
    npag = tamanhoarq/512;


    for(i=0; i<npag; i++)
    {
        fseek(arq,i*512,SEEK_SET);
        fread(PAG,sizeof(unsigned char)*512,1,arq);

        aux =508;
        nreg = lerinteiro2bits(PAG,&aux);

        for(j=0; j<nreg; j++)
        {
            aux = 508-(j)*3;
            inicioreg = lerinteiro2bits(PAG,&aux);
            char flag = lerchar(PAG,&aux);
            chave = lerinteiro4bitsEsp(PAG,&inicioreg);

            inicioreg-=4;

            if(key == chave && flag == '0')
            {
                for(i=0; i<(tb->ncampos); i++)
                {
                    if (tb->campos[i].dado.flag == 1)
                    {
                        inteiro = lerinteiro4bitsEsp(PAG,&inicioreg);
                        printf("%d",inteiro);
                    }
                    if (tb->campos[i].dado.flag == 0)
                    {
                        tamanho = lerinteiro2bitsEsp(PAG,&inicioreg);

                        varchar = (char*)malloc((sizeof(char)*tamanho));

                        varchar=lerstringEsp(PAG,&inicioreg,tamanho);

                        printf("%d %s",tamanho,varchar);
                    }
                    printf(" ");
                }

                fclose(arq);
                return 0;
            }
        }
    }
    fclose(arq);
    printf("\n\nNao Encontrado %d \n",key);
    return 0;
}

int remover(char*nomesaida,int key,Tabela *tab,char *nometabbusca)
{
    int tamanhoarq,npag,chave,i,j,aux,inteiro;
    short int nreg, inicioreg,tamanho;
    char *varchar;
    unsigned char* PAG = (unsigned char*)malloc(sizeof(unsigned char)*512);
    FILE *arq;
    if((arq=fopen(nomesaida,"rb+"))==NULL)return -1;
    Tabela*tb=tab;
    while(tb!=NULL && strcmp(tb->nome,nometabbusca)!=0)
    {
        tb=tab->prox;
    }

    fseek(arq,0,SEEK_END);
    tamanhoarq = ftell(arq);
    npag = tamanhoarq/512;


    for(i=0; i<npag; i++)
    {
        fseek(arq,i*512,SEEK_SET);
        fread(PAG,sizeof(unsigned char)*512,1,arq);

        aux =508;
        nreg = lerinteiro2bits(PAG,&aux);

        for(j=0; j<nreg; j++)
        {
            aux = 508-(j)*3;
            inicioreg = lerinteiro2bits(PAG,&aux);
            char flag = lerchar(PAG,&aux);
            chave = lerinteiro4bitsEsp(PAG,&inicioreg);

            inicioreg-=4;

            if(key == chave && flag == '0')
            {
                flag = '1';
                aux--;
                PAG = inserechar(PAG,&aux,flag);
                fseek(arq,i*512,SEEK_SET);
                fwrite(PAG,sizeof(unsigned char)*512,1,arq);
                fclose(arq);
                return 0;
            }
        }
    }
    fclose(arq);
    printf("\n\nNao Encontrado %d \n",key);
    return 1;
}

int inseretupla(Tabela* tab,char *nomeArquivo,int pagina, int *ultimapagina,char *nomearq)
{
    FILE *test;
    char corrigir[20];
    corrigir[0]='A';
    corrigir[1]='\0';
    strcat(corrigir,nomearq);
    if((test=fopen(nomearq,"rb+"))==NULL)return -1;
    Dado* info = (Dado*)malloc(sizeof(Dado)*(tab->ncampos));
    short int tamanho=0;
    Tabela* tb=tab;
    int calculo,i;
    char aux='0';
    int confirm;
    //printf("AAAA");
    //system("pause");
    while(tb!=NULL && strcmp(tb->nome,nomeArquivo)!=0)
    {
        tb=tab->prox;
    }
    unsigned char* PAG =(unsigned char*)malloc(sizeof(unsigned char)*512);
    for(i=0; i<512; i++)PAG[i]=0;
    int posicao=0;
    int posicaoaux=508;
    short int newend[2]= {0,0};
    if(tb==NULL) return -1;
    tamanho = 0;
    for(i=0; i<(tb->ncampos); i++)
    {
        if (tb->campos[i].dado.flag == 1)
        {
            printf("Digite um inteiro: ");
            fscanf(stdin,"%d",&info[i].inteiro);
            tamanho+=4;
        }
        if (tb->campos[i].dado.flag == 0)
        {
            printf("Digite o tamanho do varchar: ");
            fscanf(stdin,"%hd",&info[i].tamanho);
            tamanho+=info[i].tamanho+1+sizeof(short int);       //2 do short int e 1 do /0
            info[i].varchar =(char*)malloc((sizeof(char)*info[i].tamanho)+1);
            printf("Digite a string: ");
            fscanf(stdin,"%s",info[i].varchar);
        }
    }
    fseek(test,(512*pagina),SEEK_SET);
    fread(PAG,sizeof(unsigned char)*512,1,test);
    do
    {
        confirm=0;
        posicaoaux = 508;
        newend[0]=lerinteiro2bits(PAG,&posicaoaux);
        newend[1]=lerinteiro2bits(PAG,&posicaoaux);
        calculo=508-newend[1]-((newend[0]+1)*sizeof(int)*2)-tamanho;
        if(calculo<0)
        {
            printf("DEU MERDA\n%d",pagina);
            system("pause");
            fseek(test,(512*pagina),SEEK_SET);
            fwrite(PAG,1,sizeof(unsigned char)*512,test);
            pagina++;
            for(i=0; i<512; i++)PAG[i]=0;
            posicaoaux=508;
            newend[0]=0;
            newend[1]=0;
            confirm=1;
        }
    }
    while(confirm==1);
    posicao=newend[1];
    for(i=0; i<tb->ncampos; i++)
    {
        if(tb->campos[i].dado.flag==1)
        {
            PAG=insereInteiro4bits(PAG,&posicao,info[i].inteiro);
        }
        if(tb->campos[i].dado.flag==0)
        {
            PAG=insereInteiro2bits(PAG,&posicao,info[i].tamanho);
            PAG=inserestring(PAG,&posicao,info[i].varchar,info[i].tamanho);
            free(info[i].varchar);
        }
    }

    posicaoaux=505-(newend[0])*3;
    PAG = insereInteiro2bits(PAG,&posicaoaux,newend[1]);
    PAG = inserechar(PAG,&posicaoaux,aux);
    newend[0]=newend[0]+1;
    newend[1]=newend[1]+tamanho;
    posicaoaux=508;
    PAG=insereInteiro2bits(PAG,&posicaoaux,newend[0]);
    PAG=insereInteiro2bits(PAG,&posicaoaux,newend[1]);
    *ultimapagina=pagina;
    if((test=fopen(nomearq,"rb+"))==NULL)return -1;
    fseek(test,(512*pagina),SEEK_SET);
    fwrite(PAG,1,sizeof(unsigned char)*512,test);
    fclose(test);
    return 0;

}

int atualiza(char*nomesaida,int key,Tabela *tab,char *nometabbusca, int pagina,int *ultimapagina,char *nomearq)
{
 remover(nomesaida,key,tab,nometabbusca);
 inseretupla(tab,nometabbusca,pagina,&pagina,nomesaida);

 return 0;
}

