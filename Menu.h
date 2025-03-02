#pragma once
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <iostream>
using namespace std;

// Global Variables
SDL_Surface* menus[5]; // Adjusted size
SDL_Texture* menuTextures[5]; 
bool selected[5] = { false }; // Ensure the selected array is initialized
SDL_Color color[2] = { {255, 255, 255}, {255, 0, 0} };
SDL_Rect pos[5]; 
SDL_Texture* text;
SDL_Texture* texture;
SDL_Surface* surf;
SDL_Surface* surface;
SDL_Rect textrect;

void initializeMenu(SDL_Renderer* renderer, TTF_Font* font, const char* labels[], int numMenuItems) {
    surf = IMG_Load("snake_2.jpg");
    text = SDL_CreateTextureFromSurface(renderer, surf);
    SDL_FreeSurface(surf);
    surface = TTF_RenderText_Solid(font, "SnakeOMania", { 180,0,0,255 });
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    textrect = { 150, 100, 500, 200 };
    SDL_FreeSurface(surface);

    for (int i = 0; i < numMenuItems; ++i) {
        menus[i] = TTF_RenderText_Solid(font, labels[i], color[0]);
        menuTextures[i] = SDL_CreateTextureFromSurface(renderer, menus[i]);
        SDL_FreeSurface(menus[i]);
    }
}

void calculateMenuPositions(int numMenuItems) {
    const int leftMargin = 150; 

    SDL_QueryTexture(menuTextures[0], NULL, NULL, &pos[0].w, &pos[0].h);
    pos[0].x = leftMargin;
    pos[0].y = 650 / 2 - pos[0].h - 20;

    for (int i = 1; i < numMenuItems; ++i) {
        SDL_QueryTexture(menuTextures[i], NULL, NULL, &pos[i].w, &pos[i].h);
        pos[i].x = leftMargin;
        pos[i].y = pos[i - 1].y + pos[i - 1].h + 20;
    }
}

void renderMenu(SDL_Renderer* renderer, int numMenuItems) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); //MENU BAR TEXT COLOUR 
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, text, NULL, NULL);
    SDL_RenderCopy(renderer, texture, NULL, &textrect);

    for (int i = 0; i < numMenuItems; ++i) {
        SDL_RenderCopy(renderer, menuTextures[i], NULL, &pos[i]);
    }

    SDL_RenderPresent(renderer);
}

void cleanupMenu(int numMenuItems) {
    for (int i = 0; i < numMenuItems; ++i) {
        SDL_DestroyTexture(menuTextures[i]);
    }
    SDL_DestroyTexture(text);
    SDL_DestroyTexture(texture);
}
Mix_Music* loadMusic(const char* filename) {
    Mix_Music* music = Mix_LoadMUS("backgroundmusic.mp3"); //FILE THEKE MUSIC UPLOAD  
    if (!music) {
        std::cerr << "Mix_LoadMUS Error: " << Mix_GetError() << std::endl;
        return nullptr;
    }
    return music;
}

    void playMusic(Mix_Music * music) {
        if (Mix_PlayingMusic() == 0) {
            Mix_PlayMusic(music, -1);
        }
    }
void stopMusic() {
    Mix_HaltMusic();
}

int mainmenu(SDL_Renderer* renderer, TTF_Font* font) {
    const int NUMMENU = 4;
    const char* labels[NUMMENU] = { "PLAY", "HIGHEST SCORE", "RULES", "EXIT" };//MENU OPTION 
    initializeMenu(renderer, font, labels, NUMMENU);
    calculateMenuPositions(NUMMENU);
    Mix_Music* music = loadMusic("backgroundmusic.mp3"); //GAME BACKROUND MUSIC UPLOAD
    if (music) {
        playMusic(music);
    }
    SDL_Event event;
    bool running = true;
    int selectedMenuItem = -1;
    while (running) {
        
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
            case SDL_QUIT:
                running = false;
                break;
            case SDL_MOUSEMOTION: {
                int x = event.motion.x;
                int y = event.motion.y;
                for (int i = 0; i < NUMMENU; ++i) {
                    bool inside = x >= pos[i].x && x <= pos[i].x + pos[i].w && y >= pos[i].y && y <= pos[i].y + pos[i].h;
                    if (inside && !selected[i]) {
                        selected[i] = true;
                        SDL_DestroyTexture(menuTextures[i]);
                        menus[i] = TTF_RenderText_Solid(font, labels[i], color[1]);
                        menuTextures[i] = SDL_CreateTextureFromSurface(renderer, menus[i]);
                        SDL_FreeSurface(menus[i]);
                    }
                    else if (!inside && selected[i]) {
                        selected[i] = false;
                        SDL_DestroyTexture(menuTextures[i]);
                        menus[i] = TTF_RenderText_Solid(font, labels[i], color[0]);
                        menuTextures[i] = SDL_CreateTextureFromSurface(renderer, menus[i]);
                        SDL_FreeSurface(menus[i]);
                    }
                }
                break;
            }
            case SDL_MOUSEBUTTONDOWN: {
                int x = event.button.x;
                int y = event.button.y;
                for (int i = 0; i < NUMMENU; ++i) {
                    if (x >= pos[i].x && x <= pos[i].x + pos[i].w && y >= pos[i].y && y <= pos[i].y + pos[i].h) {
                        selectedMenuItem = i;
                        running = false;
                        
                    }
                }
                break;
            }
            case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_ESCAPE)
                    running = false;
                break;
            }
        }

        renderMenu(renderer, NUMMENU);
    }

    cleanupMenu(NUMMENU);
    stopMusic();
    return selectedMenuItem;
}

int playmenu(SDL_Renderer* renderer, TTF_Font* font) {
    const int NUMMENU = 5;
    const char* labels[NUMMENU] = { "Beginner", "Easy", "Medium", "Hard","Back" }; 
    initializeMenu(renderer, font, labels, NUMMENU);
    calculateMenuPositions(NUMMENU);
    Mix_Music* music = loadMusic("backgroundmusic.mp3"); 
    if (music) {
        playMusic(music);
    }
    SDL_Event event;
    bool running = true;
    bool time = true;
    int selectedMenuItem = -1;
    while (running) {
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
            case SDL_QUIT:
                running = false;
                break;
            case SDL_MOUSEMOTION: {
                int x = event.motion.x;
                int y = event.motion.y;
                for (int i = 0; i < NUMMENU; ++i) {
                    bool inside = x >= pos[i].x && x <= pos[i].x + pos[i].w && y >= pos[i].y && y <= pos[i].y + pos[i].h;
                    if (inside && !selected[i]) {
                        selected[i] = true;
                        SDL_DestroyTexture(menuTextures[i]);
                        menus[i] = TTF_RenderText_Solid(font, labels[i], color[1]);
                        menuTextures[i] = SDL_CreateTextureFromSurface(renderer, menus[i]);
                        SDL_FreeSurface(menus[i]);
                    }
                    else if (!inside && selected[i]) {
                        selected[i] = false;
                        SDL_DestroyTexture(menuTextures[i]);
                        menus[i] = TTF_RenderText_Solid(font, labels[i], color[0]);
                        menuTextures[i] = SDL_CreateTextureFromSurface(renderer, menus[i]);
                        SDL_FreeSurface(menus[i]);
                    }
                }
                break;
            }
            case SDL_MOUSEBUTTONDOWN: {
                int x = event.button.x;
                int y = event.button.y;
                for (int i = 0; i < NUMMENU; ++i) {
                    if (x >= pos[i].x && x <= pos[i].x + pos[i].w && y >= pos[i].y && y <= pos[i].y + pos[i].h) {
                        selectedMenuItem = i;
                        running = false;
                    }
                }
                break;
            }
            case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_ESCAPE)
                    running = false;
                break;
            }
        }

        renderMenu(renderer, NUMMENU);
    }

    cleanupMenu(NUMMENU);
    stopMusic();
    return selectedMenuItem;
}


