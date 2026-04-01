#include "scores.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// ── Obtenir la date du jour ───────────────────────────────
static void obtenirDate(char *buffer) {
    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);
    strftime(buffer, 20, "%d/%m/%Y", tm_info);
}

void initSousMenuScores(SousMenuScores *sm) {
    sm->nb           = 0;
    sm->visible      = 0;
    sm->enSaisie     = 0;
    sm->retourActif  = 0;
    sm->rangJoueur   = -1;
    sm->boutonRetour = (SDL_Rect){325, 510, 150, 45};
    memset(sm->nomSaisi, 0, sizeof(sm->nomSaisi));
    chargerScores(sm);
}

void chargerScores(SousMenuScores *sm) {
    sm->nb = 0;
    FILE *f = fopen(FICHIER_SCORES, "r");
    if (!f) return;
    while (sm->nb < MAX_SCORES &&
           fscanf(f, "%49s %d %19s",
                  sm->liste[sm->nb].nom,
                  &sm->liste[sm->nb].score,
                  sm->liste[sm->nb].date) == 3) {
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
    char date[20];
    obtenirDate(date);

    if (sm->nb < MAX_SCORES) {
        strcpy(sm->liste[sm->nb].nom,  nom);
        strcpy(sm->liste[sm->nb].date, date);
        sm->liste[sm->nb].score = score;
        sm->nb++;
    } else {
        if (score > sm->liste[MAX_SCORES - 1].score) {
            strcpy(sm->liste[MAX_SCORES - 1].nom,  nom);
            strcpy(sm->liste[MAX_SCORES - 1].date, date);
            sm->liste[MAX_SCORES - 1].score = score;
        }
    }
    trierScores(sm);

    // Trouver le rang du joueur
    sm->rangJoueur = -1;
    for (int i = 0; i < sm->nb; i++) {
        if (strcmp(sm->liste[i].nom, nom) == 0 &&
            sm->liste[i].score == score) {
            sm->rangJoueur = i + 1;
            break;
        }
    }

    // Sauvegarder dans le fichier
    FILE *f = fopen(FICHIER_SCORES, "w");
    if (!f) return;
    for (int i = 0; i < sm->nb; i++)
        fprintf(f, "%s %d %s\n",
                sm->liste[i].nom,
                sm->liste[i].score,
                sm->liste[i].date);
    fclose(f);
}

void afficherSousMenuScores(SDL_Renderer *ren,
                             SousMenuScores *sm,
                             TTF_Font *font) {
    if (!sm->visible) return;

    SDL_Color blanc  = {255, 255, 255, 255};
    SDL_Color jaune  = {255, 215,   0, 255};
    SDL_Color rouge  = {220,  50,  50, 255};
    SDL_Color vert   = { 50, 200,  50, 255};
    SDL_Color gris   = {180, 180, 180, 255};

    // Fond
    SDL_SetRenderDrawBlendMode(ren, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(ren, 0, 0, 0, 220);
    SDL_Rect fond = {100, 60, 600, 510};
    SDL_RenderFillRect(ren, &fond);

    // Bordure dorée
    SDL_SetRenderDrawColor(ren, 255, 215, 0, 255);
    SDL_RenderDrawRect(ren, &fond);

    // Titre
    SDL_Surface *surf = TTF_RenderText_Solid(font,
                        "=== MEILLEURS SCORES ===", jaune);
    SDL_Texture *tex  = SDL_CreateTextureFromSurface(ren, surf);
    SDL_Rect dest     = {220, 75, surf->w, surf->h};
    SDL_RenderCopy(ren, tex, NULL, &dest);
    SDL_FreeSurface(surf);
    SDL_DestroyTexture(tex);

    // En-tête colonnes
    surf = TTF_RenderText_Solid(font,
           "Rang   Nom                  Score      Date", gris);
    tex  = SDL_CreateTextureFromSurface(ren, surf);
    dest = (SDL_Rect){120, 120, surf->w, surf->h};
    SDL_RenderCopy(ren, tex, NULL, &dest);
    SDL_FreeSurface(surf);
    SDL_DestroyTexture(tex);

    // Ligne séparatrice
    SDL_SetRenderDrawColor(ren, 255, 215, 0, 255);
    SDL_RenderDrawLine(ren, 110, 148, 690, 148);

    // Liste des scores avec rang et date
    if (sm->nb == 0) {
        surf = TTF_RenderText_Solid(font,
               "Aucun score enregistre.", blanc);
        tex  = SDL_CreateTextureFromSurface(ren, surf);
        dest = (SDL_Rect){220, 200, surf->w, surf->h};
        SDL_RenderCopy(ren, tex, NULL, &dest);
        SDL_FreeSurface(surf);
        SDL_DestroyTexture(tex);
    } else {
        for (int i = 0; i < sm->nb; i++) {
            // Mettre en surbrillance le rang du joueur
            SDL_Color couleur = blanc;
            if (sm->rangJoueur == i + 1)
                couleur = jaune;

            char ligne[100];
            sprintf(ligne, " #%d     %-20s   %5d pts   %s",
                    i + 1,
                    sm->liste[i].nom,
                    sm->liste[i].score,
                    sm->liste[i].date);

            surf = TTF_RenderText_Solid(font, ligne, couleur);
            tex  = SDL_CreateTextureFromSurface(ren, surf);
            dest = (SDL_Rect){115, 160 + i * 45,
                              surf->w, surf->h};
            SDL_RenderCopy(ren, tex, NULL, &dest);
            SDL_FreeSurface(surf);
            SDL_DestroyTexture(tex);
        }
    }

    // Afficher le rang du joueur après saisie
    if (sm->rangJoueur > 0 && !sm->enSaisie) {
        char txtRang[80];
        sprintf(txtRang, "Votre rang : #%d  Felicitations !",
                sm->rangJoueur);
        surf = TTF_RenderText_Solid(font, txtRang, vert);
        tex  = SDL_CreateTextureFromSurface(ren, surf);
        dest = (SDL_Rect){160, 420, surf->w, surf->h};
        SDL_RenderCopy(ren, tex, NULL, &dest);
        SDL_FreeSurface(surf);
        SDL_DestroyTexture(tex);
    }

    // Saisie nom
    if (sm->enSaisie) {
        char prompt[100];
        sprintf(prompt, "Entrez votre nom : %s_",
                sm->nomSaisi);
        surf = TTF_RenderText_Solid(font, prompt, rouge);
        tex  = SDL_CreateTextureFromSurface(ren, surf);
        dest = (SDL_Rect){160, 420, surf->w, surf->h};
        SDL_RenderCopy(ren, tex, NULL, &dest);
        SDL_FreeSurface(surf);
        SDL_DestroyTexture(tex);
    }

    // Bouton Retour
    if (sm->retourActif)
        SDL_SetRenderDrawColor(ren, 180, 140, 0, 255);
    else
        SDL_SetRenderDrawColor(ren, 60, 60, 60, 255);
    SDL_RenderFillRect(ren, &sm->boutonRetour);
    SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
    SDL_RenderDrawRect(ren, &sm->boutonRetour);

    surf = TTF_RenderText_Solid(font, "Retour", vert);
    tex  = SDL_CreateTextureFromSurface(ren, surf);
    dest = (SDL_Rect){
        sm->boutonRetour.x + (sm->boutonRetour.w - surf->w) / 2,
        sm->boutonRetour.y + (sm->boutonRetour.h - surf->h) / 2,
        surf->w, surf->h
    };
    SDL_RenderCopy(ren, tex, NULL, &dest);
    SDL_FreeSurface(surf);
    SDL_DestroyTexture(tex);
}

int inputSousMenuScores(SousMenuScores *sm,
                         SDL_Event *e,
                         int scoreJoueur) {
    if (!sm->visible) return 0;

    // Survol bouton Retour
    if (e->type == SDL_MOUSEMOTION) {
        int mx = e->motion.x;
        int my = e->motion.y;
        SDL_Rect *r = &sm->boutonRetour;
        sm->retourActif =
            (mx >= r->x && mx <= r->x + r->w &&
             my >= r->y && my <= r->y + r->h) ? 1 : 0;
    }

    // Clic bouton Retour
    if (e->type == SDL_MOUSEBUTTONDOWN &&
        e->button.button == SDL_BUTTON_LEFT) {
        int mx = e->button.x;
        int my = e->button.y;
        SDL_Rect *r = &sm->boutonRetour;
        if (mx >= r->x && mx <= r->x + r->w &&
            my >= r->y && my <= r->y + r->h) {
            sm->visible    = 0;
            sm->enSaisie   = 0;
            sm->rangJoueur = -1;
            memset(sm->nomSaisi, 0, sizeof(sm->nomSaisi));
            return 1;
        }
    }

    if (e->type == SDL_KEYDOWN) {
        if (e->key.keysym.sym == SDLK_ESCAPE) {
            sm->visible    = 0;
            sm->enSaisie   = 0;
            sm->rangJoueur = -1;
            memset(sm->nomSaisi, 0, sizeof(sm->nomSaisi));
            return 1;
        }

        if (sm->enSaisie) {
            SDL_Keycode k = e->key.keysym.sym;

            if (k == SDLK_BACKSPACE) {
                int len = strlen(sm->nomSaisi);
                if (len > 0) sm->nomSaisi[len - 1] = '\0';
            }
            else if (k == SDLK_RETURN &&
                     strlen(sm->nomSaisi) > 0) {
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
    return 0;
}
