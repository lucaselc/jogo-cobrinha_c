#include <jogo.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "cobra.h"
#define TAM 10
//função que cria a cobra e inicializa ela no meio da tela
Cobra* cria_cobra(){
    Cobra *cobra = (Cobra*)malloc(sizeof(Cobra));
    cobra->tamanho = 1;
    cobra->corpo = (Posicao*)malloc(sizeof(Posicao));
    cobra->corpo->x = largura_janela/2;
    cobra->corpo->y = altura_janela/2;
    return cobra;
}

//função que altera as coordenadas da cobra conforme a direção indicada
void move_cobra(Cobra *cobra, enum Direcao direcao){
    for(int i = cobra->tamanho - 1; i>0 ;i--){
        cobra->corpo[i] = cobra->corpo[i-1];
    }
    switch(direcao){
        case PARA_CIMA:
            cobra->corpo[0].y-=10;
            break;
        case PARA_BAIXO:
            cobra->corpo[0].y+=10;
            break;
        case PARA_ESQUERDA:
            cobra->corpo[0].x-=10;
            break;
        case PARA_DIREITA:
            cobra->corpo[0].x+=10;
            break;                  
    }
}

//função apenas para desenhar o corpo da cobra utilizando a função da biblioteca
void desenha_cobra(Cobra *cobra){
    cor(PRETO);
    for(int segmento = 1; segmento < cobra->tamanho; segmento++){
        Posicao *segmento_pos = &cobra->corpo[segmento];
        desenha_retangulo(segmento_pos->x, segmento_pos->y, TAM,TAM);
    }
    cor(VERMELHO_ESCURO);
    desenha_retangulo(cobra->corpo[0].x, cobra->corpo[0].y, TAM, TAM);
}

//função que é utilizada para descobrir a direção oposta da cobra, pois a intenção é que ela 
//não possa retornar para direção oposta da direção atual
enum Direcao oposta(enum Direcao direcao){
    switch(direcao){
    case PARA_CIMA:
        return PARA_BAIXO;
    case PARA_BAIXO:
        return PARA_CIMA;
    case PARA_DIREITA:
        return PARA_ESQUERDA;
    case PARA_ESQUERDA:
        return PARA_DIREITA;        
    }
}

//função que aumenta o corpo da cobra quando come uma maçã
void aumenta_cobra(Cobra *cobra){
    cobra->tamanho++; 
    cobra->corpo = realloc(cobra->corpo, sizeof(Posicao)* cobra->tamanho);
    cobra->corpo[cobra->tamanho-1].x = -100;
    cobra->corpo[cobra->tamanho-1].y = -100;  
}

//libera o espaço que foi alocado
void destroi_cobra(Cobra *cobra){
    free(cobra->corpo);
    free(cobra);
}