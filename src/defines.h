// Defines

#define CONFIG_BUFFER_SIZE 255
#define MAX_COLOR_NUMBER   9

enum colors {
    BACKGROUND,
    BLACK,
    WHITE,
    RED,
    ORANGE,
    YELLOW,
    GREEN,
    BLUE,
    PURPLE,
    PINK
};

/* ----------------------------------------------------------------------- */
// Structs

typedef struct rgb_t {
    char r;
    char g;
    char b;
} rgb;

/* ----------------------------------------------------------------------- */
// Default settings

int WINDOW_W = 750, WINDOW_H = 750;    // Window size in pixels
int CELL_SIZE = 10;                    // Size of the squares. In pixels
int FPS = 60, DELAY = 50;    // FPS used by the program normally vs holding space

int COLOR_NUMBER = 2;
int GRID_COLOR   = 40;                                          // As RGB
int BACKGROUND_R = 20, BACKGROUND_G = 20, BACKGROUND_B = 22;    // As RGB
int COLORS_ARRAY[MAX_COLOR_NUMBER];    // Will store the colors defined by the user
int ROTATIONS_ARRAY[MAX_COLOR_NUMBER];    // Will store the rotation that the ant
                                          // should do when encountered X color
