#pragma once
#include<SDL.h>
#include<SDL_ttf.h>
#include<iostream>
using namespace std;
bool backrules = false;
void showrules(SDL_Renderer* renderer)
{
	TTF_Font* font = TTF_OpenFont("arial.ttf", 17);
	SDL_Color color = { 0,0,0,255 };
	SDL_SetRenderDrawColor(renderer, 0, 200, 200, 255);
	SDL_RenderClear(renderer);
	bool running = true;
	while (running)
	{

		SDL_Event e;
		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT) {
				backrules = true;
				running = false;
			}
			else if (e.type == SDL_KEYDOWN) {
				if (e.key.keysym.sym == SDLK_q) {
					running = false;
				}
			}

				SDL_Surface* textsurf = TTF_RenderText_Solid(font, "1.Snake will get bigger after eating each of the food and score will increase by 5.", color);
				SDL_Surface* textsurf1 = TTF_RenderText_Solid(font, "2. In the free level , if snake's head touches the body then game will be over.", color);
				SDL_Surface* textsurf2 = TTF_RenderText_Solid(font, "3. In the box level if the snake's head touches it's own body or if the snake's", color);
				SDL_Surface* textsurf3 = TTF_RenderText_Solid(font, "    head touches any of the 4 boundary borders, then game will be over.", color);
				SDL_Surface* textsurf4 = TTF_RenderText_Solid(font, " 4. Use UP, DOWN,LEFT,RIGHT keys to move the snake. Press P to pause the game.", color);
				SDL_Texture* texttexture = SDL_CreateTextureFromSurface(renderer, textsurf);
				SDL_Texture* texttexture1 = SDL_CreateTextureFromSurface(renderer, textsurf1);
				SDL_Texture* texttexture2 = SDL_CreateTextureFromSurface(renderer, textsurf2);
				SDL_Texture* texttexture3 = SDL_CreateTextureFromSurface(renderer, textsurf3);
				SDL_Texture* texttexture4 = SDL_CreateTextureFromSurface(renderer, textsurf4);
				SDL_FreeSurface(textsurf);
				SDL_FreeSurface(textsurf1);
				SDL_Rect textRect = { 20,100,765,40 };
				SDL_Rect textRect1 = { 20, 140, 765, 40 };
				SDL_Rect textRect2 = { 20, 180, 765, 40 };
				SDL_Rect textRect3 = { 20, 220, 765, 40 };
				SDL_Rect textRect4 = { 20,260,765,40 };
				SDL_RenderCopy(renderer, texttexture, NULL, &textRect);
				SDL_RenderCopy(renderer, texttexture1, NULL, &textRect1);
				SDL_RenderCopy(renderer, texttexture2, NULL, &textRect2);
				SDL_RenderCopy(renderer, texttexture3, NULL, &textRect3);
				SDL_RenderCopy(renderer, texttexture4, NULL, &textRect4);
				SDL_RenderPresent(renderer);
				SDL_Delay(25);
			}
		}
	}