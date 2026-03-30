#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include "scores.h"
#include "menu.h"

int main() {
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    TTF_Init();
    IMG_Init(IMG_INIT_PNG);
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);

    SDL_Window *win = SDL_CreateWindow(
        "Mon Jeu",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        800, 600, 0);

    SDL_Renderer *ren = SDL_CreateRenderer(win, -1,
        SDL_RENDERER_ACCELERATED);

    TTF_Font *font = TTF_OpenFont(
        "/usr/share/fonts/truetype/liberation/LiberationSans-Regular.ttf",
        22);

    // ── INIT ─────────────────────────────────────────────
    Menu menu;
    initMenu(&menu);

    SousMenuScores sm;
    initSousMenuScores(&sm);

    int scoreJoueur = 1500; // score simulé
    int continuer   = 1;
    SDL_Event event;

    // ── GAME LOOP ─────────────────────────────────────────
    while (continuer) {

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) continuer = 0;

            // Input menu principal
            if (menu.visible && !sm.visible) {
                int clique = inputMenu(&menu, &event);

                if (clique == 0) {
                    // Jouer
                    printf("Jouer cliqué\n");
                }
                else if (clique == 1) {
                    // Options
                    printf("Options cliqué\n");
                }
                else if (clique == 2) {
                    // Meilleurs Scores → ouvrir sous-menu
                    sm.visible  = 1;
                    sm.enSaisie = 1;
                    menu.visible = 0;
                }
                else if (clique == 3) {
                    // Quitter
                    continuer = 0;
                }
            }

            // Input sous-menu scores
            if (sm.visible) {
                inputSousMenuScores(&sm, &event, scoreJoueur);

                // Quand scores fermé → retour menu
                if (!sm.visible) {
                    menu.visible = 1;
                }
            }
        }

        // ── AFFICHAGE ─────────────────────────────────────
        SDL_SetRenderDrawColor(ren, 30, 30, 60, 255);
        SDL_RenderClear(ren);

        // Afficher menu OU sous-menu scores
        if (menu.visible) {
            afficherMenu(ren, &menu, font);
        }
        if (sm.visible) {
            afficherSousMenuScores(ren, &sm, font);
        }

        SDL_RenderPresent(ren);
        SDL_Delay(16);
    }

    // ── LIBERATION ────────────────────────────────────────
    TTF_CloseFont(font);
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    Mix_CloseAudio();
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
    return 0;
}
