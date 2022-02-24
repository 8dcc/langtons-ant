#define CONFIG_BUFFER_SIZE 255
#define DEBUG_SETTINGS 1

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
		}
	}

	fclose(config_file);
	return 0;
}

int parse_setting(char setting, int value) {
	// TODO
	switch (setting) {
		case "a":
			a = value;
			break;
		default:
			break;
	}

	return 0;
}
