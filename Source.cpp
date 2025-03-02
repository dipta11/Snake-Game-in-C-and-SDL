#include"Game.h"
#include"Menu.h"
#include"Rules.h"
#include <cstdio>
bool back = false;
void Showscore(SDL_Renderer* renderer, TTF_Font* font, SDL_Color color) {
	int highest = 0;
	FILE* fp2 = nullptr;
	errno_t err;
	bool showhigh = true;
	// pC ER HIGHSCORE FILE OPEN 
	err = fopen_s(&fp2, "highscore.txt", "r");
	if (err == 0 && fp2 != NULL) {
		// Read highest score  
		fscanf_s(fp2, "%d", &highest);
		fclose(fp2);
	}
	else {
		
		printf("Error opening file for reading\n");
		return;
	}

	
	//printf("Highest Score: %d\n", highest);
	while (showhigh)
	{
		// Render the highest score
		
		SDL_Event e;
		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT) {
				back = true;
				showhigh = false;
			}
		}
		
		std::string highscoreText = " Highest Score: " + std::to_string(highest);
		SDL_Texture* highscoreTexture = renderText(renderer, font, highscoreText, color);
		if (!highscoreTexture) {
			printf("Failed to create highscore texture\n");
			return;
		}

		SDL_Rect highscoreRect = { 300, 200, 200, 50 }; // Adjust size and position as needed
		SDL_SetRenderDrawColor(renderer, 0, 200, 200, 255);
		// Clear the screen before rendering new content
		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, highscoreTexture, NULL, &highscoreRect);
		SDL_RenderPresent(renderer);
		SDL_DestroyTexture(highscoreTexture);
		SDL_Delay(25);


	}
}
void menuhandling(SDL_Renderer* renderer, TTF_Font* font, SDL_Color color)
{
	int selectedMenuItem = mainmenu(renderer, font);
	int selectedlevel;
	int selectedgame;

	switch (selectedMenuItem)
	{
	case 0:
		
		selectedgame = playmenu(renderer, font);
		switch (selectedgame)
		{
		case 0:
			game1(renderer, font, color);
			if (game1over)
			{
				menuhandling(renderer, font, color);
			}
			break;
		case 1:
			game2(renderer, font, color);
			if (game2over)
			{
				menuhandling(renderer, font, color);
			}
			break;
		case 2:
			game3(renderer, font, color);
			if (game3over)
			{
				menuhandling(renderer, font, color);
			}
			break;
		case 3:
			game4(renderer, font, color);
			if (game4over)
			{
				menuhandling(renderer, font, color);
			}
			break;
		case 4:
			menuhandling(renderer, font, color);
			break;

		default:
			
			break;


		}
		break;
	case 1:

		Showscore(renderer,font,color);
		if (back)
		{
			menuhandling(renderer,font,color);
		}
			break;
	case 2:
		showrules(renderer);
		if (backrules)
		{
			menuhandling(renderer, font, color);
		}
		break;
	case 3:
		cout << "Quit" << endl;
		break;
	default:

		break;
	}

}
//sdl everything initialize . start main code 
	int main(int argc, char* argv[]) {
		if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
			std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
			return 1;
		}
		if (TTF_Init() == -1) {
			std::cerr << "TTF_Init Error: " << TTF_GetError() << std::endl;
			SDL_Quit();
			return 1;
		}
		TTF_Font* font = TTF_OpenFont("arial.ttf", 30);
		if (!font) {
			std::cerr << "TTF_OpenFont Error: " << TTF_GetError() << std::endl;
			TTF_Quit();
			SDL_Quit();
			return 1;
		}
		SDL_Color color = { 0, 0, 0, 255 };
		SDL_Window* window = SDL_CreateWindow("Snake", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 550, 0);
		if (!window) {
			std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
			TTF_CloseFont(font);
			TTF_Quit();
			SDL_Quit();
			return 1;
		}
		SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);
		if (!renderer) {
			std::cerr << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
			SDL_DestroyWindow(window);
			TTF_CloseFont(font);
			TTF_Quit();
			SDL_Quit();
			return 1;
		}
		if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) == -1) {
			std::cerr << "Mix_OpenAudio Error: " << Mix_GetError() << std::endl;
			SDL_DestroyRenderer(renderer);
			SDL_DestroyWindow(window);
			TTF_CloseFont(font);
			TTF_Quit();
			SDL_Quit();
			return 1;
		}
		menuhandling(renderer, font, color);
		TTF_CloseFont(font);
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		TTF_Quit();
		SDL_Quit();

		return 0;
	}


