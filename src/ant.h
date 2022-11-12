#define MAX_ANT_NUMBER 20

enum directions { UP, DOWN, LEFT, RIGHT };
enum rotations { CW, CCW, U_TURN, NO_ROTATION };

typedef struct ANT {
	int yp;			// Y position
	int xp;			// X position
	int rotation;	// Check the readme or the enum for rotations
} ant;

ant ANTS_ARRAY[MAX_ANT_NUMBER];

/* ------------------------------------------------------------------------------- */

void move_forward(int ant_i) {		// Ant index
	switch (ANTS_ARRAY[ant_i].rotation) {
		case UP:
			if (ANTS_ARRAY[ant_i].yp > 0) ANTS_ARRAY[ant_i].yp--;
			else ANTS_ARRAY[ant_i].yp = WINDOW_H/CELL_SIZE-1;
			break;
		case DOWN:
			if (ANTS_ARRAY[ant_i].yp < WINDOW_H/CELL_SIZE-1) ANTS_ARRAY[ant_i].yp++;
			else ANTS_ARRAY[ant_i].yp = 0;
			break;
		case LEFT:
			if (ANTS_ARRAY[ant_i].xp > 0) ANTS_ARRAY[ant_i].xp--;
			else ANTS_ARRAY[ant_i].xp = WINDOW_W/CELL_SIZE-1;
			break;
		case RIGHT:
			if (ANTS_ARRAY[ant_i].xp < WINDOW_W/CELL_SIZE-1) ANTS_ARRAY[ant_i].xp++;
			else ANTS_ARRAY[ant_i].xp = 0;
			break;
		default:
			break;
	}
}

/* ------------------------------------------------------------------------------- */
/* 
 * Rotations:
 * 		0 = ClockWise
 * 		1 = CounterClockWise
 * 		2 = U turn (Reverse rotation)
 * 		3 = No rotation
 */
int rotate(int rotation, int facing_direction) {
	if (rotation == CW) {
		switch (facing_direction) {
			case UP:		return RIGHT;
			case DOWN:		return LEFT;
			case LEFT:		return UP;	
			case RIGHT:		return DOWN;
			default:		break;
		}
	} else if (rotation == CCW) {
		switch (facing_direction) {
			case UP:		return LEFT;
			case DOWN:		return RIGHT;
			case LEFT:		return DOWN;
			case RIGHT:		return UP;
			default:		break;
		}
	} else if (rotation == U_TURN) {
		switch (facing_direction) {
			case UP:		return DOWN;
			case DOWN:		return UP;
			case LEFT:		return RIGHT;
			case RIGHT:		return LEFT;
			default:		break;
		}
	} else {	// NO_ROTATION or unknown value
		return facing_direction;
	}

	printf("\nError getting the rotation.\n");
	return -1;
}

/* ------------------------------------------------------------------------------- */

void draw_grid(SDL_Renderer* renderer) {
	SDL_SetRenderDrawColor(renderer, GRID_COLOR, GRID_COLOR, GRID_COLOR, 255);
	for (int x_grid = CELL_SIZE; x_grid < WINDOW_W; x_grid = x_grid + CELL_SIZE) {
		SDL_RenderDrawLine(renderer, x_grid, 0, x_grid, WINDOW_H);
	}
	for (int y_grid = CELL_SIZE; y_grid < WINDOW_H; y_grid = y_grid + CELL_SIZE) {
		SDL_RenderDrawLine(renderer, 0, y_grid, WINDOW_W, y_grid);
	}
}

void start_ant() {
	// Start at the center of the screen, facing up
	ANTS_ARRAY[0].yp = WINDOW_H/CELL_SIZE / 2;
	ANTS_ARRAY[0].xp = WINDOW_W/CELL_SIZE / 2; 
	ANTS_ARRAY[0].rotation = UP;

	for (int n = 1; n < MAX_ANT_NUMBER; n++) {
		ANTS_ARRAY[n].yp = -1;
		ANTS_ARRAY[n].xp = -1; 
		ANTS_ARRAY[n].rotation = -1;
	}
}

/* ------------------------------------------------------------------------------- */
/*
 * In order to access the 2d array, we pass the x and y size and then refer to the array as
 * 		cell_array[desiredx * x_size + desiredy] = ... ;
 * Credit to:
 *  https://stackoverflow.com/a/546891
 *  http://c-faq.com/aryptr/ary2dfunc2.html
 */
int move_ant(int* cell_array, int x_size, int ant_i) {
	int current_color = cell_array[ANTS_ARRAY[ant_i].yp * x_size + ANTS_ARRAY[ant_i].xp];
	int color_in_array = 0;		// The position in the COLORS_ARRAY[] of the current color. See next for loop.

	// We loop through the COLORS_ARRAY until we find the position of our color
	for (int n = 0; n < MAX_COLOR_NUMBER; n++) {
		if (current_color == COLORS_ARRAY[n]) {
			color_in_array = n;
			break;
		}
	}

	// Change the color of the current cell based on the previous one
	if (current_color == BACKGROUND) {
		cell_array[ANTS_ARRAY[ant_i].yp * x_size + ANTS_ARRAY[ant_i].xp] = COLORS_ARRAY[1];
	} else {
		if (color_in_array+1 >= COLOR_NUMBER) {
			cell_array[ANTS_ARRAY[ant_i].yp * x_size + ANTS_ARRAY[ant_i].xp] = COLORS_ARRAY[0];
		} else {
			cell_array[ANTS_ARRAY[ant_i].yp * x_size + ANTS_ARRAY[ant_i].xp] = COLORS_ARRAY[color_in_array+1];
		}
	}

	// Will check the rotation asigned to the current color, so if we encounter X color, we need to aply its rotation
	// which is specified in the config.cfg.
	ANTS_ARRAY[ant_i].rotation = rotate(ROTATIONS_ARRAY[color_in_array], ANTS_ARRAY[ant_i].rotation);

	// Move forward once
	move_forward(ant_i);

	return 0;
}

/* ------------------------------------------------------------------------------- */

// Given a color and an array, will write the RGB values of that color to the array
int rgb_from_color(int color, int rgb_buffer[]) {
	switch (color) {
		case 1:
			rgb_buffer[0] = 0;
			rgb_buffer[1] = 0;
			rgb_buffer[2] = 0;
			return 0;
		case 2:
			rgb_buffer[0] = 255;
			rgb_buffer[1] = 255;
			rgb_buffer[2] = 255;
			return 0;
		case 3:
			rgb_buffer[0] = 255;
			rgb_buffer[1] = 0;
			rgb_buffer[2] = 0;
			return 0;
		case 4:
			rgb_buffer[0] = 255;
			rgb_buffer[1] = 165;
			rgb_buffer[2] = 0;
			return 0;
		case 5:
			rgb_buffer[0] = 255;
			rgb_buffer[1] = 255;
			rgb_buffer[2] = 0;
			return 0;
		case 6:
			rgb_buffer[0] = 0;
			rgb_buffer[1] = 255;
			rgb_buffer[2] = 0;
			return 0;
		case 7:
			rgb_buffer[0] = 0;
			rgb_buffer[1] = 0;
			rgb_buffer[2] = 255;
			return 0;
		case 8:
			rgb_buffer[0] = 160;
			rgb_buffer[1] = 32;
			rgb_buffer[2] = 240;
			return 0;
		case 9:
			rgb_buffer[0] = 255;
			rgb_buffer[1] = 68;
			rgb_buffer[2] = 204;
			return 0;
		default:
			return 1;
			break;
	}
}
