#include "maze.h"
#define GRID_SIZE 40

/**
 * drawMap - Draws a 2D map representation using
 *  the provided textures and renderer.
 * @renderer: The SDL_Renderer to use for rendering the map.
 * @wallTexture: The SDL_Texture to use for rendering walls.
 * @floorTexture: The SDL_Texture to use for rendering the floor.
 * @currentPixel: The x-coordinate for the position to draw.
 * @lineHeight: The height of the line to draw.
 * @isNorthSouthWall: A boolean flag indicating whether
 *  the wall is oriented north-south.
 */

void drawMap(SDL_Renderer *renderer, SDL_Texture *wallTexture,
			 SDL_Texture *floorTexture,
			 int currentPixel, int lineHeight, bool isNorthSouthWall)
{
	SDL_Rect wall;
	int upper_half_of_map, lower_half_of_map;

	upper_half_of_map = (WINDOW_HEIGHT / 2) - (lineHeight / 2);
	lower_half_of_map = (WINDOW_HEIGHT / 2) + (lineHeight / 2);
	wall.x = currentPixel;
	wall.y = upper_half_of_map;
	wall.w = 1;
	wall.h = lineHeight;

	/* Render the sky */
	SDL_SetRenderDrawColor(renderer, 0x87, 0xCE, 0xEB, 0xFF);
	SDL_RenderDrawLine(renderer, currentPixel, 0, currentPixel, wall.y);

	/* Set wall color based on orientation */
	if (isNorthSouthWall)
		SDL_SetRenderDrawColor(renderer, 0xA9, 0xA9, 0xA9, 0xFF);
	else
		SDL_SetRenderDrawColor(renderer, 0xD3, 0xD3, 0xD3, 0xFF);

	/* Render the wall texture */
	SDL_RenderFillRect(renderer, &wall);
	SDL_RenderCopy(renderer, wallTexture, NULL, &wall);

	/* Render the floor */
	SDL_SetRenderDrawColor(renderer, 0x00, 0x80, 0x00, 0xFF);
	SDL_RenderDrawLine(renderer, currentPixel, lower_half_of_map,
					   currentPixel, WINDOW_HEIGHT);

	/* Render the floor texture */
	SDL_Rect floor;

	floor.x = currentPixel;
	floor.y = lower_half_of_map;
	floor.w = 1;
	floor.h = WINDOW_HEIGHT - floor.y;
	SDL_RenderCopy(renderer, floorTexture, NULL, &floor);
}

/**
 * drawMapOnWindow - Draws the map on the game window
 *  if the showMap flag is true.
 * @renderer: The SDL_Renderer to use for rendering the map.
 * @showMap: A pointer to a boolean flag indicating
 *  whether the map should be shown.
 * @game: A pointer to Game structure.
 */

void drawMapOnWindow(SDL_Renderer *renderer, bool *showMap, Game *game)
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
					   game->x + 150 * cos(game->playerDir),
					   game->y + 150 * sin(game->playerDir));
}

/**
 * parseMap - Parses a map file and loads its data into a 2D array.
 * @filePath: The path to the map file to load.
 * @game: A pointer to Game structure.
 * Return: True on success.
 */

bool parseMap(const char *filePath, Game *game)
{
	int height, width, i;
	FILE *file = NULL;
	char line[MAPWIDTH + 2];

	file = fopen(filePath, "r");
	if (file == NULL)
	{
		perror("Error opening file");
		return (false);
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
					game->map[height][width] = 1;
				}
				else if (line[i] == 'e')
				{
					game->map[height][width] = 0;
				}
				else
				{
					fprintf(stderr, "Invalid character in map file: %c\n", line[i]);
					fclose(file);
					return (false);
				}
				width++;
			}
		}
		height++;
	}
	fclose(file);
	return (true);
}

/**
 * castRays - Casts rays to render the 3D view of the game world.
 * @renderer: The SDL_Renderer to use for rendering the game.
 * @wallTexture: The SDL_Texture to use for rendering the walls.
 * @floorTexture: The SDL_Texture to use for rendering the floor.
 * @game: A pointer to Game structure.
 */

void castRays(SDL_Renderer *renderer, SDL_Texture *wallTexture,
			  SDL_Texture *floorTexture, Game *game)
{
	float rayAngle, rayX, rayY, rayDirX, rayDirY, distance;
	bool hit, isNorthSouthWall;
	int mapX, mapY, lineHeight, currentPixel;

	for (currentPixel = 0; currentPixel < WINDOW_WIDTH; currentPixel++)
	{
		rayAngle = game->playerDir - 0.5f + (float)currentPixel / WINDOW_WIDTH;
		rayX = game->x;
		rayY = game->y;
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
				if (game->map[mapX][mapY] == 1)
				{
					hit = true;
					if (fabs(rayDirX) > fabs(rayDirY))
						isNorthSouthWall = false;
					else
						isNorthSouthWall = true;
					distance = sqrt((rayX - game->x) * (rayX - game->x) +
									(rayY - game->y) * (rayY - game->y));
					lineHeight = (int)(WINDOW_HEIGHT / distance);
					drawMap(renderer, wallTexture, floorTexture,
							currentPixel, lineHeight, isNorthSouthWall);
				}
			}
			else
				hit = true;
		}
	}
}
