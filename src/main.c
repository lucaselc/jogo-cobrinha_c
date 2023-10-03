#include <jogo.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "cobra.h"
#include "maca.h"
#define TAM 10
#define maxJogadores 5

int metade_largura, metade_altura;

typedef struct{
    char nick[20];
    int pontuacao;
}Jogador;

int tela_incial(){
    int DELAY=0;
    while (janela_esta_aberta()){
        desenha_imagem(metade_largura,metade_altura, "assets/fund.png");
        cor(PRETO);
        fonte("assets/fonte1.ttf", 60);
        desenha_texto(180, 240/2, "SNAKE");
        cor(PRETO);
        fonte("assets/fonte1.ttf", 30);
        desenha_texto(140, 250, "ESCOLHA O MODO:");
        fonte("assets/fonte1.ttf", 16);
        desenha_retangulo(175,300, 80,40);
        cor(VERDE_CLARO);
        desenha_texto(180, 325, "Lento");
        cor(PRETO);
        desenha_texto(195, 360, "(F1)");
        desenha_texto(297, 360, "(F2)");
        desenha_texto(397, 360, "(F3)");
        desenha_retangulo(280,300, 80,40);
        cor(VERDE_CLARO);
        desenha_texto(285, 325, "Suave");
        cor(PRETO);
        desenha_retangulo(380,300, 80,40);
        cor(VERDE_CLARO);
        desenha_texto(385, 325, "Veloz");
        //CONTROLAR A VELOCIDADE DA COBRA
        if(tecla_acabou_de_ser_apertada(F1)){
            DELAY = 5;
            break;
        }
        if(tecla_acabou_de_ser_apertada(F2)){
            DELAY = 3;
            break;
        }
        if(tecla_acabou_de_ser_apertada(F3)){
            DELAY = 2;
            break;
        }
        if (tecla_acabou_de_ser_apertada(ESC)) {
        janela_deve_fechar();
        }
    }
    return DELAY;
}
int tela_jogo(int velocidade){
    Posicao maca;
    int score = 0;
    enum Direcao direcao_atual = PARA_DIREITA;
    gera_maca(&maca);
    Cobra *cobra = cria_cobra();
    int temporizador = 0, DELAY = velocidade;
    while (janela_esta_aberta()){
        desenha_imagem(metade_largura,metade_altura, "assets/fund.png");
        cor(PRETO);
        fonte("assets/fonte1.ttf", 20);
        desenha_texto(10, 25, "SCORE %d", score);
        //para verificar qual será a proxima direção
        enum Direcao nova_direcao = direcao_atual;
        if(tecla_pressionada('W')){
            nova_direcao = PARA_CIMA;
        }
        if(tecla_pressionada('S')){
            nova_direcao = PARA_BAIXO;
        }
        if(tecla_pressionada('A')){
            nova_direcao = PARA_ESQUERDA;
        }
        if(tecla_pressionada('D')){
            nova_direcao = PARA_DIREITA;
        }
        //PARA EVITAR QUE A COBRA CONSIGA IR PARA DIREÇÃO OPOSTA
        if(temporizador == 0){
            if(nova_direcao != oposta(direcao_atual))
                direcao_atual = nova_direcao;
            move_cobra(cobra, direcao_atual);
        }
        //colisao da propria cobra
        for(int i=1; i < cobra->tamanho; i++){
            if(cobra->corpo[0].x == cobra->corpo[i].x && cobra->corpo[0].y == cobra->corpo[i].y){
                goto perdeu;
            }
        }
        //colisao da cobra com as paredaes
        if(cobra->corpo[0].x > largura_janela || cobra->corpo[0].y > altura_janela || cobra->corpo[0].x < 0 || cobra->corpo[0].y <0){
            break;
        }
        // velocidade
        temporizador = (temporizador + 1) % DELAY;        
        desenha_cobra(cobra);
        cor(VERMELHO);
        //desenhar a maçã
        desenha_retangulo(maca.x, maca.y, 10 , 10);
        //função que verifica se a cobra comeu a maçã, se sim , gerar uma nova 
        // e aumentar mais um segmento
        if(comeu_maca(cobra,&maca)){
            toca_som("assets/maca.mp3");
            gera_maca(&maca);
            aumenta_cobra(cobra);
            if(DELAY==5)
            score += 1;
            if(DELAY==3)
            score +=2;
            if(DELAY==2)
            score +=5;
        }
        if (tecla_acabou_de_ser_apertada(ESC)) {
            janela_deve_fechar();
        }
    }
    perdeu:
        destroi_cobra(cobra);
        return score;             
}

void tela_ranking(Jogador *jogadores, int numJogadores){
    while (janela_esta_aberta()){
        desenha_imagem(largura_janela/2, altura_janela/2, "assets/fundo.rank.jpg");
        fonte("assets/fonte1.ttf", 40);
        desenha_texto(105, 110,"RANKING GERAL");
        for(int i = 0; i < numJogadores; i++){
            fonte("assets/fonte1.ttf", 20);
            desenha_texto(240, 180 + 30*i, "%s %d", jogadores[i].nick, jogadores[i].pontuacao);
        }
        if(tecla_acabou_de_ser_apertada(ESPACO)){
            return;
        }
        if (tecla_acabou_de_ser_apertada(ESC)) {
            janela_deve_fechar();
        }
    }
}

void escreveRanking(Jogador *jogadores, Jogador jogadorAtual, int numJogadores){
    int menorIdx, menor = jogadorAtual.pontuacao;
    FILE *placar = fopen("assets/ranking.txt", "w");
    for(int i=0; i<numJogadores; i++){
        fprintf(placar ,"%s %d\n", jogadores[i].nick , jogadores[i].pontuacao);
    }
    fclose(placar);
     if (tecla_acabou_de_ser_apertada(ESC)) {
            janela_deve_fechar();
        }
}

int verifica_se_maior(Jogador* x, Jogador* y){
    return y->pontuacao-x->pontuacao;
}

void tela_fim_jogo(int score, Jogador *jogadores, int numJogadores, Jogador jogadorAtual){
    //bool deu_gameover = false ;
    para_som("assets/entrada.mp3");
    toca_som("assets/gameover.mp3");
    jogadores[numJogadores] = jogadorAtual;
    numJogadores++;
    qsort(jogadores,numJogadores,sizeof(Jogador), (__compar_fn_t) verifica_se_maior);
    escreveRanking(jogadores,jogadorAtual,numJogadores);
    while (janela_esta_aberta()){
        desenha_imagem(metade_largura,metade_altura, "assets/gameover.png");
        cor(BRANCO);
        fonte("assets/fonte1.ttf", 20);
        desenha_texto(205, 280, "SEU SCORE: %d", score);
        fonte("assets/fonte1.ttf", 15);
        desenha_texto(190, 380, "VER RANKING GERAL?");
        fonte("assets/fonte1.ttf", 10);
        desenha_texto(285, 400, "(F5)");
        fonte("assets/fonte1.ttf", 15);
        desenha_texto(192, 435, "JOGAR NOVAMENTE?");
        fonte("assets/fonte1.ttf", 10);
        desenha_texto(272, 460, "(ESPACO)");
        if(tecla_acabou_de_ser_apertada(ESPACO)){
            toca_som_em_loop("assets/entrada.mp3");
            return;
        }
        else if(tecla_acabou_de_ser_apertada(F5)){
            tela_ranking(jogadores, numJogadores);
        }
        if (tecla_acabou_de_ser_apertada(ESC)) {
        janela_deve_fechar();
        }
    }
        
 }


int main() {
    Jogador jogadores[maxJogadores+1], jogadorAtual = {.pontuacao = 0};
    printf("Digite o seu nick: \n");
    scanf("%s", jogadorAtual.nick);
    int numJogadores = 0;
    FILE *placar = fopen("assets/ranking.txt", "r");
    if(placar == NULL){
        printf("Erro ao abrir o arquivo\n");
    }
    else{
        while(!feof(placar) && numJogadores < maxJogadores){
        fscanf(placar, "%s %d", jogadores[numJogadores].nick, &jogadores[numJogadores].pontuacao);
        numJogadores++;
        }
    }
    fclose(placar);
    numJogadores--; 
    abre_janela(640, 480);
    toca_som_em_loop("assets/entrada.mp3");
    metade_altura = altura_janela/2;
    metade_largura = largura_janela/2;
    srand(time(NULL));

    while (janela_esta_aberta()){
        int velocidade = tela_incial();
        int score = tela_jogo(velocidade);
        jogadorAtual.pontuacao = score;
        tela_fim_jogo(score, jogadores, numJogadores, jogadorAtual);
        if (tecla_acabou_de_ser_apertada(ESC)) {
            janela_deve_fechar();
        }
    }
    
}