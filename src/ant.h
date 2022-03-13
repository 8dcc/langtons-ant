
enum directions { UP, DOWN, LEFT, RIGHT };

// TODO: Will need to store the currents ant position in an array in main.c
void move_forward(int facing_direction, int ant_position_array[]) {
	switch (facing_direction) {
		case UP:
			ant_position_array[1]++;
			break;
		case DOWN:
			ant_position_array[1]--;
			break;
		case LEFT:
			ant_position_array[0]--;
			break;
		case RIGHT:
			ant_position_array[0]++;
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
				break
			case LEFT:
				return UP;
				break
			case RIGHT:
				return DOWN;
				break
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
				break
			case LEFT:
				return DOWN;
				break
			case RIGHT:
				return UP;
				break
			default:
				break;
		}
	}

	return 1;
}

// TODO: Function to return rotation based on current color? or just main.c
