#ifndef COBRA_H
#define COBRA_H
typedef struct{
    int x,y;
}Posicao;

typedef struct{
    int tamanho;
    Posicao *corpo;
}Cobra;

enum Direcao{
    PARA_CIMA,
    PARA_BAIXO,
    PARA_ESQUERDA,
    PARA_DIREITA
};


Cobra* cria_cobra();
void move_cobra(Cobra *cobra, enum Direcao direcao);
void desenha_cobra(Cobra *cobra);
enum Direcao oposta(enum Direcao direcao);
void aumenta_cobra(Cobra *cobra);
void destroi_cobra(Cobra *cobra);

#endif