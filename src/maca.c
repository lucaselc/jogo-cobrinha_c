#include <jogo.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "cobra.h"
#include "maca.h"

void gera_maca(Posicao *maca){
    maca->x = (rand() % (largura_janela / 10))*10;
    maca->y = (rand() % (altura_janela / 10))*10;
}

int comeu_maca(Cobra *cobra, Posicao *maca){
    if(cobra->corpo->x == maca->x && cobra->corpo->y == maca->y)
        return 1;
    else{ 
        return 0;
    }
}