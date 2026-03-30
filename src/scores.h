#ifndef SCORES_H
#define SCORES_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#define MAX_SCORES 5
#define FICHIER_SCORES "scores.txt"

typedef struct {
    char nom[50];
    int  score;
} ScoreEntry;

typedef struct {
    ScoreEntry liste[MAX_SCORES];
    int        nb;
    int        visible;
    char       nomSaisi[50];
    int        enSaisie;
} SousMenuScores;

void initSousMenuScores(SousMenuScores *sm);
void chargerScores(SousMenuScores *sm);
void sauvegarderScore(SousMenuScores *sm, char *nom, int score);
void trierScores(SousMenuScores *sm);
void afficherSousMenuScores(SDL_Renderer *ren, SousMenuScores *sm, TTF_Font *font);
void inputSousMenuScores(SousMenuScores *sm, SDL_Event *e, int scoreJoueur);

#endif
