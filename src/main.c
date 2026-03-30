#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include "scores.h"

int main() {
    // ── INIT SDL ──────────────────────────────────────────
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
        printf("Erreur SDL_Init: %s\n", SDL_GetError());
        return 1;
    }
    TTF_Init();
    IMG_Init(IMG_INIT_PNG);
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);

    SDL_Window *win = SDL_CreateWindow(
        "Mon Jeu - Meilleurs Scores",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        800, 600, 0);

    SDL_Renderer *ren = SDL_CreateRenderer(win, -1,
        SDL_RENDERER_ACCELERATED);

    TTF_Font *font = TTF_OpenFont(
        "/usr/share/fonts/truetype/liberation/LiberationSans-Regular.ttf",
        22);

    if (!font) {
        printf("Erreur font: %s\n", TTF_GetError());
        return 1;
    }

    // ── INIT SOUS-MENU SCORES ─────────────────────────────
    SousMenuScores sm;
    initSousMenuScores(&sm);

    // Simuler un score du joueur
    int scoreJoueur = 1500;

    // Ouvrir directement le sous-menu avec saisie du nom
    sm.visible  = 1;
    sm.enSaisie = 1;

    // ── GAME LOOP ─────────────────────────────────────────
    int continuer = 1;
    SDL_Event event;

    while (continuer) {

        // LECTURE EVENEMENTS
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                continuer = 0;
            }
            // Passer les events au sous-menu scores
            inputSousMenuScores(&sm, &event, scoreJoueur);

            // Si le sous-menu est fermé → quitter
            if (!sm.visible) {
                continuer = 0;
            }
        }

        // AFFICHAGE
        SDL_SetRenderDrawColor(ren, 40, 40, 40, 255);
        SDL_RenderClear(ren);

        // Afficher le sous-menu scores
        afficherSousMenuScores(ren, &sm, font);

        SDL_RenderPresent(ren);
        SDL_Delay(16);
    }

    // ── LIBERATION MEMOIRE ────────────────────────────────
    TTF_CloseFont(font);
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    Mix_CloseAudio();
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();

    return 0;
}
