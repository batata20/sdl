#ifndef MENU_H
#define MENU_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

// Un bouton du menu
typedef struct {
    SDL_Rect  zone;
    char      label[50];
    int       actif;   // 1 = souris dessus
} Bouton;

// Le menu principal
typedef struct {
    Bouton boutons[4]; // Jouer, Options, Meilleurs Scores, Quitter
    int    nb_boutons;
    int    visible;
} Menu;

void initMenu(Menu *m);
void afficherMenu(SDL_Renderer *ren, Menu *m, TTF_Font *font);
int  inputMenu(Menu *m, SDL_Event *e); // retourne index bouton cliqué ou -1

#endif
