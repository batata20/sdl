#include "scores.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void initSousMenuScores(SousMenuScores *sm) {
    sm->nb       = 0;
    sm->visible  = 0;
    sm->enSaisie = 0;
    memset(sm->nomSaisi, 0, sizeof(sm->nomSaisi));
    chargerScores(sm);
}

void chargerScores(SousMenuScores *sm) {
    sm->nb = 0;
    FILE *f = fopen(FICHIER_SCORES, "r");
    if (!f) return;
    while (sm->nb < MAX_SCORES &&
           fscanf(f, "%49s %d",
                  sm->liste[sm->nb].nom,
                  &sm->liste[sm->nb].score) == 2) {
        sm->nb++;
    }
    fclose(f);
    trierScores(sm);
}

void trierScores(SousMenuScores *sm) {
    for (int i = 0; i < sm->nb - 1; i++) {
        for (int j = i + 1; j < sm->nb; j++) {
            if (sm->liste[j].score > sm->liste[i].score) {
                ScoreEntry tmp = sm->liste[i];
                sm->liste[i]   = sm->liste[j];
                sm->liste[j]   = tmp;
            }
        }
    }
}

void sauvegarderScore(SousMenuScores *sm, char *nom, int score) {
    if (sm->nb < MAX_SCORES) {
        strcpy(sm->liste[sm->nb].nom, nom);
        sm->liste[sm->nb].score = score;
        sm->nb++;
    } else {
        if (score > sm->liste[MAX_SCORES - 1].score) {
            strcpy(sm->liste[MAX_SCORES - 1].nom, nom);
            sm->liste[MAX_SCORES - 1].score = score;
        }
    }
    trierScores(sm);

    FILE *f = fopen(FICHIER_SCORES, "w");
    if (!f) return;
    for (int i = 0; i < sm->nb; i++)
        fprintf(f, "%s %d\n", sm->liste[i].nom, sm->liste[i].score);
    fclose(f);
}

void afficherSousMenuScores(SDL_Renderer *ren,
                             SousMenuScores *sm,
                             TTF_Font *font) {
    if (!sm->visible) return;

    SDL_Color blanc = {255, 255, 255, 255};
    SDL_Color jaune = {255, 215,   0, 255};
    SDL_Color rouge = {220,  50,  50, 255};

    // Fond
    SDL_SetRenderDrawBlendMode(ren, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(ren, 0, 0, 0, 210);
    SDL_Rect fond = {150, 80, 500, 440};
    SDL_RenderFillRect(ren, &fond);

    // Bordure dorée
    SDL_SetRenderDrawColor(ren, 255, 215, 0, 255);
    SDL_RenderDrawRect(ren, &fond);

    // Titre
    SDL_Surface *surf = TTF_RenderText_Solid(font,
                        "=== MEILLEURS SCORES ===", jaune);
    SDL_Texture *tex  = SDL_CreateTextureFromSurface(ren, surf);
    SDL_Rect dest     = {220, 100, surf->w, surf->h};
    SDL_RenderCopy(ren, tex, NULL, &dest);
    SDL_FreeSurface(surf);
    SDL_DestroyTexture(tex);

    // Liste
    if (sm->nb == 0) {
        surf = TTF_RenderText_Solid(font, "Aucun score enregistre.", blanc);
        tex  = SDL_CreateTextureFromSurface(ren, surf);
        dest = (SDL_Rect){220, 200, surf->w, surf->h};
        SDL_RenderCopy(ren, tex, NULL, &dest);
        SDL_FreeSurface(surf);
        SDL_DestroyTexture(tex);
    } else {
        for (int i = 0; i < sm->nb; i++) {
            char ligne[100];
            sprintf(ligne, "%d.  %-20s   %d pts",
                    i + 1, sm->liste[i].nom, sm->liste[i].score);
            surf = TTF_RenderText_Solid(font, ligne, blanc);
            tex  = SDL_CreateTextureFromSurface(ren, surf);
            dest = (SDL_Rect){190, 170 + i * 45, surf->w, surf->h};
            SDL_RenderCopy(ren, tex, NULL, &dest);
            SDL_FreeSurface(surf);
            SDL_DestroyTexture(tex);
        }
    }

    // Saisie nom
    if (sm->enSaisie) {
        char prompt[100];
        sprintf(prompt, "Entrez votre nom : %s_", sm->nomSaisi);
        surf = TTF_RenderText_Solid(font, prompt, rouge);
        tex  = SDL_CreateTextureFromSurface(ren, surf);
        dest = (SDL_Rect){180, 420, surf->w, surf->h};
        SDL_RenderCopy(ren, tex, NULL, &dest);
        SDL_FreeSurface(surf);
        SDL_DestroyTexture(tex);
    }

    // Instruction
    surf = TTF_RenderText_Solid(font, "ECHAP = fermer", blanc);
    tex  = SDL_CreateTextureFromSurface(ren, surf);
    dest = (SDL_Rect){300, 475, surf->w, surf->h};
    SDL_RenderCopy(ren, tex, NULL, &dest);
    SDL_FreeSurface(surf);
    SDL_DestroyTexture(tex);
}

void inputSousMenuScores(SousMenuScores *sm,
                          SDL_Event *e,
                          int scoreJoueur) {
    if (!sm->visible) return;

    if (e->type == SDL_KEYDOWN) {
        if (e->key.keysym.sym == SDLK_ESCAPE) {
            sm->visible  = 0;
            sm->enSaisie = 0;
            memset(sm->nomSaisi, 0, sizeof(sm->nomSaisi));
        }

        if (sm->enSaisie) {
            SDL_Keycode k = e->key.keysym.sym;

            if (k == SDLK_BACKSPACE) {
                int len = strlen(sm->nomSaisi);
                if (len > 0) sm->nomSaisi[len - 1] = '\0';
            }
            else if (k == SDLK_RETURN && strlen(sm->nomSaisi) > 0) {
                sauvegarderScore(sm, sm->nomSaisi, scoreJoueur);
                sm->enSaisie = 0;
                memset(sm->nomSaisi, 0, sizeof(sm->nomSaisi));
            }
            else if (((k >= SDLK_a && k <= SDLK_z) ||
                       (k >= SDLK_0 && k <= SDLK_9)) &&
                      strlen(sm->nomSaisi) < 20) {
                int len = strlen(sm->nomSaisi);
                sm->nomSaisi[len]     = (char)k;
                sm->nomSaisi[len + 1] = '\0';
            }
        }
    }
}
