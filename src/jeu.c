#include "jeu.h"
#include <stdio.h>

void initJeu(Jeu *j) {
    j->x       = 400;
    j->y       = 300;
    j->score   = 0;
    j->vie     = 3;
    j->termine = 0;
}

void afficherJeu(SDL_Renderer *ren, Jeu *j, TTF_Font *font) {
    SDL_Color blanc = {255, 255, 255, 255};
    SDL_Color jaune = {255, 215,   0, 255};
    SDL_Color rouge = {220,  50,  50, 255};

    // Personnage (carré bleu)
    SDL_SetRenderDrawColor(ren, 50, 100, 220, 255);
    SDL_Rect perso = {j->x - 20, j->y - 20, 40, 40};
    SDL_RenderFillRect(ren, &perso);

    // Score en haut à gauche
    char txtScore[50];
    sprintf(txtScore, "Score : %d", j->score);
    SDL_Surface *surf = TTF_RenderText_Solid(font, txtScore, jaune);
    SDL_Texture *tex  = SDL_CreateTextureFromSurface(ren, surf);
    SDL_Rect dest     = {10, 10, surf->w, surf->h};
    SDL_RenderCopy(ren, tex, NULL, &dest);
    SDL_FreeSurface(surf);
    SDL_DestroyTexture(tex);

    // Vies en haut à droite
    char txtVie[50];
    sprintf(txtVie, "Vies : %d", j->vie);
    surf = TTF_RenderText_Solid(font, txtVie, rouge);
    tex  = SDL_CreateTextureFromSurface(ren, surf);
    dest = (SDL_Rect){650, 10, surf->w, surf->h};
    SDL_RenderCopy(ren, tex, NULL, &dest);
    SDL_FreeSurface(surf);
    SDL_DestroyTexture(tex);

    // Instructions
    surf = TTF_RenderText_Solid(font,
           "Fleches = bouger | ESPACE = +10pts | Q = fin", blanc);
    tex  = SDL_CreateTextureFromSurface(ren, surf);
    dest = (SDL_Rect){100, 560, surf->w, surf->h};
    SDL_RenderCopy(ren, tex, NULL, &dest);
    SDL_FreeSurface(surf);
    SDL_DestroyTexture(tex);
}

void updateJeu(Jeu *j) {
    // Limites de l'écran
    if (j->x < 20)  j->x = 20;
    if (j->x > 780) j->x = 780;
    if (j->y < 20)  j->y = 20;
    if (j->y > 560) j->y = 560;

    // Partie terminée si plus de vies
    if (j->vie <= 0) j->termine = 1;
}

void inputJeu(Jeu *j, SDL_Event *e) {
    if (e->type == SDL_KEYDOWN) {
        switch (e->key.keysym.sym) {
            case SDLK_LEFT:  j->x -= 10; break;
            case SDLK_RIGHT: j->x += 10; break;
            case SDLK_UP:    j->y -= 10; break;
            case SDLK_DOWN:  j->y += 10; break;

            // ESPACE = gagner des points
            case SDLK_SPACE:
                j->score += 10;
                break;

            // Q = terminer la partie manuellement
            case SDLK_q:
                j->termine = 1;
                break;
        }
    }
}
