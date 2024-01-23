
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "SDL.h"

/*----------------------------------------------------------------------------*/
/* Macros */

#define LENGTH(ARR) (sizeof(ARR) / sizeof((ARR)[0]))

#define COL_GRID_OVERLAY 0x222222
#define COL_ANT          0xFF0000

#define CELL_SZ  5 /* px */
#define MAX_ANTS 100

/* Frames per second when not holding space */
#define FPS 60

/* Default values if the parameter is not specified */
#define DEFAULT_W     100
#define DEFAULT_H     100
#define DEFAULT_DELAY 10

/* Comment to disable printing the total number of steps */
#define PRINT_STEPS

/*----------------------------------------------------------------------------*/
/* Types */

typedef enum {
    NORTH = 0,
    SOUTH = 1,
    WEST  = 2,
    EAST  = 3,
} EOrientation;

typedef enum {
    NONE  = 0, /* Don't turn */
    LEFT  = 1, /* 90º counter-clockwise */
    RIGHT = 2, /* 90º clockwise */
    BACK  = 3, /* 180º */
} ERotation;

typedef struct {
    uint32_t col;
    /* Before leaving this color, where should the ant turn? */
    ERotation rotation;
} color_t;

typedef struct {
    /* ctx.grid[] position */
    int x, y;
    EOrientation orientation;
} ant_t;

typedef struct {
    bool running;
    bool draw_grid_overlay;

    /* Delay in ms when holding space */
    int delay;

    /* Each item is an index of the colors[] array */
    int* grid;
    /* Independent of tile size */
    int grid_w, grid_h;

    ant_t* ants[MAX_ANTS];
    int ant_num;
} ctx_t;

/*----------------------------------------------------------------------------*/
/* Globals */

color_t colors[] = {
    /* clang-format off */
    { 0x000000, LEFT },
    { 0xEEEEEE, RIGHT },

    #ifdef BLUE_PALETTE
    { 0x000000, RIGHT },
    { 0x03045E, RIGHT },
    { 0x023E8A, LEFT },
    { 0x0077B6, LEFT },
    { 0x0096C7, LEFT },
    { 0x00B4D8, RIGHT },
    { 0x48CAE4, LEFT },
    { 0x90E0EF, RIGHT },
    { 0xADE8F4, LEFT },
    #endif
    /* clang-format on */
};

ctx_t ctx = {
    .running           = true,
    .draw_grid_overlay = false,
    .delay             = DEFAULT_DELAY,
    .grid              = NULL,
    .grid_w            = DEFAULT_W,
    .grid_h            = DEFAULT_H,
    .ant_num           = 0,
};

/*----------------------------------------------------------------------------*/
/* Static functions */

static void die(const char* fmt, ...) {
    va_list va;
    va_start(va, fmt);

    vfprintf(stderr, fmt, va);
    putc('\n', stderr);

    SDL_Quit();
    exit(1);
}

static bool parse_args(int argc, char** argv) {
    if (argc == 2) {
        fprintf(stderr, "You need to specify a width and a height.\n");
        return false;
    }

    if (argc >= 3) {
        ctx.grid_w = atoi(argv[1]);
        ctx.grid_h = atoi(argv[2]);

        if (ctx.grid_w < 1 || ctx.grid_h < 1) {
            fprintf(stderr, "Invalid grid size.\n");
            return false;
        }
    }

    if (argc >= 4) {
        ctx.delay = atoi(argv[3]);

        if (ctx.delay < 1) {
            fprintf(stderr, "Invalid delay.\n");
            return false;
        }
    }

    return true;
}

static inline const char* rotation2str(ERotation rotation) {
    /* clang-format off */
    switch (rotation) {
        default:
        case NONE:  return "None";
        case LEFT:  return "Counter-clockwise";
        case RIGHT: return "Clockwise";
        case BACK:  return "180º";
    }
    /* clang-format on */
}

static void grid_init(void) {
    ctx.grid = malloc(ctx.grid_w * ctx.grid_h * sizeof(uint32_t));

    /* Each cell contains an index to the colors[] array, start at 0 */
    for (int y = 0; y < ctx.grid_h; y++)
        for (int x = 0; x < ctx.grid_w; x++)
            ctx.grid[ctx.grid_w * y + x] = 0;
}

static inline void set_render_color(SDL_Renderer* rend, uint32_t col) {
    const uint8_t r = (col >> 16) & 0xFF;
    const uint8_t g = (col >> 8) & 0xFF;
    const uint8_t b = (col >> 0) & 0xFF;
    const uint8_t a = 255;
    SDL_SetRenderDrawColor(rend, r, g, b, a);
}

static void draw_grid_overlay(SDL_Renderer* rend) {
    set_render_color(rend, COL_GRID_OVERLAY);

    const int window_w = ctx.grid_w * CELL_SZ;
    const int window_h = ctx.grid_h * CELL_SZ;

    /* Vertical lines */
    for (int x = CELL_SZ; x < window_w; x += CELL_SZ)
        SDL_RenderDrawLine(rend, x, 0, x, window_h);

    /* Horizontal lines */
    for (int y = CELL_SZ; y < window_h; y += CELL_SZ)
        SDL_RenderDrawLine(rend, 0, y, window_w, y);
}

static void draw_ant(SDL_Renderer* rend, ant_t* ant) {
    set_render_color(rend, COL_ANT);

    const int margin = 1;

    int px_x = (ant->x * CELL_SZ) + margin;
    int px_y = (ant->y * CELL_SZ) + margin;
    int px_w = CELL_SZ - (margin * 2);
    int px_h = CELL_SZ - (margin * 2);

    /* Increase position 1px since the overlay will cover left and up borders */
    if (ctx.draw_grid_overlay) {
        px_x++;
        px_y++;
        px_w--;
        px_h--;
    }

    SDL_Rect rect = {
        .x = px_x,
        .y = px_y,
        .w = px_w,
        .h = px_h,
    };

    SDL_RenderFillRect(rend, &rect);
}

static EOrientation rotate(EOrientation orientation, ERotation rotation) {
    /* clang-format off */
    switch (rotation) {
        default:
        case NONE: return orientation;
        case LEFT:
            switch (orientation) {
                default:
                case NORTH: return WEST;
                case SOUTH: return EAST;
                case WEST:  return SOUTH;
                case EAST:  return NORTH;
            }
        case RIGHT:
            switch (orientation) {
                default:
                case NORTH: return EAST;
                case SOUTH: return WEST;
                case WEST:  return NORTH;
                case EAST:  return SOUTH;
            }
        case BACK:
            switch (orientation) {
                default:
                case NORTH: return SOUTH;
                case SOUTH: return NORTH;
                case WEST:  return EAST;
                case EAST:  return WEST;
            }
    }
    /* clang-format on */
}

static inline void ant_move_forward(ant_t* ant) {
    /* clang-format off */
    switch (ant->orientation) {
        case NORTH: ant->y--;   break;
        case SOUTH: ant->y++;   break;
        case WEST:  ant->x--;   break;
        case EAST:  ant->x++;   break;
    }
    /* clang-format on */

    /* Make sure we are not out of bounds */
    if (ant->x < 0)
        ant->x = ctx.grid_w - 1;
    if (ant->x >= ctx.grid_w)
        ant->x = 0;

    if (ant->y < 0)
        ant->y = ctx.grid_h - 1;
    if (ant->y >= ctx.grid_h)
        ant->y = 0;
}

static void ant_step(ant_t* ant) {
    uint32_t color_idx = ctx.grid[ctx.grid_w * ant->y + ant->x];
    color_t color      = colors[color_idx];

    /* Where are we supposed to turn before leaving the color? */
    ant->orientation = rotate(ant->orientation, color.rotation);

    /* Switch to the next color */
    ctx.grid[ctx.grid_w * ant->y + ant->x]++;

    /* If we reached the last color, go back to the first one */
    if (color_idx + 1 >= LENGTH(colors))
        ctx.grid[ctx.grid_w * ant->y + ant->x] = 0;

    /* Move forward depending on orientation */
    ant_move_forward(ant);

#ifdef PRINT_STEPS
    static uint32_t cur_step = 0;
    printf("\rStep: %d", cur_step++);
    fflush(stdout);
#endif
}

/*----------------------------------------------------------------------------*/
/* Main function */

int main(int argc, char** argv) {
    if (!parse_args(argc, argv))
        die("Usage: %s <width> <height> [delay]", argv[0]);

    printf("Grid size: %dx%d\n"
           "Cell size: %d px\n"
           "Delay: %d ms\n\n"
           "Color palette:\n",
           ctx.grid_w, ctx.grid_h, CELL_SZ, ctx.delay);

    for (uint32_t i = 0; i < LENGTH(colors); i++)
        printf("[%d] 0x%06X (%s)\n", i, colors[i].col,
               rotation2str(colors[i].rotation));

#ifdef PRINT_STEPS
    putchar('\n');
#endif

    /* Initialize grid in our global `ctx' structure */
    grid_init();

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0)
        die("Unable to start SDL.");

    const int window_w = ctx.grid_w * CELL_SZ;
    const int window_h = ctx.grid_h * CELL_SZ;
    SDL_Window* sdl_window =
      SDL_CreateWindow("Langton's ant", SDL_WINDOWPOS_CENTERED,
                       SDL_WINDOWPOS_CENTERED, window_w, window_h, 0);
    if (!sdl_window)
        die("Error creating SDL window.");

    /* Create renderer */
    SDL_Renderer* sdl_renderer =
      SDL_CreateRenderer(sdl_window, -1,
                         SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!sdl_renderer) {
        SDL_DestroyWindow(sdl_window);
        die("Error creating SDL renderer.");
    }

    /* Main loop */
    bool space_pressed = false;
    bool stepping      = false;

    while (ctx.running) {
        /* Reset every frame */
        stepping = false;

        /* Parse SDL events */
        SDL_Event sdl_event;
        while (SDL_PollEvent(&sdl_event)) {
            switch (sdl_event.type) {
                case SDL_QUIT:
                    ctx.running = false;
                    break;
                case SDL_KEYDOWN:
                    // Check the pressed key
                    switch (sdl_event.key.keysym.scancode) {
                        case SDL_SCANCODE_ESCAPE:
                        case SDL_SCANCODE_Q:
                            ctx.running = false;
                            break;
                        case SDL_SCANCODE_G:
                            ctx.draw_grid_overlay = !ctx.draw_grid_overlay;
                            break;
                        case SDL_SCANCODE_SPACE:
                            space_pressed = !space_pressed;
#ifdef PRINT_STEPS
                            if (!space_pressed)
                                putchar('\n');
#endif
                            break;
                        case SDL_SCANCODE_RIGHT:
                            stepping = true;
                            break;
                        default:
                            break;
                    }
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    switch (sdl_event.button.button) {
                        case SDL_BUTTON_LEFT:
                            int grid_x = sdl_event.motion.x / CELL_SZ;
                            int grid_y = sdl_event.motion.y / CELL_SZ;

                            /* Check if there is already an ant in that cell */
                            for (int i = 0; i < ctx.ant_num; i++) {
                                if (ctx.ants[i]->x == grid_x &&
                                    ctx.ants[i]->y == grid_y) {
                                    puts("There is an ant in that position, "
                                         "skipping.");
                                    break;
                                }
                            }

                            /* Create new ant where the user clicked */
                            ctx.ants[ctx.ant_num]    = malloc(sizeof(ant_t));
                            ctx.ants[ctx.ant_num]->x = grid_x;
                            ctx.ants[ctx.ant_num]->y = grid_y;

                            /* Default orientation is always north */
                            ctx.ants[ctx.ant_num]->orientation = NORTH;

                            ctx.ant_num++;
                            break;
                        default:
                            break;
                    }
                    break;
                default:
                    break;
            }
        }

        /* Clear window */
        set_render_color(sdl_renderer, 0x000000);
        SDL_RenderClear(sdl_renderer);

        /* Either we pressed space at some point or we are manually stepping */
        if (space_pressed || stepping)
            for (int i = 0; i < ctx.ant_num; i++)
                ant_step(ctx.ants[i]);

        /* Draw cells */
        for (int y = 0; y < ctx.grid_h; y++) {
            for (int x = 0; x < ctx.grid_w; x++) {
                uint32_t color_idx = ctx.grid[ctx.grid_w * y + x];
                set_render_color(sdl_renderer, colors[color_idx].col);

                SDL_Rect rect = {
                    .x = x * CELL_SZ,
                    .y = y * CELL_SZ,
                    .w = CELL_SZ,
                    .h = CELL_SZ,
                };

                SDL_RenderFillRect(sdl_renderer, &rect);
            }
        }

        /* Draw ants themselves */
        for (int i = 0; i < ctx.ant_num; i++)
            draw_ant(sdl_renderer, ctx.ants[i]);

        /* Draw grid overlay if enabled */
        if (ctx.draw_grid_overlay)
            draw_grid_overlay(sdl_renderer);

        /* Send to renderer */
        SDL_RenderPresent(sdl_renderer);

        if (space_pressed)
            SDL_Delay(ctx.delay);
        else
            SDL_Delay(1000 / FPS);
    }

    /* Free our stuff */
    free(ctx.grid);
    for (int i = 0; i < ctx.ant_num; i++)
        free(ctx.ants[i]);

    SDL_DestroyRenderer(sdl_renderer);
    SDL_DestroyWindow(sdl_window);
    SDL_Quit();

#ifdef PRINT_STEPS
    putchar('\n');
#endif

    return 0;
}
