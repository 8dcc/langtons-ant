/*
 *	https://github.com/r4v10l1/langtons-ant
 *	Langtons ant simulation made in pure C with SDL2.
 *	Based on https://github.com/r4v10l1/LangtonsAnt
 */

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "SDL.h"

#include "defines.h"
#include "misc.h"
#include "read_config.h"    // For config.cfg
#include "ant.h"

int main(int argc, char* argv[]) {
    // Read config file
    clear_config_arrays();
    if (read_config() == 1)
        die("Error opening config.cfg\nExiting...\n");

    // Start SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0)
        die("Unable to start SDL\n");

    // Create window
    SDL_Window* sdl_window =
      SDL_CreateWindow("Langton's ant", SDL_WINDOWPOS_CENTERED,
                       SDL_WINDOWPOS_CENTERED, WINDOW_W, WINDOW_H, 0);
    if (!sdl_window)
        die("Error creating a window\n");

    // Create renderer
    Uint32 render_flags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
    SDL_Renderer* sdl_renderer = SDL_CreateRenderer(sdl_window, -1, render_flags);
    if (!sdl_renderer) {
        SDL_DestroyWindow(sdl_window);
        die("Error creating a renderer\n");
    }

    start_ant();

    int cell_grid[WINDOW_H / CELL_SIZE][WINDOW_W / CELL_SIZE];

    // Clear the array
    for (int y = 0; y < WINDOW_H / CELL_SIZE; y++) {
        for (int x = 0; x < WINDOW_W / CELL_SIZE; x++) {
            cell_grid[y][x] = BACKGROUND;
        }
    }

    // Main loop
    int running = 1, grid_active = 0;
    int space_pressed = 0, r_arrow_pressed = 0;
    int number_of_steps = 0;
    rgb rgb_buf         = { 0, 0, 0 };

    SDL_Rect current_cell;

    SDL_Event sdl_event;    // Create an event for the keys and shit
    while (running) {
        if (r_arrow_pressed)
            r_arrow_pressed = 0;    // Restore arrow state so it executes once only

        // Events
        while (SDL_PollEvent(&sdl_event)) {
            switch (sdl_event.type) {
                case SDL_QUIT:
                    running = 0;
                    break;
                case SDL_KEYDOWN:
                    // Check the pressed key
                    switch (sdl_event.key.keysym.scancode) {
                        case SDL_SCANCODE_ESCAPE:
                        case SDL_SCANCODE_Q:
                            running = 0;
                            break;
                        case SDL_SCANCODE_G:
                            grid_active = !grid_active;
                            break;
                        case SDL_SCANCODE_SPACE:
                            space_pressed = !space_pressed;
                            break;
                        case SDL_SCANCODE_RIGHT:
                            r_arrow_pressed = 1;
                            break;
                        default:
                            break;
                    }
                    break;
                default:
                    break;
            }
        }

        // Clear window
        SDL_SetRenderDrawColor(sdl_renderer, BACKGROUND_R, BACKGROUND_G,
                               BACKGROUND_B, 255);
        SDL_RenderClear(sdl_renderer);

        // Do something every frame the space is pressed
        if (space_pressed || r_arrow_pressed) {
            for (int n = 0; n < MAX_ANT_NUMBER; n++) {
                if (ANTS_ARRAY[n].yp >= 0 && ANTS_ARRAY[n].xp >= 0)
                    // See comment before this function in ant.h
                    move_ant(&cell_grid[0][0], WINDOW_W / CELL_SIZE, n);
            }
        }

        // Draw the cells
        for (int y = 0; y < WINDOW_H / CELL_SIZE; y++) {
            for (int x = 0; x < WINDOW_W / CELL_SIZE; x++) {
                // Only draw colors if the cell is not empty (background)
                if (cell_grid[y][x] > BACKGROUND) {
                    // Convert int color to rgb values for sdl
                    color2rgb(cell_grid[y][x], &rgb_buf);
                    SDL_SetRenderDrawColor(sdl_renderer, rgb_buf.r, rgb_buf.g,
                                           rgb_buf.b, 255);

                    current_cell.x = x * CELL_SIZE;
                    current_cell.y = y * CELL_SIZE;
                    current_cell.w = CELL_SIZE;
                    current_cell.h = CELL_SIZE;
                    SDL_RenderFillRect(sdl_renderer, &current_cell);
                }
            }
        }

        // Draw the ant
        for (int n = 0; n < MAX_ANT_NUMBER; n++) {
            if (ANTS_ARRAY[n].yp >= 0 && ANTS_ARRAY[n].xp >= 0) {
                SDL_SetRenderDrawColor(sdl_renderer, 150, 0, 0, 255);
                current_cell.x = ANTS_ARRAY[n].xp * CELL_SIZE + 1;
                current_cell.y = ANTS_ARRAY[n].yp * CELL_SIZE + 1;
                current_cell.w = CELL_SIZE - 2;
                current_cell.h = CELL_SIZE - 2;
                SDL_RenderFillRect(sdl_renderer, &current_cell);
            }
        }

        // Draw grid if active
        if (grid_active)
            draw_grid(sdl_renderer);

        // Send to renderer
        SDL_RenderPresent(sdl_renderer);
        if (space_pressed && DELAY) {
            SDL_Delay(DELAY);
        } else {
            SDL_Delay(1000 / FPS);
        }
    }

    // End of the program
    SDL_DestroyRenderer(sdl_renderer);
    SDL_DestroyWindow(sdl_window);
    SDL_Quit();

    return 0;
}
