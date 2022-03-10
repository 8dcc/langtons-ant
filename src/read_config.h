#define CONFIG_BUFFER_SIZE 255
#define DEBUG_SETTINGS 0
#define MAX_COLOR_NUMBER 9

int WINDOW_W = 1200, WINDOW_H = 750;
int FPS = 60, DELAY = 50;
int DEBUG_PRINT = 0;

int COLOR_NUMBER = 2;
int BACKGROUND_R = 0, BACKGROUND_G = 0, BACKGROUND_B = 0;
int COLORS_ARRAY[MAX_COLOR_NUMBER];

void clear_colors_array() {
	for (int n = 0; n < MAX_COLOR_NUMBER; n++) {
		COLORS_ARRAY[n] = n;
	}
}

int parse_setting(char setting[], int value) {
	if (compare_strings(setting, "WINDOW_W") || compare_strings(setting, "window_w")) {
		WINDOW_W = value;
	} else if (compare_strings(setting, "WINDOW_H") || compare_strings(setting, "window_h")) {
		WINDOW_H = value;
	} else if (compare_strings(setting, "FPS") || compare_strings(setting, "fps")) {
		FPS = value;
	} else if (compare_strings(setting, "DELAY") || compare_strings(setting, "delay")) {
		DELAY = value;
	} else if (compare_strings(setting, "DEBUG_PRINT") || compare_strings(setting, "debug_print")) {
		DEBUG_PRINT = value;
	} else if (compare_strings(setting, "COLOR_NUMBER") || compare_strings(setting, "color_number")) {
		COLOR_NUMBER = value;
	} else if (compare_strings(setting, "BACKGROUND_R") || compare_strings(setting, "background_r")) {
		BACKGROUND_R = value;
	} else if (compare_strings(setting, "BACKGROUND_G") || compare_strings(setting, "background_g")) {
		BACKGROUND_G = value;
	} else if (compare_strings(setting, "BACKGROUND_B") || compare_strings(setting, "background_b")) {
		BACKGROUND_B = value;
	} else {
		// For each color get the value, if there is no value set it to 0
		char string_to_check[8] = "COLOR_";
		string_to_check[8] = '\0';
		char string_to_check_low[8] = "color_";
		string_to_check_low[8] = '\0';
		for (int n = 0; n < COLOR_NUMBER; n++) {
			string_to_check[7] = n+48;		// We do this so we get "COLOR_1", "COLOR_2", etc.
			string_to_check_low[7] = n+48;
			if (compare_strings(setting, string_to_check) || compare_strings(setting, string_to_check_low)) {
				COLORS_ARRAY[n] = value;	// Value will be depending on the code colors in the readme
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

