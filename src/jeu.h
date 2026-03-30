#ifndef JEU_H
#define JEU_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

typedef struct {
    int      x, y;         // position personnage
    int      score;        // score actuel
    int      vie;          // nombre de vies
    int      termine;      // 1 = partie terminée
} Jeu;

void initJeu(Jeu *j);
void afficherJeu(SDL_Renderer *ren, Jeu *j, TTF_Font *font);
void updateJeu(Jeu *j);
void inputJeu(Jeu *j, SDL_Event *e);

#endif
