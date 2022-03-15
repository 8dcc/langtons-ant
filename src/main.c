/*
 *	https://github.com/r4v10l1/langtons-ant-c
 *	Langtons ant simulation made in pure C with SDL2.
 *	Based on https://github.com/r4v10l1/LangtonsAnt
 */

#include <stdio.h>
#include "SDL.h"

#include "misc.h"
#include "read_config.h"	// For config.cfg
#include "ant.h"

int main(int argc, char* argv[]) {
	
	// Read config file
	clear_colors_array();
	if (read_config() == 1) {
		printf("Error opening config.cfg\nExiting...\n");
		return 1;
	}
	if (DEBUG_PRINT) printf("Configuration loaded!\n");

	// Start SDL
	if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO) != 0) {
		SDL_Log("Unable to start: %s\n", SDL_GetError());
		return 1;
	}
	if (DEBUG_PRINT) printf("SLD started!\n");
	
	// Create window
	SDL_Window* sdl_window = SDL_CreateWindow("SDL2 test",  // Window name
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_W, WINDOW_H, 0); 
	if (!sdl_window) {
		printf("Error creating a window: %s\n", SDL_GetError());
		SDL_Quit();
		return 1;
	}
	if (DEBUG_PRINT) printf("Window created!\n");

	// Create renderer
	Uint32 render_flags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
	SDL_Renderer* sdl_renderer = SDL_CreateRenderer(sdl_window, -1, render_flags);
	if (!sdl_renderer) {
		printf("Error creating a renderer: %s\n", SDL_GetError());
		SDL_DestroyWindow(sdl_window);
		SDL_Quit();
		return 1;
	}
	if (DEBUG_PRINT) printf("Renderer created!\n\n");
	
	printf("Press space to start the simulation.\n");

	/* ------------------------------------------------------------------------------- */

	// Main loop
	int running = 1, space_pressed = 0; 

	SDL_Event sdl_event;	// Create an event for the keys and shit
	while (running == 1) {
		// Events
		while (SDL_PollEvent(&sdl_event)) {
			switch (sdl_event.type) {
				case SDL_QUIT:  // Window is closed
					running = 0;
					break;
				case SDL_KEYDOWN:
					// Check the pressed key
					switch (sdl_event.key.keysym.scancode) {
						case SDL_SCANCODE_ESCAPE:
							running = 0;
							if (DEBUG_PRINT == 1) {
								printf("Esc key pressed!\n");
							}
							break;
						case SDL_SCANCODE_SPACE:
							space_pressed = 1;
							break;
						default:
							break;
					}
					break;
				case SDL_KEYUP:
					// Check the released key
					switch (sdl_event.key.keysym.scancode) {
						case SDL_SCANCODE_SPACE:
							space_pressed = 0;
							if (DEBUG_PRINT == 1) {
								printf("Space key released!\n");
							}
							break;
						default:
							break;
					}
					break;
				default:
					break;
			}
		}

		/* ------------------------------------------------------------------------------- */

		// Clear window
		SDL_SetRenderDrawColor(sdl_renderer, BACKGROUND_R, BACKGROUND_G, BACKGROUND_B, 255);
		SDL_RenderClear(sdl_renderer);

		// Do something every frame the space is pressed
		if (space_pressed) {
			// Do shit
		}
		
		// Send to renderer
		SDL_RenderPresent(sdl_renderer);
		if (space_pressed && DELAY) {
			SDL_Delay(DELAY);
		} else {
			SDL_Delay(1000/FPS);
		}
	}

	// End of the program
	if (DEBUG_PRINT) printf("Reached end of the program!\n");
	SDL_DestroyRenderer(sdl_renderer);
	SDL_DestroyWindow(sdl_window);
	SDL_Quit();

	return 0;
}
