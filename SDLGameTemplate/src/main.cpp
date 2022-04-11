#include "engine.h"
#include "sprite.h"
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_mixer.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include<time.h>

// Forward function declarations
void Update(float dt);
void RenderFrame(float dt);

int grid_cell_size = 40;
int grid_cell_sprite_size = grid_cell_size / 1.001;
int const grid_width = 15;
int const grid_height = 13;
int grid_bomb = 40;
int WINDOW_WIDTH = (grid_width * grid_cell_size);
int WINDOW_HEIGHT = (grid_height * grid_cell_size);
int object_sprite_width = grid_width;
int object_sprite_height = grid_height * 2;


bool gameOver;
int GameOverCurrentTime = 0;
int GameOverTime = 100;

Uint32 mouse;
int mouse_x, mouse_y;
// Street texture
Sprite guy;
//=============================================================================-
//information about cells in grid
class grid
{
private:
	int type[grid_width][grid_height][1];
};
Mix_Chunk* sound_sel;
grid myGrid; //create grid
void startGame()
{

}
int main(int argc, char* argv[])
{
	int frequencia = 22050;
	Uint16 formato = AUDIO_S16SYS;
	int canal = 2; // 1 mono; 2 = stereo;
	int buffer = 4096;
	Mix_OpenAudio(frequencia, formato, canal, buffer);
	//Play audio test
	//Mix_PlayChannel(0, sound_test , 0);
	Mix_Chunk* sound_test;
	
	//sound_test = Mix_LoadWAV("assets/test.wav");
	

	if (!InitSDL())
	{
		return 1;
	}

	if (!CreateWindow("Bombsweeper", WINDOW_WIDTH, WINDOW_HEIGHT))
	{
		return 1;
	}
	guy = LoadSprite("assets/guy.png");
	srand(time(0));
	// Push functions to the game loop
	startGame();

	StartLoop(Update, RenderFrame);

	FreeSprite(guy);
	CleanUp();
	return 0;
}
int mouse_last_x = 0, mouse_last_y = 0;
//=============================================================================
void Update(float dt)
{
	SDL_PumpEvents();


}
void RenderFrame(float interpolation)
{
	// Clear screen
	SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 0);
	SDL_RenderClear(gRenderer);
	// Draw grid
	SDL_Rect grid = {
	0,
	0,
	grid_cell_size * grid_width,
	grid_cell_size * grid_height,
	};
	SDL_SetRenderDrawColor(gRenderer, 0, 100, 0, 255); //BACKGROUND
	SDL_RenderFillRect(gRenderer, &grid);
	SDL_SetRenderDrawColor(gRenderer, 0, 200, 0, 255);
	for (int h = 0; h < grid_height; h++)
	{
		for (int w = 0; w < grid_width; w++)
		{
			SDL_Rect cell = {
			(w * grid_cell_size + grid_cell_size / 2) - grid_cell_sprite_size / 2, //render cell.x in center
			(h * grid_cell_size + grid_cell_size / 2) - grid_cell_sprite_size / 2, //render cell.y in center
			grid_cell_sprite_size,
			grid_cell_sprite_size
			};
			SDL_RenderFillRect(gRenderer, &cell);
		}
	}
}
