#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define TAM 50

typedef struct lista_docs{

    char nome[TAM];
    int  quant;
    struct lista_docs *prox;

}Lista_docs;

typedef struct docs{

    char nome[TAM];
    int  quant;

}Docs;

typedef struct lista{

    char         info[TAM];
    int          n_docs;
    Lista_docs   *l;
    struct lista *prox;

}Lista;

Lista* inicia(void){
    return NULL;
}

Lista_docs* inicia_docs(void){
    return NULL;
}

Lista_docs* insere_doc(Lista_docs* b, char *nome_arq, int rep){
    Lista_docs* novo = (Lista_docs*)malloc(sizeof(Lista_docs));
    strcpy(novo->nome, nome_arq);
    novo->quant = rep;
    novo->prox  = b;
    return novo;
}

Lista* insere(Lista* l, Lista_docs* b, char *palavra, int x, char *nome_arq, int rep){
    Lista* novo    = (Lista*)malloc(sizeof(Lista));
    strcpy(novo->info, palavra);
    novo->n_docs = x;
    novo->prox   = l;
    novo->l      = insere_doc(b,nome_arq, rep);
    return novo;
}

Lista* Existe(Lista *l, char *palavra){

    if(l == NULL)
        return NULL;
    else if(strcmp(l->info,palavra)== 0){
        return l;
    }else
        return Existe(l->prox,palavra);
}

int marcador_fim(char c){//função retorna se o caracter é um marcador de fim de palavra ou nao
    if(c == ' ' || c == ',' || c == ';' || c == '.' || c == '!' || c == '?'|| c == EOF|| c == '\n' || c == '(' || c == ')' || c == '[' || c == ']')
        return 1;
    else
        return 0;
}

int stopword(char *palavra, char vet[127][20]){
    int i, cont = 0;
    for(i=0; i<127; i++) {
        if(strcmp(palavra,vet[i])==0)
            cont++;
    }
    return cont;
}

void ordena(Lista_docs **l){
    if(l != NULL){
        Lista_docs *aux = *l, *t;
        int x;
        char str[20];

        while(aux != NULL) {
          t = aux->prox;
          while(t != NULL) {
            if(aux->quant < t->quant) {
                x = aux->quant;
                strcpy(str,aux->nome);
                aux->quant = t->quant;
                strcpy(aux->nome, t->nome);
                t->quant = x;
                strcpy(t->nome, str);
            }
            t = t->prox;
          }
          aux = aux->prox;

        }
    }
}

Lista* indexar(FILE *arq, char *nome, Lista *l, Lista_docs* b, char vet[127][20]){
    char c, str[TAM] = "";
    int i = 0;

    while(c!=EOF){
        c = tolower(fgetc(arq));

        if(marcador_fim(c) == 1){
            if(stopword(str,vet)==0){
                if(Existe(l,str) == NULL && strcmp(str,"")!= 0){
                    l = insere(l,b,str,1, nome, 1);
                }else if(Existe(l,str) != NULL && strcmp(str,"")!= 0){
                    Lista *aux = Existe(l,str);
                    if(strcmp(aux->l->nome,nome)==0)
                        aux->l->quant+=1;
                    else{
                        aux->l = insere_doc(aux->l, nome, 1);
                        aux->n_docs++;
                    }
                }
            }
            strcpy(str,"");
            i = 0;
        }else{
            str[i] = c;
            str[i+1] = '\0';
            i++;
        }

    }
    return l;
}

void buscaE_OU(char *palavra1,char *palavra2, Lista *l, int num){
    Lista *a, *b;
    Docs *vet1, *vet2;
    int i = 0, j = 0, cont = 0, cont1= 0, aux = 0;

    vet1 = (Docs*)malloc(num * sizeof(Docs));
    vet2 = (Docs*)malloc(num * sizeof(Docs));

    a = Existe(l, palavra1);
    b = Existe(l, palavra2);

    if(a == NULL || b == NULL)
        printf("Palavra(s) nao encontrada(s).\n");
    else{
        ordena(&a->l);//palavra1
        ordena(&b->l);//palavra2
        Lista_docs *c, *d;
        for(c = a->l; c!= NULL; c = c->prox){//palavra1
            strcpy(vet1[cont].nome,c->nome);
            vet1[cont].quant = c->quant;
            cont++;
        }

        for(d = b->l; d!= NULL; d = d->prox){//palavra2
            strcpy(vet2[cont1].nome,d->nome);
            vet2[cont1].quant = d->quant;
            cont1++;
        }

        printf("\nAs duas palavras aparecem juntas nos seguintes arquivos:\n\n");
        for(i = 0; i<cont; i++){//palavra1
            for(j = 0; j<cont1; j++){//palavra2
                if(strcmp(vet1[i].nome,vet2[j].nome)==0){
                    printf("%s\t",vet2[j].nome);
                    printf("%s: %d repeticoes, %s: %d repeticoes \n\n",  a->info, vet1[i].quant, b->info, vet2[j].quant);
                }
            }
        }

        printf("\nAs palavras aparecem juntas OU separadas nos seguintes arquivos:\n\n");
        for(i = 0; i < cont ;i++){
            printf("%s\n",vet1[i].nome);
        }
        for(i = 0; i < cont1; i++){
            aux = 0;
            for(j = 0; j < cont; j++){
                if(strcmp(vet1[j].nome,vet2[i].nome) == 0)
                    aux++;
            }
            if(aux == 0)
                printf("%s\n", vet2[i].nome);
        }
    }
}

void main(){
    FILE *arq1, *arq2;
    char nome_arq[15], str1[TAM], str2[TAM], vet[127][20];
    int op, i, n = 20;
    Lista *l;
    Lista_docs *b;
    l = inicia();
    b = inicia_docs();

    arq2 = fopen("stopwords.txt","rt");
    for(i = 0; i < 127; i++){
        fscanf(arq2,"%s", vet[i]);
    }

    for(i = 1; i<=n;i++){
        sprintf(nome_arq, "doc%d.txt",i);
        arq1 = fopen(nome_arq, "rt");
        if(!arq1)
            printf("Erro ao abrir arquivo %d.\n", i);
        else{
            l = indexar(arq1, nome_arq, l, b, vet);
            fclose(arq1);
        }
    }

    while(op!=3){
        printf("\nO que deseja fazer?\n1-Indexar arquivo.\n2-Procurar Palavra\n3-Sair\n");
        scanf("%d", &op);

        switch(op){
            case 1:
                printf("Digite o nome do arquivo com a extensao: ");
                scanf("%s", nome_arq);
                arq1 = fopen(nome_arq, "rt");
                if(!arq1)
                    printf("Erro ao abrir arquivo.\n");
                else{
                    l = indexar(arq1, nome_arq, l, b, vet);
                    n++;
                    fclose(arq1);
                }
                break;
            case 2:
                printf("Digite a primeira palavra que deseja buscar: ");
                scanf("%s", str1);
                printf("Digite a segunda palavra que deseja buscar: ");
                scanf("%s", str2);
                buscaE_OU(strlwr(str1), strlwr(str2), l, n);
                break;
            default:
                if(op!=3)
                    printf("Digite uma opcao valida.\n");
        }
    }
    free(l);
    free(b);
}


