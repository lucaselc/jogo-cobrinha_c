#include <jogo.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

int main() {

    abre_janela(640, 480);

    while (janela_esta_aberta()) {
        cor(AMARELO);
        desenha_retangulo(0, 0, 320, 240);
        desenha_imagem(mouse_x, mouse_y, "assets/tux.png");

        fonte("assets/ubuntu.ttf", 32);
        desenha_texto(20, 20, "Esse é um teste!");

        if (tecla_apertada(ESC))
            janela_deve_fechar();

    }

    fecha_janela();

}