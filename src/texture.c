#include "maze.h"

/**
 * loadTexture - A function to load specific texture image
 *  and creates an SDL_Texture structure.
 * @file: The texture file to be opened.
 * @renderer: A structure for rendering operations.
 * Return: A pointer to the created SDL_Texture, or NULL if there was an error
 */

SDL_Texture *loadTexture(const char *file, SDL_Renderer *renderer)
{
	SDL_Texture *texture = NULL;
	SDL_Surface *loadedImage = NULL;

	loadedImage = IMG_Load(file);
	if (loadedImage != NULL)
	{
		texture = SDL_CreateTextureFromSurface(renderer, loadedImage);
		SDL_FreeSurface(loadedImage);
		if (texture == NULL)
		{
			printf("Unable to create texture from %s! SDL Error: %s\n",
				   file, SDL_GetError());
		}
	}
	else
	{
		printf("Unable to load image %s! SDL_image Error: %s\n",
			   file, IMG_GetError());
	}
	return (texture);
}
