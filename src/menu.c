#include "menu.h"
#include <string.h>
#include <stdio.h>

void initMenu(Menu *m) {
    m->visible    = 1;
    m->nb_boutons = 4;

    // Bouton Jouer
    m->boutons[0].zone  = (SDL_Rect){300, 150, 200, 50};
    strcpy(m->boutons[0].label, "Jouer");
    m->boutons[0].actif = 0;

    // Bouton Options
    m->boutons[1].zone  = (SDL_Rect){300, 230, 200, 50};
    strcpy(m->boutons[1].label, "Options");
    m->boutons[1].actif = 0;

    // Bouton Meilleurs Scores
    m->boutons[2].zone  = (SDL_Rect){300, 310, 200, 50};
    strcpy(m->boutons[2].label, "Meilleurs Scores");
    m->boutons[2].actif = 0;

    // Bouton Quitter
    m->boutons[3].zone  = (SDL_Rect){300, 390, 200, 50};
    strcpy(m->boutons[3].label, "Quitter");
    m->boutons[3].actif = 0;
}

void afficherMenu(SDL_Renderer *ren, Menu *m, TTF_Font *font) {
    if (!m->visible) return;

    SDL_Color blanc = {255, 255, 255, 255};
    SDL_Color jaune = {255, 215,   0, 255};

    // Titre MENU
    SDL_Surface *surf = TTF_RenderText_Solid(font, "MENU", jaune);
    SDL_Texture *tex  = SDL_CreateTextureFromSurface(ren, surf);
    SDL_Rect dest     = {350, 70, surf->w, surf->h};
    SDL_RenderCopy(ren, tex, NULL, &dest);
    SDL_FreeSurface(surf);
    SDL_DestroyTexture(tex);

    // Dessiner chaque bouton
    for (int i = 0; i < m->nb_boutons; i++) {
        // Fond bouton
        if (m->boutons[i].actif) {
            // Bouton survolé = couleur dorée
            SDL_SetRenderDrawColor(ren, 180, 140, 0, 255);
        } else {
            // Bouton normal = gris foncé
            SDL_SetRenderDrawColor(ren, 60, 60, 60, 255);
        }
        SDL_RenderFillRect(ren, &m->boutons[i].zone);

        // Bordure blanche
        SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
        SDL_RenderDrawRect(ren, &m->boutons[i].zone);

        // Texte du bouton
        surf = TTF_RenderText_Solid(font, m->boutons[i].label, blanc);
        tex  = SDL_CreateTextureFromSurface(ren, surf);
        dest = (SDL_Rect){
            m->boutons[i].zone.x + (m->boutons[i].zone.w - surf->w) / 2,
            m->boutons[i].zone.y + (m->boutons[i].zone.h - surf->h) / 2,
            surf->w, surf->h
        };
        SDL_RenderCopy(ren, tex, NULL, &dest);
        SDL_FreeSurface(surf);
        SDL_DestroyTexture(tex);
    }
}

// Retourne l'index du bouton cliqué ou -1
int inputMenu(Menu *m, SDL_Event *e) {
    if (!m->visible) return -1;

    // Survol souris → activer bouton
    if (e->type == SDL_MOUSEMOTION) {
        int mx = e->motion.x;
        int my = e->motion.y;
        for (int i = 0; i < m->nb_boutons; i++) {
            SDL_Rect *z = &m->boutons[i].zone;
            m->boutons[i].actif =
                (mx >= z->x && mx <= z->x + z->w &&
                 my >= z->y && my <= z->y + z->h) ? 1 : 0;
        }
    }

    // Clic souris → détecter quel bouton
    if (e->type == SDL_MOUSEBUTTONDOWN &&
        e->button.button == SDL_BUTTON_LEFT) {
        int mx = e->button.x;
        int my = e->button.y;
        for (int i = 0; i < m->nb_boutons; i++) {
            SDL_Rect *z = &m->boutons[i].zone;
            if (mx >= z->x && mx <= z->x + z->w &&
                my >= z->y && my <= z->y + z->h) {
                return i; // 0=Jouer 1=Options 2=Scores 3=Quitter
            }
        }
    }
    return -1;
}
