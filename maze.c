#include <math.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <SDL2/SDL.h>

#define MAPWIDTH 3
#define MAPHEIGHT 3

const int screenWidth = 640;
const int screenHeight = 480;
SDL_Window *Window = NULL;
SDL_Renderer *Renderer = NULL;

void castRays(SDL_Renderer *renderer)
{
    const int map[MAPWIDTH][MAPHEIGHT] = {
        {1, 1, 1},
        {1, 0, 1},
        {1, 1, 1}};

    float playerX = 1.5;   // Player's X position in the map
    float playerY = 1.5;   // Player's Y position in the map
    float playerAngle = 0; // Player's direction

    for (int x = 0; x < screenWidth; x++)
    {
        float rayAngle = playerAngle - 0.5f + (float)x / screenWidth;

        float rayX = playerX;
        float rayY = playerY;

        float rayDirX = cos(rayAngle);
        float rayDirY = sin(rayAngle);

        bool hit = false;
        while (!hit)
        {
            rayX += rayDirX * 0.01f;
            rayY += rayDirY * 0.01f;

            int mapX = (int)rayX;
            int mapY = (int)rayY;

            if (mapX >= 0 && mapX < MAPWIDTH && mapY >= 0 && mapY < MAPHEIGHT)
            {
                if (map[mapX][mapY] == 1)
                {
                    hit = true;

                    // Calculate the distance to the wall
                    float dist = sqrt((rayX - playerX) * (rayX - playerX) + (rayY - playerY) * (rayY - playerY));

                    // Calculate wall height
                    int lineHeight = (int)(screenHeight / dist);

                    // Draw the wall slice
                    SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0xFF);
                    SDL_RenderDrawLine(renderer, x, (screenHeight / 2) - (lineHeight / 2), x, (screenHeight / 2) + (lineHeight / 2));
                }
            }
            else
            {
                hit = true;
            }
        }
    }
}

int main(int argc, char *args[])
{
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());

    else
    {
        // Create window
        Window = SDL_CreateWindow("The Maze Demo", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screenWidth, screenHeight, SDL_WINDOW_SHOWN);
        if (Window == NULL)
        {
            printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        }
        else
        {
            // Render the window
            Renderer = SDL_CreateRenderer(Window, -1, SDL_RENDERER_ACCELERATED);
        }
    }
    bool quit = false;
    SDL_Event e;
    while (!quit)
    {
        while (SDL_PollEvent(&e) != 0)
        {
            if (e.type == SDL_QUIT)
            {
                quit = true;
            }
        }

        // Render the scene
        SDL_SetRenderDrawColor(Renderer, 0x00, 0x00, 0x00, 0xFF);
        SDL_RenderClear(Renderer);
        castRays(Renderer);
        // Raycasting logic here
        SDL_RenderPresent(Renderer);
    }

    SDL_DestroyRenderer(Renderer);
    SDL_DestroyWindow(Window);
    SDL_Quit();

    return (0);
}
