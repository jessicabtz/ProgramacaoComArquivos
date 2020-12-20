#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct reuniao{
    int id;
    int dia,mes,ano;
    int hora,minuto;
    int qtdPessoas;
}reuniao;

void main(){
    FILE *arq;
    FILE *arq2;
    FILE *arq3;
    reuniao *agenda;
    reuniao *auxiliar;
    int id, dado, dia, mes, ano, hora, minuto, pessoas;
    char op, campo[10];

    arq  = fopen("original.bin", "rb");
    if(!arq)
        printf("erro ao abrir o original.bin");
    arq2 = fopen("correcoes.txt", "rt");
    if(!arq2)
        printf("erro ao abrir o correcoes.txt");
    arq3 = fopen("corrigido.bin","wb");
    if(!arq3)
        printf("erro ao abrir o corrigido.bin");

    agenda = (reuniao*)malloc(sizeof(reuniao));
    auxiliar = (reuniao*)malloc(sizeof(reuniao));

    fread(agenda, sizeof(reuniao), 1, arq);
    fscanf(arq2, "%d %c", &id, &op);

    while(!feof(arq)|| !feof(arq2)){
        if(id > agenda->id){
            switch(op){
                case 'I':
                case 'E':
                case 'M':
                    fwrite(agenda, sizeof(reuniao), 1, arq3);
                    fread(agenda, sizeof(reuniao), 1, arq);
                    break;
            }
        }else if(id == agenda->id){
            switch(op){
                case 'I':
                    fscanf(arq2, " %d %d %d %d %d %d", &dia, &mes, &ano, &hora, &minuto,&pessoas);
                    printf("Registro %d ja existe. \n",id);
                    fscanf(arq2, "%d %c", &id, &op);
                    break;
                case 'E':
                    fread(agenda, sizeof(reuniao), 1, arq);
                    fscanf(arq2, "%d %c", &id, &op);
                    break;
                case 'M':
                    fscanf(arq2, " %s %d", &campo, &dado);
                    if(strcmp(campo,"HORA")==0)
                        agenda->hora = dado;
                    else if(strcmp(campo,"MINUTO")==0)
                        agenda->minuto = dado;
                    else if(strcmp(campo,"DIA")==0)
                        agenda->dia = dado;
                    else if(strcmp(campo,"MES")==0)
                        agenda->mes = dado;
                    else if(strcmp(campo,"ANO")==0)
                        agenda->ano = dado;
                    else if(strcmp(campo,"PESSOAS")==0)
                        agenda->qtdPessoas = dado;

                    fscanf(arq2, "%d %c", &id, &op);
                    if(id != agenda->id){
                        fwrite(agenda, sizeof(reuniao), 1, arq3);
                        fread(agenda, sizeof(reuniao), 1, arq);
                    }
                    break;
            }
        }else{
            switch(op){
                case 'I':
                    fscanf(arq2, " %d %d %d %d %d %d", &dia, &mes, &ano, &hora, &minuto,&pessoas);
                    auxiliar->id         = id;
                    auxiliar->dia        = dia;
                    auxiliar->mes        = mes;
                    auxiliar->ano        = ano;
                    auxiliar->hora       = hora;
                    auxiliar->minuto     = minuto;
                    auxiliar->qtdPessoas = pessoas;
                    fwrite(auxiliar, sizeof(reuniao), 1, arq3);
                    fscanf(arq2, "%d %c", &id, &op);
                    break;
                case 'E':
                    printf("Registro %d nao existe no arquivo\n", id);
                    fscanf(arq2, "%d %c", &id, &op);
                    break;
                case 'M':
                    fscanf(arq2, " %s %d", &campo, &dado);
                    printf("Registro %d nao existe no arquivo\n", id);
                    fscanf(arq2, "%d %c", &id, &op);
                    break;
            }
        }
        if(!feof(arq) && feof(arq2)){
            id = 9999999;
        }else if(feof(arq) && !feof(arq2)){
            agenda->id = 9999999;
        }
    }

    free(agenda);
    free(auxiliar);

    fclose(arq);
    fclose(arq2);
    fclose(arq3);
}
