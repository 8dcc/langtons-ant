
enum directions { UP, DOWN, LEFT, RIGHT };
enum rotations {CW, CCW};

// y_pos, x_pos, facing_dir
int ANT_STATE[3];

void move_forward() {
	switch (ANT_STATE[3]) {
		case UP:
			if (ANT_STATE[0] < WINDOW_H/CELL_SIZE) ANT_STATE[0]++;
			break;
		case DOWN:
			if (ANT_STATE[0] > 0) ANT_STATE[0]--;
			break;
		case LEFT:
			if (ANT_STATE[1] > 0) ANT_STATE[1]--;
			break;
		case RIGHT:
			if (ANT_STATE[1] < WINDOW_W/CELL_SIZE) ANT_STATE[1]++;
			break;
		default:
			break;
	}
}

// rotation: 0 = ClockWise; 1 = CounterClockWise
int rotate(int rotation) {
	if (rotation) {		// CW
		switch (ANT_STATE[3]) {
			case UP:
				return RIGHT;
				break;
			case DOWN:
				return LEFT;
				break;
			case LEFT:
				return UP;
				break;
			case RIGHT:
				return DOWN;
				break;
			default:
				break;
		}
	} else {			// CCW
		switch (ANT_STATE[3]) {
			case UP:
				return LEFT;
				break;
			case DOWN:
				return RIGHT;
				break;
			case LEFT:
				return DOWN;
				break;
			case RIGHT:
				return UP;
				break;
			default:
				break;
		}
	}

	return 1;
}

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
	ANT_STATE[0] = WINDOW_H/CELL_SIZE / 2;
	ANT_STATE[1] = WINDOW_W/CELL_SIZE / 2; 
	ANT_STATE[2] = UP;
}


/*
 * In order to access the 2d array, we pass the x and y size and then refer to the array as
 * 		cell_array[desiredx * x_size + desiredy] = ... ;
 * Credit to:
 *  https://stackoverflow.com/a/546891
 *  http://c-faq.com/aryptr/ary2dfunc2.html
 */
int move_ant(int* cell_array, int y_size, int x_size) {
	int current_color = cell_array[ANT_STATE[0] * x_size + ANT_STATE[1]];
	int color_in_array = 0;		// The position in the COLORS_ARRAY[] of the current color. See next for loop.

	// We loop through the COLORS_ARRAY until we find the position of our color
	for (int n = 0; n < MAX_COLOR_NUMBER; n++) {
		if (current_color == COLORS_ARRAY[n]) {
			color_in_array = n;
			break;
		}
	}

	//TODO: THE ANT STOPS AT THE 4TH COLOR
	// Change the color of the current cell based on the previous one
	if (current_color == BACKGROUND) {
		cell_array[ANT_STATE[0] * x_size + ANT_STATE[1]] = COLORS_ARRAY[0];
	} else {
		if (color_in_array+1 >= COLOR_NUMBER) {
			cell_array[ANT_STATE[0] * x_size + ANT_STATE[1]] = COLORS_ARRAY[0];
		} else {
			cell_array[ANT_STATE[0] * x_size + ANT_STATE[1]] = COLORS_ARRAY[color_in_array+1];
		}
	}
	if (DEBUG_PRINT) printf("Moved to: %3d|%3d\n", ANT_STATE[0], ANT_STATE[1]);

	// Will check the rotation asigned to the current color, so if we encounter X color, we need to aply its rotation
	// which is specified in the config.cfg.
	ANT_STATE[3] = rotate(ROTATIONS_ARRAY[color_in_array]);

	// Move forward once
	move_forward();

	return 0;
}

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


