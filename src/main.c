#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include "scores.h"
#include "menu.h"
#include "jeu.h"

// Etats du jeu
#define ETAT_MENU   0
#define ETAT_JEU    1
#define ETAT_SCORES 2

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
    Menu           menu;
    SousMenuScores sm;
    Jeu            jeu;

    initMenu(&menu);
    initSousMenuScores(&sm);
    initJeu(&jeu);

    int etat      = ETAT_MENU;
    int continuer = 1;
    SDL_Event event;

    // ── GAME LOOP ─────────────────────────────────────────
    while (continuer) {

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) continuer = 0;

            // ── ETAT MENU ─────────────────────────────────
            if (etat == ETAT_MENU) {
                int clique = inputMenu(&menu, &event);

                if (clique == 0) {
                    // Jouer → lancer le jeu
                    initJeu(&jeu);
                    etat = ETAT_JEU;
                }
                else if (clique == 2) {
                    // Meilleurs Scores → ouvrir sous-menu
                    sm.visible  = 1;
                    sm.enSaisie = 0; // pas de saisie ici
                    etat = ETAT_SCORES;
                }
                else if (clique == 3) {
                    continuer = 0;
                }
            }

            // ── ETAT JEU ──────────────────────────────────
            else if (etat == ETAT_JEU) {
                inputJeu(&jeu, &event);

                // Partie terminée → aller aux scores
                if (jeu.termine) {
                    sm.visible  = 1;
                    sm.enSaisie = 1; // saisie du nom
                    etat = ETAT_SCORES;
                }
            }

            // ── ETAT SCORES ───────────────────────────────
            else if (etat == ETAT_SCORES) {
                // Passer le VRAI score du jeu
                int retour = inputSousMenuScores(&sm, &event,
                                                 jeu.score);
                if (retour == 1 || !sm.visible) {
                    sm.visible = 0;
                    etat = ETAT_MENU;
                    menu.visible = 1;
                }
            }
        }

        // ── UPDATE ────────────────────────────────────────
        if (etat == ETAT_JEU) {
            updateJeu(&jeu);
            // Partie terminée après update
            if (jeu.termine) {
                sm.visible  = 1;
                sm.enSaisie = 1;
                etat = ETAT_SCORES;
            }
        }

        // ── AFFICHAGE ─────────────────────────────────────
        SDL_SetRenderDrawColor(ren, 30, 30, 60, 255);
        SDL_RenderClear(ren);

        if (etat == ETAT_MENU) {
            afficherMenu(ren, &menu, font);
        }
        else if (etat == ETAT_JEU) {
            afficherJeu(ren, &jeu, font);
        }
        else if (etat == ETAT_SCORES) {
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
