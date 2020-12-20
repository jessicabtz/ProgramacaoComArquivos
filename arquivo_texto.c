#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define TAM 50

typedef struct lista{

    char info[TAM];
    struct lista *prox;

}Lista;

Lista* inicia(void){
    return NULL;
}

Lista* insere(Lista* l, char *palavra){
    Lista* novo = (Lista*)malloc(sizeof(Lista));
    int i;
    strcpy(novo->info, palavra);
    novo->prox = l;
    return novo;
}

int Existe(Lista *l, char *palavra){

    if(l == NULL)
        return 0;
    else if(strcmp(l->info,palavra)== 0){
        return 1;
    }else
        return Existe(l->prox,palavra);
}

void imprime(Lista* l){
    Lista* p;
    for(p = l; p != NULL; p = p->prox){
        printf("%s\n",p->info);
    }
    printf("\n");
}

int marcador_fim(char c){//função retorna se o caracter é um marcador de fim de palavra ou nao
    if(c == ' ' || c == ',' || c == ';' || c == '.' || c == '!' || c == '?'|| c == EOF|| c == '\n')
        return 1;
    else
        return 0;
}

void mostra_palavras(FILE *arq){
    char c, str[TAM] = "";
    int i = 0;
    Lista *l;

    l = inicia();

    while(c!=EOF){
        c = tolower(fgetc(arq));

        if(marcador_fim(c) == 1){
            if(Existe(l,str) == 0 && strcmp(str,"")!= 0){
                l = insere(l,str);
            }
            strcpy(str,"");
            i = 0;
        }else{
            str[i] = c;
            str[i+1] = '\0';
            i++;
        }

    }
   imprime(l);
}

void main(){
    char nomearq[60];
    FILE *arq;

    printf("Insira o nome do arquivo\n");
    scanf("%s",nomearq);

    arq = fopen(nomearq,"rt");
    if(!arq)
        printf("Erro ao abrir o arquivo");
    else
        mostra_palavras(arq);

    fclose(arq);
}
