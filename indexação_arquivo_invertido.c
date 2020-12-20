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

void busca(char *palavra, Lista *l){
    Lista *aux;
    aux = Existe(l, palavra);
    if(aux == NULL)
        printf("Palavra nao existe nos arquivos.\n");
    else{
        ordena(&aux->l);
        Lista_docs *d;
        for(d = aux->l; d!= NULL; d = d->prox){
            printf("%s\t", d->nome);
            printf("%d\n", d->quant);
        }
    }
}

void main(){
    FILE *arq1, *arq2;
    char nome_arq[15], str[TAM], vet[127][20];
    int op, i;
    Lista *l;
    Lista_docs *b;
    l = inicia();
    b = inicia_docs();

    arq2 = fopen("stopwords.txt","rt");
    for(i = 0; i < 127; i++){
        fscanf(arq2,"%s", vet[i]);
    }

    for(i = 1; i<=20;i++){
        sprintf(nome_arq, "doc%d.txt",i);
        arq1 = fopen(nome_arq, "rt");
        if(!arq1)
            printf("Erro ao abrir arquivo %d .\n", i);
        else{
        l = indexar(arq1, nome_arq, l, b, vet);
        fclose(arq1);
        }
    }

    while(op!=3){
        printf("O que deseja fazer?\n1-Indexar arquivo.\n2-Procurar Palavra\n3-Sair\n");
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
                    fclose(arq1);
                }
                break;
            case 2:
                printf("Digite a palavra que deseja buscar: ");
                scanf("%s", str);
                busca(strlwr(str),l);
                break;
            default:
                if(op!=3)
                    printf("Digite uma opccao valida");
        }
    }
    free(l);
    free(b);
}

