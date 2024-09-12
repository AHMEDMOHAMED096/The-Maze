#include "maze.h"
#define GRID_SIZE 40

/**
 * drawMap - Draws a 2D map representation using
 *  the provided textures and renderer.
 * @renderer: The SDL_Renderer to use for rendering the map.
 * @wallTexture: The SDL_Texture to use for rendering walls.
 * @floorTexture: The SDL_Texture to use for rendering the floor.
 * @x: The x-coordinate for the position to draw.
 * @lineHeight: The height of the line to draw.
 * @isNorthSouthWall: A boolean flag indicating whether
 *  the wall is oriented north-south.
 */

void drawMap(SDL_Renderer *renderer, SDL_Texture *wallTexture,
			 SDL_Texture *floorTexture,
			 int x, int lineHeight, bool isNorthSouthWall)
{
	SDL_Rect wall;

	wall.x = x;
	wall.y = (WINDOW_HEIGHT / 2) - (lineHeight / 2);
	wall.w = 1;
	wall.h = lineHeight;

	/* Render the sky */
	SDL_SetRenderDrawColor(renderer, 0x87, 0xCE, 0xEB, 0xFF);
	SDL_RenderDrawLine(renderer, x, 0, x, wall.y);

	/* Set wall color based on orientation */
	// if (isNorthSouthWall)
	// 	SDL_SetRenderDrawColor(renderer, 0xA9, 0xA9, 0xA9, 0xFF);
	// else
	// 	SDL_SetRenderDrawColor(renderer, 0xD3, 0xD3, 0xD3, 0xFF);

	/* Render the wall texture */
	SDL_RenderFillRect(renderer, &wall);
	SDL_RenderCopy(renderer, wallTexture, NULL, &wall);

	/* Render the floor texture */
	// SDL_Rect floor;

	// floor.x = x;
	// floor.y = (WINDOW_HEIGHT / 2) + (lineHeight / 2);
	// floor.w = 1;
	// floor.h = WINDOW_HEIGHT - floor.y;
	// SDL_RenderCopy(renderer, floorTexture, NULL, &floor);

	/* Render the floor */
	SDL_SetRenderDrawColor(renderer, 0x00, 0x80, 0x00, 0xFF);
	SDL_RenderDrawLine(renderer, x, (WINDOW_HEIGHT / 2) + (lineHeight / 2),
					   x, WINDOW_HEIGHT);
}

/**
 * drawMapOnWindow - Draws the map on the game window
 *  if the showMap flag is true.
 * @renderer: The SDL_Renderer to use for rendering the map.
 * @showMap: A pointer to a boolean flag indicating
 *  whether the map should be shown.
 */

void drawMapOnWindow(SDL_Renderer *renderer, bool *showMap)
{
	int i, j;

	if (!*showMap)
		return;

	/* Draw the map grid */
	SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);

	for (i = 0; i < WINDOW_WIDTH; i += GRID_SIZE)
	{
		SDL_RenderDrawLine(renderer, i, 0, i, WINDOW_HEIGHT);
	}

	for (j = 0; j < WINDOW_HEIGHT; j += GRID_SIZE)
	{
		SDL_RenderDrawLine(renderer, 0, j, WINDOW_WIDTH, j);
	}

	/* Draw the line of sight */
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
	SDL_RenderDrawLine(renderer, WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2,
					   Attributes.x + 150 * cos(Attributes.playerDir),
					   Attributes.y + 150 * sin(Attributes.playerDir));
}

/**
 * parseMap - Parses a map file and loads its data into a 2D array.
 * @filePath: The path to the map file to load.
 * @map: A 2D array to store the map data.
 */

void parseMap(const char *filePath, int map[MAPHEIGHT][MAPWIDTH])
{
	int height, width, i;
	FILE *file = NULL;
	char line[MAPWIDTH + 2];

	file = fopen(filePath, "r");
	if (file == NULL)
	{
		perror("Error opening file");
		exit(EXIT_FAILURE);
	}

	height = 0;
	while (fgets(line, sizeof(line), file) && height < MAPHEIGHT)
	{
		width = 0;
		for (i = 0; line[i] != '\0' && line[i] != '\n'; i++)
		{
			if (width < MAPWIDTH)
			{
				if (line[i] == 'w')
				{
					map[height][width] = 1;
				}
				else if (line[i] == 'e')
				{
					map[height][width] = 0;
				}
				else
				{
					fprintf(stderr, "Invalid character in map file: %c\n", line[i]);
					exit(EXIT_FAILURE);
				}
				width++;
			}
		}
		height++;
	}

	fclose(file);
}
