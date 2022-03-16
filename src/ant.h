
enum directions { UP, DOWN, LEFT, RIGHT };

// x_pos, y_pos, facing_dir
int ANT_STATE[3];

// TODO: Will need to store the currents ant position in an array in main.c
void move_forward(int facing_direction, int ant_position_array[]) {
	switch (facing_direction) {
		case UP:
			ANT_STATE[1]++;
			break;
		case DOWN:
			ANT_STATE[1]--;
			break;
		case LEFT:
			ANT_STATE[0]--;
			break;
		case RIGHT:
			ANT_STATE[0]++;
			break;
		default:
			break;
	}
}

// rotation: 0 = ClockWise; 1 = CounterClockWise
int rotate(int rotation, int facing_direction) {
	if (rotation) {		// CW
		switch (facing_direction) {
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
		switch (facing_direction) {
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
	ANT_STATE[0] = WINDOW_W/CELL_SIZE;
	ANT_STATE[1] = WINDOW_H/CELL_SIZE; 
	ANT_STATE[2] = UP;
}

// TODO: Function to return rotation based on current color? or just main.c
