#include<stdio.h>
#include<stdlib.h>

typedef struct celula
{
    void *info;
    struct celula* urm;
}TCelula,*TLista;

typedef struct
{
    int sursa;
    int destinatie;
}TInfo_muchie;

typedef struct
{
    int n;//numarul de noduri
    int m;//numarul de muchii
    int k;//numarul de culori
    TLista muchii;//lista de muchii
}TGraf;

TGraf *citire_graf()
{
    FILE *in=fopen("test.in","r");
    TGraf *graf=(TGraf*)malloc(sizeof(TGraf));
    fscanf(in,"%d",&graf->n);
    fscanf(in,"%d",&graf->m);
    fscanf(in,"%d",&graf->k);
    graf->muchii=NULL;
    TLista *aL=&graf->muchii;
    for(int i=0;i<graf->m;i++)
    {
        TInfo_muchie* info_muchie=(TInfo_muchie*)malloc(sizeof(TInfo_muchie));
        fscanf(in,"%d",&info_muchie->sursa);
        fscanf(in,"%d",&info_muchie->destinatie);
        *aL=(TLista)malloc(sizeof(TCelula));
        (*aL)->info=info_muchie;
        (*aL)->urm=NULL;
        aL=&((*aL)->urm);
    }
    fclose(in);
    return graf;
}

void scriere_expresie(TGraf *graf)
{
    FILE *out=fopen("test.out","w");

//^(din v)(V(din c)x indice k*v+c ) cu v=0:n-1 si c=0:k-1
    for(int v=0;v<graf->n;v++)
    {
        if(graf->k>1)
            fprintf(out,"(");
        for(int c=0;c<graf->k;c++)
        {
            fprintf(out,"x%d",graf->k*v+c);
            if(c!=graf->k-1)
                fprintf(out,"V");
        }
        if(graf->k>1)
            fprintf(out,")");
        if(v!=graf->n-1)
            fprintf(out,"^");
    }
    if(graf->n&&graf->k>=2)
        fprintf(out,"^");
//^(din v)(^(din i si j) ~(x indice k*v+i) V ~(x indice k*v+j))
//cu v=0:n-1, i=0:k-2, j=i+1:k-1
    for(int v=0;v<graf->n;v++)
    {
        if(graf->k<2) break;
        for(int i=0;i<graf->k-1;i++)
            for(int j=i+1;j<graf->k;j++)
                {fprintf(out,"(~x%dV~x%d)",graf->k*v+i,graf->k*v+j);
                if(i!=graf->k-2)
                    fprintf(out,"^");}
        if(v!=graf->n-1)
            fprintf(out,"^");
    }

    if(graf->m)
        fprintf(out,"^");
//^(din muchie)(^(din c) ~(x indice k*s+c) V ~(k*d+c))
//cu muchie in lista graf->muchii (de lungime m), c=0:k-1
//iar s-numarul nodului sursa din muchie si 
//d-numarul nodului destinatie din muchie
    for(TLista m=graf->muchii;m;m=m->urm)
    {
        TInfo_muchie* info=(TInfo_muchie*)m->info;
        for(int c=0;c<graf->k;c++)
            {fprintf(out,"(~x%dV~x%d)",graf->k*info->sursa+c,graf->k*info->destinatie+c);
            if(c!=graf->k-1)
                fprintf(out,"^");}
        if(m->urm)
            fprintf(out,"^");
    }
    fprintf(out,"\n");
    fclose(out);
}

int main()
{
    TGraf *graf=citire_graf();
    scriere_expresie(graf);
    return 0;
}
