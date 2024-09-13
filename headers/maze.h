#ifndef MAZE_H
#define MAZE_H

#include <math.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#define MAPWIDTH 24
#define MAPHEIGHT 24
#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 1080
#define PI 3.14159265358979323846

/**
 * struct Player - Represents a player in the game with position,
 * direction, and movement attributes.
 * @x: The x-coordinate of the player's position.
 * @y: The y-coordinate of the player's position.
 * @playerDir: The direction the player is facing.
 * @moveSpeed: The speed at which the player moves.
 * @rotationSpeed: The speed at which the player rotates.
 */

typedef struct
{
	float x, y, playerDir, moveSpeed, rotationSpeed;
} Player;

extern Player Attributes;
extern int map[MAPHEIGHT][MAPWIDTH];

void parseMap(const char *filePath, int map[MAPHEIGHT][MAPWIDTH]);
void handleInput(SDL_Event e, bool *moveForward,
				 bool *moveBackward, bool *moveLeft,
				 bool *moveRight, bool *rotateLeft,
				 bool *rotateRight, bool *quit, bool *showMap);

void updatePlayerPosition(bool moveForward, bool moveBackward, bool moveLeft,
						  bool moveRight, bool rotateLeft, bool rotateRight);

void drawMapOnWindow(SDL_Renderer *renderer, bool *showMap);
void drawMap(SDL_Renderer *renderer, SDL_Texture *wallTexture,
			 SDL_Texture *floorTexture,
			 int x, int lineHeight, bool isNorthSouthWall);

void castRays(SDL_Renderer *renderer, SDL_Texture *wallTexture,
			  SDL_Texture *floorTexture);

SDL_Texture *loadTexture(const char *file, SDL_Renderer *renderer);
void destroySDL(SDL_Texture *wallTexture, SDL_Texture *floorTexture,
				SDL_Renderer *Renderer, SDL_Window *Window);

#endif /* MAZE_H */
