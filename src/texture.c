#include "maze.h"

/**
 * loadTexture - Function to load specific texture image
 *  and creates an SDL_Texture structure.
 * @file: The texture file to be opened.
 * @renderer: Structure for rendering operations.
 * Return: Pointer to the created SDL_Texture, or NULL if there was an error
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
		else
		{
			SDL_SetTextureScaleMode(texture, SDL_ScaleModeNearest);
		}
	}
	else
	{
		printf("Unable to load image %s! SDL_image Error: %s\n",
			   file, IMG_GetError());
	}
	return (texture);
}

/**
 * loadTextures - Function to load game textures.
 * @wallTexture: A double pointer to wallTexture to be loaded.
 * @floorTexture: A double pointer to floorTexture to be loaded.
 * @Renderer: Structure for rendering operations.
 * Return: True on success.
 */

bool loadTextures(SDL_Texture **wallTexture,
				  SDL_Texture **floorTexture, SDL_Renderer *Renderer)
{
	*wallTexture = loadTexture("wallTexture.png", Renderer);
	*floorTexture = loadTexture("floorTexture.jpg", Renderer);

	return ((*wallTexture != NULL || *floorTexture != NULL));
}
