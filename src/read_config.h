
void clear_config_arrays() {
    for (int n = 0; n < MAX_COLOR_NUMBER; n++) {
        COLORS_ARRAY[n]    = n + 1;
        ROTATIONS_ARRAY[n] = n % 2;
    }
}

void stolower(char* s) {
    for (int i = 0; s[i] != '\0'; i++)
        s[i] = tolower(s[i]);
}

int parse_setting(char setting[], int value) {
    stolower(setting);

    if (!strcmp(setting, "window_w")) {
        WINDOW_W = value;
    } else if (!strcmp(setting, "window_h")) {
        WINDOW_H = value;
    } else if (!strcmp(setting, "cell_size")) {
        CELL_SIZE = value;
    } else if (!strcmp(setting, "fps")) {
        FPS = value;
    } else if (!strcmp(setting, "delay")) {
        DELAY = value;
    } else if (!strcmp(setting, "color_number")) {
        if (COLOR_NUMBER < 2)
            COLOR_NUMBER = 2;
        else if (COLOR_NUMBER > 9)
            COLOR_NUMBER = 9;
        else
            COLOR_NUMBER = value;
    } else if (!strcmp(setting, "background_r")) {
        BACKGROUND_R = value;
    } else if (!strcmp(setting, "background_g")) {
        BACKGROUND_G = value;
    } else if (!strcmp(setting, "background_b")) {
        BACKGROUND_B = value;
    } else if (!strcmp(setting, "grid_color")) {
        GRID_COLOR = value;
    } else {
        // For each color get the value, if there is no value set it to 0
        char color_check[]    = "color_0";
        char rotation_check[] = "rotation_0";

        for (int n = 0; n < MAX_COLOR_NUMBER; n++) {
            // We do this so we get "COLOR_1", "COLOR_2", etc.
            color_check[6]    = n + '0';
            rotation_check[9] = n + '0';

            // Value will be depending on the code colors in the readme
            if (!strcmp(setting, color_check))
                COLORS_ARRAY[n] = value;
            else if (!strcmp(setting, rotation_check))
                ROTATIONS_ARRAY[n] = value;
        }
    }

    return 0;
}

int read_config() {
    char line[CONFIG_BUFFER_SIZE], setting[CONFIG_BUFFER_SIZE];
    int value = 0;
    FILE* config_file;

    config_file = fopen("config.cfg", "r");

    if (!config_file)
        return 1;

    while (fgets(line, sizeof(line), config_file)) {
        sscanf(line, "%s = %d", setting, &value);

        // Ignore comments
        if (setting[0] != '#')
            parse_setting(setting, value);
    }

    fclose(config_file);
    return 0;
}

