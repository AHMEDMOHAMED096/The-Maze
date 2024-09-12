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
 */

void updatePlayerPosition(bool moveForward, bool moveBackward,
						  bool moveLeft, bool moveRight, bool rotateLeft, bool rotateRight)
{
	float newX, newY;
	int mapX, mapY;

	newX = Attributes.x;
	newY = Attributes.y;
	if (moveForward && !moveBackward)
	{
		newX += cos(Attributes.playerDir) * Attributes.moveSpeed;
		newY += sin(Attributes.playerDir) * Attributes.moveSpeed;
	}
	if (moveBackward && !moveForward)
	{
		newX -= cos(Attributes.playerDir) * Attributes.moveSpeed;
		newY -= sin(Attributes.playerDir) * Attributes.moveSpeed;
	}
	if (moveLeft && !moveRight)
	{
		newX += sin(Attributes.playerDir) * Attributes.moveSpeed;
		newY -= cos(Attributes.playerDir) * Attributes.moveSpeed;
	}
	if (moveRight && !moveLeft)
	{
		newX -= sin(Attributes.playerDir) * Attributes.moveSpeed;
		newY += cos(Attributes.playerDir) * Attributes.moveSpeed;
	}
	mapX = (int)newX;
	mapY = (int)newY;
	if (map[mapX][mapY] == 0)
	{
		Attributes.x = newX;
		Attributes.y = newY;
	}
	else
	{
		if (map[(int)Attributes.x][mapY] == 0)
			Attributes.y = newY;
		else if (map[mapX][(int)Attributes.y] == 0)
			Attributes.x = newX;
	}
	if (rotateLeft && !rotateRight)
		Attributes.playerDir -= Attributes.rotationSpeed;
	if (rotateRight && !rotateLeft)
		Attributes.playerDir += Attributes.rotationSpeed;
}
