#include "maze.h"

Player Attributes = {1.5, 1.5, 0, 0.03, 0.03};
int map[MAPHEIGHT][MAPWIDTH] = {0};

/**
 * castRays - Casts rays to render the 3D view of the game world.
 * @renderer: The SDL_Renderer to use for rendering the game.
 * @wallTexture: The SDL_Texture to use for rendering the walls.
 * @floorTexture: The SDL_Texture to use for rendering the floor.
 */

void castRays(SDL_Renderer *renderer, SDL_Texture *wallTexture,
			  SDL_Texture *floorTexture)
{
	float rayAngle, rayX, rayY, rayDirX, rayDirY, distance;
	bool hit, isNorthSouthWall;
	int mapX, mapY, lineHeight, x;

	for (x = 0; x < WINDOW_WIDTH; x++)
	{
		rayAngle = Attributes.playerDir - 0.5f + (float)x / WINDOW_WIDTH;
		rayX = Attributes.x;
		rayY = Attributes.y;
		rayDirX = cos(rayAngle);
		rayDirY = sin(rayAngle);
		hit = false;
		isNorthSouthWall = false;
		while (!hit)
		{
			rayX += rayDirX * 0.01f;
			rayY += rayDirY * 0.01f;
			mapX = (int)rayX;
			mapY = (int)rayY;

			if (mapX >= 0 && mapX < MAPWIDTH && mapY >= 0 && mapY < MAPHEIGHT)
			{
				if (map[mapX][mapY] == 1)
				{
					hit = true;
					if (fabs(rayDirX) > fabs(rayDirY))
						isNorthSouthWall = false;
					else
						isNorthSouthWall = true;
					distance = sqrt((rayX - Attributes.x) * (rayX - Attributes.x) +
									(rayY - Attributes.y) * (rayY - Attributes.y));
					lineHeight = (int)(WINDOW_HEIGHT / distance);
					drawMap(renderer, wallTexture, floorTexture,
							x, lineHeight, isNorthSouthWall);
				}
			}
			else
				hit = true;
		}
	}
}

/**
 * destroySDL - Cleans up and destroys SDL resources.
 * @wallTexture: The SDL_Texture to be destroyed.
 * @Renderer: The SDL_Renderer to be destroyed.
 * @Window: The SDL_Window to be destroyed.
 */

void destroySDL(SDL_Texture *wallTexture, SDL_Renderer *Renderer, SDL_Window *Window)
{
	SDL_DestroyTexture(wallTexture);
	SDL_DestroyRenderer(Renderer);
	SDL_DestroyWindow(Window);
	IMG_Quit();
	SDL_Quit();
}

/**
 * main - Entry point to the game.
 * @argc: Command line arguments number.
 * @args: Command line arguments.
 * Return: 0 on success.
 */

int main(int argc, char *args[])
{
	if (argc != 2)
	{
		fprintf(stderr, "Usage: %s <map file>\n", args[0]);
		return (EXIT_FAILURE);
	}
	SDL_Window *Window = NULL;
	SDL_Renderer *Renderer = NULL;

	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		return (1);
	}
	else
	{
		Window = SDL_CreateWindow("The Maze", SDL_WINDOWPOS_UNDEFINED,
								  SDL_WINDOWPOS_UNDEFINED,
								  WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
		if (Window == NULL)
		{
			printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
			return (1);
		}
		else
		{
			Renderer = SDL_CreateRenderer(Window, -1, SDL_RENDERER_ACCELERATED);
			if (Renderer == NULL)
			{
				printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
				SDL_DestroyWindow(Window);
				SDL_Quit();
				return (1);
			}
		}
	}
	SDL_Texture *wallTexture = NULL, *floorTexture = NULL;
	bool quit = false, showMap = false, moveForward = false, moveBackward = false;
	bool moveRight = false, rotateLeft = false,
		 rotateRight = false, moveLeft = false;

	wallTexture = loadTexture("wallTexture.jpg", Renderer);
	floorTexture = loadTexture("floorTexture.jpg", Renderer);
	while (!quit)
	{
		SDL_Event e;

		while (SDL_PollEvent(&e) != 0)
		{
			if (e.type == SDL_QUIT)
				quit = true;
			else
				handleInput(e, &moveForward, &moveBackward, &moveLeft, &moveRight,
							&rotateLeft, &rotateRight, &quit, &showMap);
		}
		updatePlayerPosition(moveForward, moveBackward, moveLeft,
							 moveRight, rotateLeft, rotateRight);
		/* Render the scene */
		SDL_SetRenderDrawColor(Renderer, 0x00, 0x00, 0x00, 0xFF);
		SDL_RenderClear(Renderer);
		parseMap(args[1], map);
		castRays(Renderer, wallTexture, floorTexture);
		drawMapOnWindow(Renderer, &showMap);
		SDL_RenderPresent(Renderer);
	}
	destroySDL(wallTexture, Renderer, Window);
	return (0);
}
