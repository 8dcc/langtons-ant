#define CONFIG_BUFFER_SIZE 255
#define DEBUG_SETTINGS 0
#define MAX_COLOR_NUMBER 9

// -------------------------------------------------------------------------
// Default settings

enum colors { BACKGROUND, BLACK, WHITE, RED, ORANGE, YELLOW, GREEN, BLUE, PURPLE, PINK };

int WINDOW_W = 750, WINDOW_H = 750;								// Window size in pixels
int CELL_SIZE = 10;												// Size of the squares. In pixels
int FPS = 60, DELAY = 50;										// FPS used by the program normally vs holding space
int DEBUG_PRINT = 1;											// Print useful debug information

int COLOR_NUMBER = 2;
int GRID_COLOR = 40;											// As RGB
int BACKGROUND_R = 20, BACKGROUND_G = 20, BACKGROUND_B = 22;	// As RGB
int COLORS_ARRAY[MAX_COLOR_NUMBER];						 		// Will store the colors defined by the user
int ROTATIONS_ARRAY[MAX_COLOR_NUMBER];							// Will store the rotation that the ant should do when encountered X color

// -------------------------------------------------------------------------

void clear_config_arrays() {
	for (int n = 0; n < MAX_COLOR_NUMBER; n++) {
		COLORS_ARRAY[n] = n+1;
		ROTATIONS_ARRAY[n] = n % 2;
	}
}

int parse_setting(char setting[], int value) {
	if (compare_strings(setting, "WINDOW_W") || compare_strings(setting, "window_w")) {
		WINDOW_W = value;
	} else if (compare_strings(setting, "WINDOW_H") || compare_strings(setting, "window_h")) {
		WINDOW_H = value;
	} else if (compare_strings(setting, "CELL_SIZE") || compare_strings(setting, "cell_size")) {
		CELL_SIZE = value;
	} else if (compare_strings(setting, "FPS") || compare_strings(setting, "fps")) {
		FPS = value;
	} else if (compare_strings(setting, "DELAY") || compare_strings(setting, "delay")) {
		DELAY = value;
	} else if (compare_strings(setting, "DEBUG_PRINT") || compare_strings(setting, "debug_print") ||
		  compare_strings(setting, "VERBOSE") || compare_strings(setting, "verbose")) {
		DEBUG_PRINT = value;
	} else if (compare_strings(setting, "COLOR_NUMBER") || compare_strings(setting, "color_number")) {
		if (COLOR_NUMBER < 2) {
			COLOR_NUMBER = 2;
		} else if (COLOR_NUMBER > 9) {
			COLOR_NUMBER = 9;
		} else {
			COLOR_NUMBER = value;
		}
	} else if (compare_strings(setting, "BACKGROUND_R") || compare_strings(setting, "background_r")) {
		BACKGROUND_R = value;
	} else if (compare_strings(setting, "BACKGROUND_G") || compare_strings(setting, "background_g")) {
		BACKGROUND_G = value;
	} else if (compare_strings(setting, "BACKGROUND_B") || compare_strings(setting, "background_b")) {
		BACKGROUND_B = value;
	} else if (compare_strings(setting, "GRID_COLOR") || compare_strings(setting, "grid_color")) {
		GRID_COLOR = value;
	} else {
		// For each color get the value, if there is no value set it to 0
		char color_check_u[20], color_check_l[20];
		char rotation_check_u[20], rotation_check_l[20];

		strncpy(color_check_u, "COLOR_0", 20);
		strncpy(color_check_l, "color_0", 20);
		strncpy(rotation_check_u, "ROTATION_0", 20);
		strncpy(rotation_check_l, "rotation_0", 20);

		for (int n = 0; n < MAX_COLOR_NUMBER; n++) {
			color_check_u[6] = n+48;		// We do this so we get "COLOR_1", "COLOR_2", etc.
			color_check_l[6] = n+48;
			rotation_check_u[9] = n+48;
			rotation_check_l[9] = n+48;
			if (compare_strings(setting, color_check_u) || compare_strings(setting, color_check_l)) {
				COLORS_ARRAY[n] = value;		// Value will be depending on the code colors in the readme
			} else if (compare_strings(setting, rotation_check_u) || compare_strings(setting, rotation_check_l)) {
				ROTATIONS_ARRAY[n] = value;
			}
		}
	}

	return 0;
}

int read_config() {
	char line[CONFIG_BUFFER_SIZE], setting[CONFIG_BUFFER_SIZE];
	int value = -1;
	FILE * config_file;

	config_file = fopen("config.cfg", "r");

	if (config_file == NULL) return 1;

	while ( fgets(line, sizeof(line), config_file) ) {
		sscanf(line, "%s = %d", setting, &value);
		if (setting[0] != '#') {	// Ignore comments
			if (DEBUG_SETTINGS) printf("Found setting: %10s | %d\n", setting, value);
			parse_setting(setting, value);
		}
	}

	fclose(config_file);
	return 0;
}

