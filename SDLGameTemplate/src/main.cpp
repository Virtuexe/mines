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

int grid_cell_size = 35;
int grid_cell_sprite_size = grid_cell_size / 1.001;
int const grid_width = 10;
int const grid_height = 10;
int grid_bomb = 10;
int WINDOW_WIDTH = (grid_width * grid_cell_size);
int WINDOW_HEIGHT = (grid_height * grid_cell_size);

Uint32 mouse;
int mouse_x, mouse_y;
// Street texture
Sprite num_test;
//=============================================================================-
//information about cells in grid
class grid_cell
{
private:
	bool visible[grid_width][grid_height];
	bool bomb[grid_width][grid_height];
	bool selected[grid_width][grid_height];
	int bombAmmount[grid_width][grid_height];
public:
	void cell(int x, int y, bool visible, bool bomb)
	{
		this->visible[x][y] = visible;
		this->bomb[x][y] = bomb;
	}
	bool isVisible(int x, int y)
	{
		return visible[x][y];
	}
	void setVisible(int x, int y, bool visible)
	{
		if (x >= grid_width || y >= grid_height || x < 0 || y < 0)
			return;
		this->visible[x][y] = visible;
	}
	bool isBomb(int x, int y)
	{
		if (x >= grid_width || y >= grid_height || x < 0 || y < 0)
			return false;
		return bomb[x][y];
	}
	void setBomb(int x, int y, bool bomb)
	{
		if (x >= grid_width || y >= grid_height || x < 0 || y < 0)
			return;
		this->bomb[x][y] = bomb;
	}
	void setSelected(int x, int y, bool selected)
	{
		if (x >= grid_width || y >= grid_height || x < 0 || y < 0)
			return;
		this->selected[x][y] = selected;
	}
	bool isSelected(int x, int y)
	{
		return selected[x][y];
	}
	int getBombs(int x, int y)
	{
		return bombAmmount[x][y];
	}
	//sets current cell as visible and if its neighbor is not bomb calls the same funcion on all other neighboring cells that are not visible
	//also saves ammount of bombs around current cell
	void destroyCell(int x, int y)
	{
		int mines = 0;
		if (x >= grid_width || y >= grid_height || x < 0 || y < 0)
			return;
		setVisible(x, y, true);
		for (int h = y - 1; h <= y + 1; h++)
		{
			for (int w = x - 1; w <= x + 1; w++)
			{
				if (isBomb(w, h))
				{
					mines++;
				}
			}
		}
		if (mines == 0)
		{
			for (int h = y - 1; h <= y + 1; h++)
			{
				for (int w = x - 1; w <= x + 1; w++)
				{
					if(!isVisible(w, h))
						destroyCell(w, h);
				}
			}
		}
		bombAmmount[x][y] = mines;
		return;
	}
};
grid_cell myGrid; //create grid
int main(int argc, char* argv[])
{
	if (!InitSDL())
	{
		return 1;
	}

	if (!CreateWindow("The Game", WINDOW_WIDTH, WINDOW_HEIGHT))
	{
		return 1;
	}
	num_test = LoadSprite("assets/test.png");
	srand(time(0));
	for (int i = 0; i < grid_bomb; i++)
	{
		int x = rand() % grid_width;
		int y = rand() % grid_height;
		if (myGrid.isBomb(x, y))
		{
			i--;
		}
		else
		{
			myGrid.setBomb(x, y, true);
		}
	}
	// Push functions to the game loop
	StartLoop(Update, RenderFrame);

	FreeSprite(num_test);
	CleanUp();
	return 0;
}
int mouse_last_x = 0, mouse_last_y = 0;
//=============================================================================
void Update(float dt)
{

	SDL_PumpEvents();
	// Change subsystem of project from Windows to Console
	// in order to see the stderr output
	if (IsKeyDown(SDL_SCANCODE_ESCAPE))
		ExitGame();
	if ((mouse & SDL_BUTTON_LMASK) != 0) {
		if(!myGrid.isVisible(mouse_x / grid_cell_size, mouse_y / grid_cell_size))
			myGrid.destroyCell(mouse_x / grid_cell_size, mouse_y / grid_cell_size);
	}

	mouse = SDL_GetMouseState(&mouse_x, &mouse_y);
	myGrid.setSelected(mouse_last_x / grid_cell_size, mouse_last_y / grid_cell_size, false);
	myGrid.setSelected(mouse_x/grid_cell_size,mouse_y/grid_cell_size,true);
	mouse_last_x = mouse_x;
	mouse_last_y= mouse_y;

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
	SDL_SetRenderDrawColor(gRenderer, 10, 10, 10, 255); //BACKGROUND
	SDL_RenderFillRect(gRenderer, &grid);
	SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255); //DEFAULT
	for (int h = 0; h < grid_height; h++)
	{
		for (int w = 0; w < grid_width; w++)
		{
			if (myGrid.isBomb(w, h) && myGrid.isVisible(w, h)) //BOMB
			{
				SDL_SetRenderDrawColor(gRenderer, 255, 0, 0, 255);
			}
			else if (myGrid.isSelected(w, h) && myGrid.isVisible(w, h)) //SELLECTED EMPTY
			{
				SDL_SetRenderDrawColor(gRenderer, 0, 255, 255, 255);
			}
			else if (myGrid.isVisible(w, h)) //EMPTY
			{
				SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
			}
			else if (myGrid.isSelected(w, h)) //SELECTED
			{
				SDL_SetRenderDrawColor(gRenderer, 0, 100, 255, 255);
			}
			SDL_Rect cell = {
			(w * grid_cell_size + grid_cell_size / 2) - grid_cell_sprite_size / 2, //render cell.x in center
			(h * grid_cell_size + grid_cell_size / 2) - grid_cell_sprite_size / 2, //render cell.y in center
			grid_cell_sprite_size,
			grid_cell_sprite_size
			};
			SDL_RenderFillRect(gRenderer, &cell);
			if (!myGrid.isBomb(w, h) && myGrid.isVisible(w, h) && myGrid.getBombs(w, h) > 0) //NUMBERS
			{
				switch(myGrid.getBombs(w, h))
				{
				case 1:
					SDL_RenderCopy(gRenderer, num_test.texture, NULL, &cell);
					break;
				case 2:
					SDL_RenderCopy(gRenderer, num_test.texture, NULL, &cell);
					break;
				case 3:
					SDL_RenderCopy(gRenderer, num_test.texture, NULL, &cell);
					break;
				case 4:
					SDL_RenderCopy(gRenderer, num_test.texture, NULL, &cell);
					break;
				case 5:
					SDL_RenderCopy(gRenderer, num_test.texture, NULL, &cell);
					break;
				case 6:
					SDL_RenderCopy(gRenderer, num_test.texture, NULL, &cell);
					break;
				case 7:
					SDL_RenderCopy(gRenderer, num_test.texture, NULL, &cell);
					break;
				case 8:
					SDL_RenderCopy(gRenderer, num_test.texture, NULL, &cell);
					break;
				case 9:
					SDL_RenderCopy(gRenderer, num_test.texture, NULL, &cell);
					break;
				default:
					SDL_RenderCopy(gRenderer, num_test.texture, NULL, &cell);
					break;
				}
			}
			SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);
		}
	}
}
