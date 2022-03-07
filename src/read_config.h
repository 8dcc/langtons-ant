#define CONFIG_BUFFER_SIZE 255
#define DEBUG_SETTINGS 0

int WINDOW_W = 1200, WINDOW_H = 750;
int FPS = 60, DELAY = 50;
int DEBUG_PRINT = 0;

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

