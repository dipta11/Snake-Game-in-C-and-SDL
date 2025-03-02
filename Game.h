#pragma once
#pragma once

#include<SDL.h>
#include<SDL_ttf.h>
#include<SDL_image.h>
#include<SDL_mixer.h>
#include<iostream>
#include<vector>
#include<algorithm>
#include<deque>
#include<random>
#include<string>
#include<cstdio>
using namespace std;
//int size = 1;
int score = 0;
const int windowWidth = 800;
const int windowHeight = 550;
const int snakeStep = 8;
bool game1over = false;
bool game2over = false;
bool game3over = false;
bool game4over = false;
// game over sound code 
void playgameoversound()
{
	Mix_Music* music = Mix_LoadMUS("gameOversound.mp3"); // game over sound upload file 
	if (!music) {
		std::cerr << "Mix_LoadMUS Error: " << Mix_GetError() << std::endl;
		Mix_CloseAudio();
		SDL_Quit();
	}
	Mix_PlayMusic(music, 1);

}
// food khabar sound 
void playeatsound()
{
	Mix_Music* music = Mix_LoadMUS("eatSound.mp3");
	if (!music) {
		std::cerr << "Mix_LoadMUS Error: " << Mix_GetError() << std::endl;
		Mix_CloseAudio();
		SDL_Quit();
	}
	Mix_PlayMusic(music, 1);

}

	// snake er direction 
enum  Directions {
	DOWN,
	RIGHT,
	LEFT,
	UP
};

SDL_Texture* renderText(SDL_Renderer* renderer, TTF_Font* font, const std::string& message, SDL_Color color) {
	SDL_Surface* surf = TTF_RenderText_Solid(font, message.c_str(), color);
	if (!surf) {
		printf("TTF_RenderText_Solid Error: %s\n", TTF_GetError());
		return nullptr;
	}
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surf);
	SDL_FreeSurface(surf);
	if (!texture) {
		printf("SDL_CreateTextureFromSurface Error: %s\n", SDL_GetError());
	}
	return texture;
}
// highscore storing 
void storeHighScore(int score) {
	int highscore = 0;
	FILE* fp = nullptr;
	errno_t err;

	// high score file open 
	err = fopen_s(&fp, "highscore.txt", "r");
	if (err == 0 && fp != NULL) {
		// read the high score file 
		fscanf_s(fp, "%d", &highscore);
		fclose(fp);
	}
	

	// score besi hole highscore hisabe write krbo 
	if (score > highscore) {
		highscore = score;
		
		err = fopen_s(&fp, "highscore.txt", "w");
		if (err == 0 && fp != NULL) {
		
			fprintf(fp, "%d", highscore);
			fclose(fp);
		}
		else {
			
			printf("Error opening file for writing\n");
		}
	}

}
// beeginer level er code /////////////////////////////////////////////////////////////////////////
void game1(SDL_Renderer* renderer, TTF_Font* font, SDL_Color color) {
	bool running = true;
	bool paused = false;
	int dir = RIGHT;
	SDL_Event e;
	int score = 0;
	const int snakeStep = 10;
	const int windowWidth = 800;
	const int windowHeight = 550;
	const int frameDelay = 100; 

	// saper mathar 1st postion 
	SDL_Rect head;
	head.x = 400;
	head.y = 275;
	head.w = snakeStep; 
	head.h = snakeStep;

	//saper mathar picture load as texture 
	SDL_Texture* snakeHeadTexture = IMG_LoadTexture(renderer, "headc.png"); // Load snake head image
	if (!snakeHeadTexture) {
		
		SDL_Log("Failed to load snake head texture: %s", SDL_GetError());
		return;
	}

	// for Snake body 
	std::deque<SDL_Rect> rq;
	int size = 1;

	// for food 
	SDL_Rect apple;
	apple.w = 8; // food er size 
	apple.h = 8;
	apple.x = 30 + (rand() % ((740 - 30) / 10)) * 10;
	apple.y = 100 + (rand() % ((520 - 100) / 10)) * 10;

	while (running) {
		Uint32 frameStart = SDL_GetTicks();

		// input on playing time 
		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT) {
				game1over = true;
				running = false;
			}
			// snake direction deoya 
			if (e.type == SDL_KEYDOWN) {
				if (e.key.keysym.sym == SDLK_DOWN && dir != UP) dir = DOWN;
				else if (e.key.keysym.sym == SDLK_UP && dir != DOWN) dir = UP;
				else if (e.key.keysym.sym == SDLK_RIGHT && dir != LEFT) dir = RIGHT;
				else if (e.key.keysym.sym == SDLK_LEFT && dir != RIGHT) dir = LEFT;
				else if (e.key.keysym.sym == SDLK_p) paused = !paused;
			}
		}
	
		if (!paused) {
			
			switch (dir) {
			case DOWN:
				head.y += snakeStep;
				break;
			case UP:
				head.y -= snakeStep;
				break;
			case LEFT:
				head.x -= snakeStep;
				break;
			case RIGHT:
				head.x += snakeStep;
				break;
			}

			// eating food 
			if (SDL_HasIntersection(&head, &apple)) {
				
				playeatsound();
				size += 2;
				score += 5;
				apple.x = 30 + (rand() % ((740 - 30) / 10)) * 10;
				apple.y = 100 + (rand() % ((520 - 100) / 10)) * 10;
			}

			// snake out hole new in snake 
			if (head.x < 0) {
				head.x = windowWidth - snakeStep;
			}
			else if (head.x >= windowWidth) {
				head.x = 0;
			}
			if (head.y < 60) {
				head.y = windowHeight - snakeStep;
			}
			else if (head.y >= windowHeight) {
				head.y = 60;
			}

			// snake bite on his own body / game over 
			for (auto& snake_segment : rq) {
				if (head.x == snake_segment.x && head.y == snake_segment.y) {
					std::cout << score << std::endl;
					storeHighScore(score);
					playgameoversound();
					SDL_Surface* textsurf = TTF_RenderText_Solid(font, "GAME OVER", color); // showing gameover 
					SDL_Texture* texttexture = SDL_CreateTextureFromSurface(renderer, textsurf);
					SDL_FreeSurface(textsurf);
					SDL_Rect textRect = { 200, 150, 400, 100 }; 
					SDL_RenderCopy(renderer, texttexture, NULL, &textRect);
					SDL_RenderPresent(renderer);
					SDL_DestroyTexture(texttexture);
					SDL_Delay(1000);
					game1over = true;
					running = false;
				}
			}

			// head movement 
			rq.push_front(head);

			// previos head remove 
			while (rq.size() > size)
				rq.pop_back();
		}

		// playing game window 
		SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
		SDL_RenderClear(renderer);

		// snake body 
		SDL_SetRenderDrawColor(renderer, 0, 220, 0, 255);
		for (auto& snake_segment : rq) {
			if (snake_segment.x == head.x && snake_segment.y == head.y) {
				continue;
			}
			SDL_RenderFillRect(renderer, &snake_segment);
		}

		// food colour 
		SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
		SDL_RenderFillRect(renderer, &apple);

		//  snake head (image) movement
		double angle = 0;
		switch (dir) {
		case UP:
			angle = 270;
			break;
		case DOWN:
			angle = 90;
			break;
		case LEFT:
			angle = 180;
			break;
		case RIGHT:
			angle = 0;
			break;
		}
		SDL_Point center = { head.w / 2, head.h / 2 };
		SDL_RenderCopyEx(renderer, snakeHeadTexture, NULL, &head, angle, &center, SDL_FLIP_NONE);

		// uperer score when playing 
		std::string scoreText = "Score: " + std::to_string(score);
		SDL_Texture* scoreTexture = renderText(renderer, font, scoreText, color);
		SDL_Rect scoreRect = { 10, 10, 100, 50 };
		SDL_RenderCopy(renderer, scoreTexture, NULL, &scoreRect);
		SDL_DestroyTexture(scoreTexture);
		SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
		SDL_RenderDrawLine(renderer, 0, 60, windowWidth, 60);
		// paused option ana when playing 
		if (paused) {
			
			SDL_Surface* textsurf = TTF_RenderText_Solid(font, "PAUSED", color);
			SDL_Texture* texttexture = SDL_CreateTextureFromSurface(renderer, textsurf);
			SDL_FreeSurface(textsurf);
			SDL_Rect textRect = { 300, 250, 200, 100 };
			SDL_RenderCopy(renderer, texttexture, NULL, &textRect);
			SDL_DestroyTexture(texttexture);
		}
		SDL_RenderPresent(renderer);
		SDL_Delay(25);

		//  frame rate
		Uint32 frameTime = SDL_GetTicks() - frameStart;
		if (frameDelay > frameTime) {
			SDL_Delay(frameDelay - frameTime);
		}
	}

	
	SDL_DestroyTexture(snakeHeadTexture);
}


// easy  game start //////////////////////////////////////////////////////////////////////////////////////
void game2(SDL_Renderer* renderer, TTF_Font* font, SDL_Color color) {
	bool running = true;
	bool paused = false;
	int dir = RIGHT;
	SDL_Event e;
	int score = 0;
	const int snakeStep = 20; // snake speed changed 
	const int windowWidth = 800;
	const int windowHeight = 550;
	const int frameDelay = 100;

	// Snake head position 
	SDL_Rect head;
	head.x = 400;
	head.y = 275;
	head.w = snakeStep; 
	head.h = snakeStep; 

	//head pic 
	SDL_Texture* snakeHeadTexture = IMG_LoadTexture(renderer, "headc.png"); 
	if (!snakeHeadTexture) {
		
		SDL_Log("Failed to load snake head texture: %s", SDL_GetError());
		return;
	}

	// Snake body 
	std::deque<SDL_Rect> rq;
	int size = 1;

	// for food
	SDL_Rect apple;
	apple.w = 12;
	apple.h = 12;
	apple.x = 30 + (rand() % ((740 - 30) / 10)) * 10;
	apple.y = 100 + (rand() % ((520 - 100) / 10)) * 10;

	while (running) {
		Uint32 frameStart = SDL_GetTicks();

		// kewy press 
		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT) {
				game2over = true;
				running = false;
			}
			if (e.type == SDL_KEYDOWN) {
				if (e.key.keysym.sym == SDLK_DOWN && dir != UP) dir = DOWN;
				else if (e.key.keysym.sym == SDLK_UP && dir != DOWN) dir = UP;
				else if (e.key.keysym.sym == SDLK_RIGHT && dir != LEFT) dir = RIGHT;
				else if (e.key.keysym.sym == SDLK_LEFT && dir != RIGHT) dir = LEFT;
				else if (e.key.keysym.sym == SDLK_p) paused = !paused;
			}
		}

		if (!paused) {
			// movment kora snake er 
			switch (dir) {
			case DOWN:
				head.y += snakeStep;
				break;
			case UP:
				head.y -= snakeStep;
				break;
			case LEFT:
				head.x -= snakeStep;
				break;
			case RIGHT:
				head.x += snakeStep;
				break;
			}

			// food khaoya 
			if (SDL_HasIntersection(&head, &apple)) {
				
				playeatsound();
				size += 2;
				score += 5;
				apple.x = 30 + (rand() % ((740 - 30) / 10)) * 10;
				apple.y = 100 + (rand() % ((520 - 100) / 10)) * 10;
			}

			// out hoile abr back 
			if (head.x < 0) {
				head.x = windowWidth - snakeStep;
			}
			else if (head.x >= windowWidth) {
				head.x = 0;
			}
			if (head.y < 60) {
				head.y = windowHeight - snakeStep;
			}
			else if (head.y >= windowHeight) {
				head.y = 60;
			}

			// game over . body khaoya 
			for (auto& snake_segment : rq) {
				if (head.x == snake_segment.x && head.y == snake_segment.y) {
					std::cout << score << std::endl;
					storeHighScore(score);
					playgameoversound();
					SDL_Surface* textsurf = TTF_RenderText_Solid(font, "GAME OVER", color);
					SDL_Texture* texttexture = SDL_CreateTextureFromSurface(renderer, textsurf);
					SDL_FreeSurface(textsurf);
					SDL_Rect textRect = { 200, 150, 400, 100 }; 
					SDL_RenderCopy(renderer, texttexture, NULL, &textRect);
					SDL_RenderPresent(renderer);
					SDL_DestroyTexture(texttexture);
					SDL_Delay(2000);
					game2over = true;
					running = false;
				}
			}

			// head movement 
			rq.push_front(head);

			
			while (rq.size() > size)
				rq.pop_back();
		}

		// window banano 
		SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
		SDL_RenderClear(renderer);

		//  snake body
		SDL_SetRenderDrawColor(renderer, 0, 220, 0, 255);
		for (auto& snake_segment : rq) {
			if (snake_segment.x == head.x && snake_segment.y == head.y) {
				continue;
			}
			SDL_RenderFillRect(renderer, &snake_segment);
		}

		// for food 
		SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
		SDL_RenderFillRect(renderer, &apple);

		//for  snake head (image)
		double angle = 0;
		switch (dir) {
		case UP:
			angle = 270;
			break;
		case DOWN:
			angle = 90;
			break;
		case LEFT:
			angle = 180;
			break;
		case RIGHT:
			angle = 0;
			break;
		}
		SDL_Point center = { head.w / 2, head.h / 2 };
		SDL_RenderCopyEx(renderer, snakeHeadTexture, NULL, &head, angle, &center, SDL_FLIP_NONE);

		//game  score show 
		std::string scoreText = "Score: " + std::to_string(score);
		SDL_Texture* scoreTexture = renderText(renderer, font, scoreText, color);
		SDL_Rect scoreRect = { 10, 10, 100, 50 };
		SDL_RenderCopy(renderer, scoreTexture, NULL, &scoreRect);
		SDL_DestroyTexture(scoreTexture);
		SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
		SDL_RenderDrawLine(renderer, 0, 60, windowWidth, 60);
		if (paused) {
			// pause when playing a
			SDL_Surface* textsurf = TTF_RenderText_Solid(font, "PAUSED", color);
			SDL_Texture* texttexture = SDL_CreateTextureFromSurface(renderer, textsurf);
			SDL_FreeSurface(textsurf);
			SDL_Rect textRect = { 300, 250, 200, 100 }; 
			SDL_RenderCopy(renderer, texttexture, NULL, &textRect);
			SDL_DestroyTexture(texttexture);
		}
		SDL_RenderPresent(renderer);
		SDL_Delay(25);

		
		Uint32 frameTime = SDL_GetTicks() - frameStart;
		if (frameDelay > frameTime) {
			SDL_Delay(frameDelay - frameTime);
		}
	}


	SDL_DestroyTexture(snakeHeadTexture);
}

// medium level er code ////////////////////////////////////////////////////////////////
void game3(SDL_Renderer* renderer, TTF_Font* font, SDL_Color color) {
	bool running = true;
	bool paused = false;
	int dir = RIGHT;
	SDL_Event e;
	int score = 0;
	const int snakeStep = 12;
	const int windowWidth = 800;
	const int windowHeight = 550;
	const int frameDelay = 100; // 
	const int borderSize = 12;

	// Snake head er bepar  
	SDL_Rect head;
	head.x = 396;
	head.y = 264;
	head.w = snakeStep; 
	head.h = snakeStep; 

	SDL_Texture* snakeHeadTexture = IMG_LoadTexture(renderer, "headc.png"); //  snake head image
	if (!snakeHeadTexture) {
		
		SDL_Log("Failed to load snake head texture: %s", SDL_GetError());
		return;
	}

	// Snake body 
	std::deque<SDL_Rect> rq;
	int size = 1;

	// food
	SDL_Rect apple;
	apple.w = 8;
	apple.h = 8;
	apple.x = 30 + (rand() % ((740 - 30) / 10)) * 10;
	apple.y = 100 + (rand() % ((520 - 100) / 10)) * 10;

	while (running) {
		Uint32 frameStart = SDL_GetTicks();

		//  inputs key press 
		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT) {
				game3over = true;
				running = false;
			}
			if (e.type == SDL_KEYDOWN) {
				if (e.key.keysym.sym == SDLK_DOWN && dir != UP) dir = DOWN;
				else if (e.key.keysym.sym == SDLK_UP && dir != DOWN) dir = UP;
				else if (e.key.keysym.sym == SDLK_RIGHT && dir != LEFT) dir = RIGHT;
				else if (e.key.keysym.sym == SDLK_LEFT && dir != RIGHT) dir = LEFT;
				else if (e.key.keysym.sym == SDLK_p) paused = !paused;
			}
		}

		if (!paused) {
			// Update snake going 
			switch (dir) {
			case DOWN:
				head.y += snakeStep;
				break;
			case UP:
				head.y -= snakeStep;
				break;
			case LEFT:
				head.x -= snakeStep;
				break;
			case RIGHT:
				head.x += snakeStep;
				break;
			}

			// eating food 
			if (SDL_HasIntersection(&head, &apple)) {
				
				playeatsound();
				size += 2;
				score += 5;
				apple.x = 30 + (rand() % ((740 - 30) / 10)) * 10;
				apple.y = 100 + (rand() % ((520 - 100) / 10)) * 10;
			}

			

			// box level er ta 
			for (const auto& snake_segment : rq) {
				if ((head.x == snake_segment.x && head.y == snake_segment.y) ||
					head.x < borderSize || head.y < borderSize ||
					head.x >= windowWidth - borderSize || head.y >= windowHeight - borderSize || head.y < 70) {
					playgameoversound();
					cout << score << endl;
					storeHighScore(score);
					SDL_Surface* textsurf = TTF_RenderText_Solid(font, "GAME OVER", color);
					SDL_Texture* texttexture = SDL_CreateTextureFromSurface(renderer, textsurf);
					SDL_FreeSurface(textsurf);
					SDL_Rect textRect = { 200, 150, 400, 100 };
					SDL_RenderCopy(renderer, texttexture, NULL, &textRect);
					SDL_RenderPresent(renderer);
					SDL_Delay(2000);
					SDL_DestroyTexture(texttexture);
					game3over = true;
					running = false;

					break;
				}
			}
			//  head going 
			rq.push_front(head);

			
			while (rq.size() > size)
				rq.pop_back();
		}

		// window create 
		SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
		SDL_RenderClear(renderer);

		//  snake body
		SDL_SetRenderDrawColor(renderer, 0, 220, 0, 255);
		for (auto& snake_segment : rq) {
			if (snake_segment.x == head.x && snake_segment.y == head.y) {
				continue;
			}
			SDL_RenderFillRect(renderer, &snake_segment);
		}

		//  border ta
		SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

		SDL_Rect topBorder = { 0, 60, windowWidth, borderSize };
		SDL_Rect bottomBorder = { 0, windowHeight - borderSize, windowWidth, borderSize };
		SDL_Rect leftBorder = { 0, 60, borderSize, windowHeight };
		SDL_Rect rightBorder = { windowWidth - borderSize, 60, borderSize, windowHeight };

		SDL_RenderFillRect(renderer, &topBorder);
		SDL_RenderFillRect(renderer, &bottomBorder);
		SDL_RenderFillRect(renderer, &leftBorder);
		SDL_RenderFillRect(renderer, &rightBorder);
		// food 
		SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
		SDL_RenderFillRect(renderer, &apple);

		//  snake head (image)
		double angle = 0;
		switch (dir) {
		case UP:
			angle = 270;
			break;
		case DOWN:
			angle = 90;
			break;
		case LEFT:
			angle = 180;
			break;
		case RIGHT:
			angle = 0;
			break;
		}
		SDL_Point center = { head.w / 2, head.h / 2 };
		SDL_RenderCopyEx(renderer, snakeHeadTexture, NULL, &head, angle, &center, SDL_FLIP_NONE);

		//  score
		std::string scoreText = "Score: " + std::to_string(score);
		SDL_Texture* scoreTexture = renderText(renderer, font, scoreText, color);
		SDL_Rect scoreRect = { 10, 10, 100, 50 };
		SDL_RenderCopy(renderer, scoreTexture, NULL, &scoreRect);
		SDL_DestroyTexture(scoreTexture);
		
		if (paused) {
			// Render pause text
			SDL_Surface* textsurf = TTF_RenderText_Solid(font, "PAUSED", color);
			SDL_Texture* texttexture = SDL_CreateTextureFromSurface(renderer, textsurf);
			SDL_FreeSurface(textsurf);
			SDL_Rect textRect = { 300, 250, 200, 100 }; 
			SDL_RenderCopy(renderer, texttexture, NULL, &textRect);
			SDL_DestroyTexture(texttexture);
		}
		SDL_RenderPresent(renderer);
		SDL_Delay(25);

		Uint32 frameTime = SDL_GetTicks() - frameStart;
		if (frameDelay > frameTime) {
			SDL_Delay(frameDelay - frameTime);
		}
	}

	
	SDL_DestroyTexture(snakeHeadTexture);
}

void game4(SDL_Renderer* renderer, TTF_Font* font, SDL_Color color) {
	bool running = true;
	bool paused = false;
	int dir = RIGHT;
	SDL_Event e;
	int score = 0;
	const int snakeStep = 20;
	const int windowWidth = 800;
	const int windowHeight = 550;
	const int frameDelay = 100;
	const int borderSize = 20;

	// Snake head 
	SDL_Rect head;
	head.x = 400;
	head.y = 280;
	head.w = snakeStep; 
	head.h = snakeStep; 

	SDL_Texture* snakeHeadTexture = IMG_LoadTexture(renderer, "headc.png"); 
	if (!snakeHeadTexture) {
		
		SDL_Log("Failed to load snake head texture: %s", SDL_GetError());
		return;
	}

	// Snake body 
	std::deque<SDL_Rect> rq;
	int size = 1;

	// food 
	SDL_Rect apple;
	apple.w = 12;
	apple.h = 12;
	apple.x = 30 + (rand() % ((740 - 30) / 10)) * 10;
	apple.y = 100 + (rand() % ((520 - 100) / 10)) * 10;

	while (running) {
		Uint32 frameStart = SDL_GetTicks();

		
		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT) {
				game4over = true;
				running = false;
			}
			if (e.type == SDL_KEYDOWN) {
				if (e.key.keysym.sym == SDLK_DOWN && dir != UP) dir = DOWN;
				else if (e.key.keysym.sym == SDLK_UP && dir != DOWN) dir = UP;
				else if (e.key.keysym.sym == SDLK_RIGHT && dir != LEFT) dir = RIGHT;
				else if (e.key.keysym.sym == SDLK_LEFT && dir != RIGHT) dir = LEFT;
				else if (e.key.keysym.sym == SDLK_p) paused = !paused; //p te paused
			}
		}

		if (!paused) {
			//  snake position
			switch (dir) {
			case DOWN:
				head.y += snakeStep;
				break;
			case UP:
				head.y -= snakeStep;
				break;
			case LEFT:
				head.x -= snakeStep;
				break;
			case RIGHT:
				head.x += snakeStep;
				break;
			}
			// eating food 
			if (SDL_HasIntersection(&head, &apple)) {
				
				playeatsound();
				size += 2;
				score += 5;
				apple.x = 30 + (rand() % ((740 - 30) / 10)) * 10;
				apple.y = 100 + (rand() % ((520 - 100) / 10)) * 10;
			}			
			for (const auto& snake_segment : rq) {
				if ((head.x == snake_segment.x && head.y == snake_segment.y) ||
					head.x < borderSize || head.y < borderSize ||
					head.x >= windowWidth - borderSize || head.y >= windowHeight - borderSize || head.y < 80) {
					playgameoversound();
					//cout << head.x <<" " << head.y << endl;
					storeHighScore(score);
					SDL_Surface* textsurf = TTF_RenderText_Solid(font, "GAME OVER", color);
					SDL_Texture* texttexture = SDL_CreateTextureFromSurface(renderer, textsurf);
					SDL_FreeSurface(textsurf);
					SDL_Rect textRect = { 200, 150, 400, 100 };
					SDL_RenderCopy(renderer, texttexture, NULL, &textRect);
					SDL_RenderPresent(renderer);
					SDL_Delay(2000);
					SDL_DestroyTexture(texttexture);
					game4over = true;
					running = false;
					break;
				}
			}
			rq.push_front(head);// snake movement
			while (rq.size() > size)
				rq.pop_back();
		}

		// window 
		SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
		SDL_RenderClear(renderer);

		
		SDL_SetRenderDrawColor(renderer, 0, 220, 0, 255);
		for (auto& snake_segment : rq) {
			if (snake_segment.x == head.x && snake_segment.y == head.y) {
				continue;
			}
			SDL_RenderFillRect(renderer, &snake_segment);
		}

	
		SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

		SDL_Rect topBorder = { 0, 60, windowWidth, borderSize };
		SDL_Rect bottomBorder = { 0, windowHeight - borderSize, windowWidth, borderSize };
		SDL_Rect leftBorder = { 0, 60, borderSize, windowHeight };
		SDL_Rect rightBorder = { windowWidth - borderSize, 60, borderSize, windowHeight };

		SDL_RenderFillRect(renderer, &topBorder);
		SDL_RenderFillRect(renderer, &bottomBorder);
		SDL_RenderFillRect(renderer, &leftBorder);
		SDL_RenderFillRect(renderer, &rightBorder);
		
		SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
		SDL_RenderFillRect(renderer, &apple);

		double angle = 0;
		switch (dir) {
		case UP:
			angle = 270;
			break;
		case DOWN:
			angle = 90;
			break;
		case LEFT:
			angle = 180;
			break;
		case RIGHT:
			angle = 0;
			break;
		}
		SDL_Point center = { head.w / 2, head.h / 2 };
		SDL_RenderCopyEx(renderer, snakeHeadTexture, NULL, &head, angle, &center, SDL_FLIP_NONE);

		
		std::string scoreText = "Score: " + std::to_string(score);
		SDL_Texture* scoreTexture = renderText(renderer, font, scoreText, color);
		SDL_Rect scoreRect = { 10, 10, 100, 50 };
		SDL_RenderCopy(renderer, scoreTexture, NULL, &scoreRect);
		SDL_DestroyTexture(scoreTexture);
		
		if (paused) {
			//  pause text
			SDL_Surface* textsurf = TTF_RenderText_Solid(font, "PAUSED", color);
			SDL_Texture* texttexture = SDL_CreateTextureFromSurface(renderer, textsurf);
			SDL_FreeSurface(textsurf);
			SDL_Rect textRect = { 300, 250, 200, 100 }; 
			SDL_RenderCopy(renderer, texttexture, NULL, &textRect);
			SDL_DestroyTexture(texttexture);
		}
		SDL_RenderPresent(renderer);
		SDL_Delay(25);

		
		Uint32 frameTime = SDL_GetTicks() - frameStart;
		if (frameDelay > frameTime) {
			SDL_Delay(frameDelay - frameTime);
		}
	}

	// Cleanup
	SDL_DestroyTexture(snakeHeadTexture);
}