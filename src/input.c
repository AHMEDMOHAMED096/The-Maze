#include "maze.h"

/**
 * handleInput-  Handles user input events and
 *  updates the corresponding movement.
 * @e: The SDL_Event to process.
 * @moveForward: A boolean flag indicating
 *  whether the player should move forward.
 * @moveBackward: A boolean flag indicating whether
 *  the player should move backward.
 * @moveLeft: A boolean flag indicating whether
 *  the player should move left.
 * @moveRight: A boolean flag indicating whether
 *  the player should move right.
 * @rotateLeft: A boolean flag indicating whether
 *  the player should rotate left.
 * @rotateRight: A boolean flag indicating whether
 *  the player should rotate right.
 * @quit: A boolean flag indicating whether the game should quit.
 * @showMap: A boolean flag indicating whether the map should be shown.
 */

void handleInput(SDL_Event e, bool *moveForward,
				 bool *moveBackward, bool *moveLeft,
				 bool *moveRight, bool *rotateLeft,
				 bool *rotateRight, bool *quit, bool *showMap)
{
	if (e.type == SDL_KEYDOWN || e.type == SDL_KEYUP)
	{
		bool keyPressed;

		keyPressed = (e.type == SDL_KEYDOWN);
		switch (e.key.keysym.sym)
		{
		case SDLK_w:
			*moveForward = keyPressed;
			break;
		case SDLK_s:
			*moveBackward = keyPressed;
			break;
		case SDLK_a:
			*moveLeft = keyPressed;
			break;
		case SDLK_d:
			*moveRight = keyPressed;
			break;
		case SDLK_LEFT:
			*rotateLeft = keyPressed;
			break;
		case SDLK_RIGHT:
			*rotateRight = keyPressed;
			break;
		case SDLK_ESCAPE:
			*quit = true;
			break;
		case SDLK_m:
			if (keyPressed)
				*showMap = !*showMap;
			break;
		}
	}
}

/**
 * updatePlayerPosition - Updates the player's position
 * and orientation based on input flags.
 * @moveForward: A boolean flag indicating whether
 *  the player should move forward.
 * @moveBackward: A boolean flag indicating whether
 *  the player should move backward.
 * @moveLeft: A boolean flag indicating whether
 *  the player should move left.
 * @moveRight: A boolean flag indicating whether
 *  the player should move right.
 * @rotateLeft: A boolean flag indicating whether
 *  the player should rotate left.
 * @rotateRight: A boolean flag indicating whether
 *  the player should rotate right.
 * @game: A pointer to Game structure.
 */

void updatePlayerPosition(bool moveForward, bool moveBackward,
						  bool moveLeft, bool moveRight, bool rotateLeft,
						  bool rotateRight, Game *game)
{
	float newX, newY;
	int mapX, mapY;

	newX = game->x, newY = game->y;
	if (moveForward && !moveBackward)
	{
		newX += cos(game->playerDir) * game->moveSpeed;
		newY += sin(game->playerDir) * game->moveSpeed;
	}
	if (moveBackward && !moveForward)
	{
		newX -= cos(game->playerDir) * game->moveSpeed;
		newY -= sin(game->playerDir) * game->moveSpeed;
	}
	if (moveLeft && !moveRight)
	{
		newX += sin(game->playerDir) * game->moveSpeed;
		newY -= cos(game->playerDir) * game->moveSpeed;
	}
	if (moveRight && !moveLeft)
	{
		newX -= sin(game->playerDir) * game->moveSpeed;
		newY += cos(game->playerDir) * game->moveSpeed;
	}
	mapX = (int)newX, mapY = (int)newY;
	if (game->map[mapX][mapY] == 0)
	{
		game->x = newX, game->y = newY;
	}
	else
	{
		if (game->map[(int)game->x][mapY] == 0)
			game->y = newY;
		else if (game->map[mapX][(int)game->y] == 0)
			game->x = newX;
	}
	if (rotateLeft && !rotateRight)
		game->playerDir -= game->rotationSpeed;
	if (rotateRight && !rotateLeft)
		game->playerDir += game->rotationSpeed;
}
