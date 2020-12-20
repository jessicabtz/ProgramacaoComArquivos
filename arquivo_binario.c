#include <stdio.h>
#include <stdlib.h>

#pragma pack(2)

int qtd_elementos(FILE *arq){
    int num;

    fseek(arq,0,SEEK_END);
    num = ftell(arq)/(sizeof(float));

    return num;
}

float* ler_binario(FILE *arq, int num){
    int i, tam;
    float *v;

    v = (float *) malloc(num * sizeof(float));

    fseek(arq,0,SEEK_SET);
    fread(v,sizeof(float),num,arq);

    return v;
}

void main(){
    FILE  *f1 , *f2, *arq;
    int   *v  , tam1, tam2, i = 0, j,cont = 0, aux, x = -1;
    float *v1 , *v2;

    f1   = fopen("arq1.bin", "rb");
    tam1 = qtd_elementos(f1);
    v1   = ler_binario(f1,tam1);
    fclose(f1);

    f2   = fopen("arq2.bin", "rb");
    tam2 = qtd_elementos(f2);
    v2   = ler_binario(f2,tam2);
    fclose(f2);

    arq  = fopen("resultado.bin", "wb");
    fwrite(v1,sizeof(float),tam1,arq);
    fwrite(v2,sizeof(float),tam2, arq);

    aux = tam1 - tam2;
    while(i <= aux){
        cont = 0;
        for(j = 0; j < tam2; j++){
            if(v1[i+j] == v2[j]){
                ++cont;
            }
        }
        if(cont == tam2)
            x = i;
        i++;
    }
    fwrite(&x, sizeof(int),1,arq);
    fclose(arq);

    free(v1);
    free(v2);
}

