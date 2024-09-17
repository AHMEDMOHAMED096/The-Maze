#include "maze.h"

/**
 * destroySDL - Cleans up and destroys SDL resources.
 * @wallTexture: The SDL_Texture to be destroyed.
 * @floorTexture: The SDL_Texture to be destroyed.
 * @Renderer: The SDL_Renderer to be destroyed.
 * @Window: The SDL_Window to be destroyed.
 */

void destroySDL(SDL_Texture *wallTexture, SDL_Texture *floorTexture,
				SDL_Renderer *Renderer, SDL_Window *Window)
{
	if (wallTexture)
		SDL_DestroyTexture(wallTexture);
	if (floorTexture)
		SDL_DestroyTexture(floorTexture);
	if (Renderer)
		SDL_DestroyRenderer(Renderer);
	if (Window)
		SDL_DestroyWindow(Window);
	IMG_Quit();
	SDL_Quit();
}

/**
 * initializeSDL - Initializes the SDL library
 *  and creates a window and renderer.
 * @Window: A double pointer to the SDL_Window to be created.
 * @Renderer: A double pointer to the SDL_Renderer to be created.
 * Return: True on success.
 */

bool initializeSDL(SDL_Window **Window, SDL_Renderer **Renderer)
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		SDL_Quit();
		return (false);
	}

	*Window = SDL_CreateWindow("The Maze", SDL_WINDOWPOS_UNDEFINED,
							   SDL_WINDOWPOS_UNDEFINED,
							   WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
	if (*Window == NULL)
	{
		printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
		SDL_Quit();
		return (false);
	}

	*Renderer = SDL_CreateRenderer(*Window, -1, SDL_RENDERER_ACCELERATED);
	if (*Renderer == NULL)
	{
		printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
		SDL_DestroyWindow(*Window);
		SDL_Quit();
		return (false);
	}

	return (true);
}

/**
 * gameLoop - The main loop of the game, handling rendering and game logic.
 * @Renderer: A pointer to the SDL_Renderer used for rendering textures.
 * @wallTexture: A pointer to the SDL_Texture representing the wall texture.
 * @floorTexture: A pointer to the SDL_Texture representing the floor texture.
 * @mapFile: Path to the map file.
 * @game: A pointer to Game structure.
 */

void gameLoop(SDL_Renderer *Renderer, SDL_Texture *wallTexture,
			  SDL_Texture *floorTexture, const char *mapFile, Game *game)
{
	bool quit = false, showMap = false, moveForward = false, moveBackward = false;
	bool moveRight = false, rotateLeft = false;
	bool rotateRight = false, moveLeft = false;

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
							 moveRight, rotateLeft, rotateRight, game);
		/* Render the scene */
		SDL_SetRenderDrawColor(Renderer, 0x00, 0x00, 0x00, 0xFF);
		SDL_RenderClear(Renderer);
		parseMap(mapFile, game);
		castRays(Renderer, wallTexture, floorTexture, game);
		drawMapOnWindow(Renderer, &showMap, game);
		SDL_RenderPresent(Renderer);
	}
}

/**
 * main - Entry point to the game.
 * @argc: Command line arguments number.
 * @args: Command line arguments.
 * Return: 0 on success.
 */

int main(int argc, char *args[])
{
	Game Attributes;

	Attributes.x = 1.5f;
	Attributes.y = 1.5f;
	Attributes.playerDir = 0.0;
	Attributes.moveSpeed = 0.05f;
	Attributes.rotationSpeed = 0.03f;

	if (argc != 2)
	{
		fprintf(stderr, "Usage: %s <map file>\n", args[0]);
		return (EXIT_FAILURE);
	}

	SDL_Window *Window = NULL;
	SDL_Renderer *Renderer = NULL;
	SDL_Texture *wallTexture = NULL, *floorTexture = NULL;

	if (!initializeSDL(&Window, &Renderer))
	{
		return (EXIT_FAILURE);
	}

	if (!loadTextures(&wallTexture, &floorTexture, Renderer))
	{
		fprintf(stderr, "Failed to load textures\n");
		destroySDL(wallTexture, floorTexture, Renderer, Window);
		return (EXIT_FAILURE);
	}

	if (!parseMap(args[1], &Attributes))
	{
		fprintf(stderr, "Failed to parse map file: %s\n", args[1]);
		destroySDL(wallTexture, floorTexture, Renderer, Window);
		return (EXIT_FAILURE);
	}

	gameLoop(Renderer, wallTexture, floorTexture, args[1], &Attributes);
	destroySDL(wallTexture, floorTexture, Renderer, Window);
	return (0);
}
