
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "SDL.h"

/*----------------------------------------------------------------------------*/
/* Macros */

#define COL_BACKGROUND 0x050505
#define COL_GRID       0x222222
#define COL_ANT        0xFFFFFF

#define CELL_SZ  9 /* px */
#define MAX_ANTS 100

/* Frames per second when not holding space */
#define FPS 60

/* Delay in ms. when holding space */
#define DELAY 100

/*----------------------------------------------------------------------------*/
/* Types */

typedef enum {
    UP    = 0,
    DOWN  = 1,
    LEFT  = 2,
    RIGHT = 3,
} ERotation;

typedef struct {
    /* ctx.grid[] position */
    int x, y;
    ERotation rotation;
    int color_idx;
} ant_t;

typedef struct {
    bool running;
    bool draw_grid;

    /* Independent of tile size */
    uint32_t* grid;
    int grid_w, grid_h;

    ant_t* ants[MAX_ANTS];
    int ant_num;
} ctx_t;

/*----------------------------------------------------------------------------*/
/* Globals */

ctx_t ctx = {
    .running   = true,
    .draw_grid = false,

    .grid   = NULL,
    .grid_w = 0,
    .grid_h = 0,

    .ant_num = 0,
};

/*----------------------------------------------------------------------------*/
/* Functions */

static void die(const char* fmt, ...) {
    va_list va;
    va_start(va, fmt);

    vfprintf(stderr, fmt, va);
    putc('\n', stderr);

    SDL_Quit();
    exit(1);
}

static bool parse_args(int argc, char** argv) {
    if (argc != 3) {
        fprintf(stderr, "Wrong number of arguments.\n");
        return false;
    }

    ctx.grid_w = atoi(argv[1]);
    ctx.grid_h = atoi(argv[2]);

    if (ctx.grid_w < 1 || ctx.grid_h < 1) {
        fprintf(stderr, "Invalid grid size.\n");
        return false;
    }

    return true;
}

static void grid_init(void) {
    ctx.grid = malloc(ctx.grid_w * ctx.grid_h * sizeof(uint32_t));

    for (int y = 0; y < ctx.grid_h; y++)
        for (int x = 0; x < ctx.grid_w; x++)
            ctx.grid[ctx.grid_w * y + x] = COL_BACKGROUND;
}

static inline void set_render_color(SDL_Renderer* rend, uint32_t col) {
    const uint8_t r = (col >> 16) & 0xFF;
    const uint8_t g = (col >> 8) & 0xFF;
    const uint8_t b = (col >> 0) & 0xFF;
    const uint8_t a = 255;
    SDL_SetRenderDrawColor(rend, r, g, b, a);
}

static void draw_grid(SDL_Renderer* rend) {
    set_render_color(rend, COL_GRID);

    const int window_w = ctx.grid_w * CELL_SZ;
    const int window_h = ctx.grid_h * CELL_SZ;

    for (int x = CELL_SZ; x < window_w; x += CELL_SZ)
        SDL_RenderDrawLine(rend, x, 0, x, window_h);

    for (int y = CELL_SZ; y < window_h; y += CELL_SZ)
        SDL_RenderDrawLine(rend, 0, y, window_w, y);
}

static void draw_ant(SDL_Renderer* rend, ant_t* cur_ant) {
    set_render_color(rend, COL_ANT);

    const int margin = 1;
    const int height = 3;
    const int width  = CELL_SZ - (margin * 2);

    SDL_Rect rect;
    switch (cur_ant->rotation) {
        default:
        case UP:
            rect.x = (cur_ant->x * CELL_SZ) + margin;
            rect.y = (cur_ant->y * CELL_SZ) + margin;
            rect.w = width;
            rect.h = height;
            break;
        case DOWN:
            rect.x = (cur_ant->x * CELL_SZ) + margin;
            rect.y = (cur_ant->y * CELL_SZ) + CELL_SZ - margin - height;
            rect.w = width;
            rect.h = height;
            break;
        case LEFT:
            rect.x = (cur_ant->x * CELL_SZ) + margin;
            rect.y = (cur_ant->y * CELL_SZ) + margin;
            rect.w = height;
            rect.h = width;
            break;
        case RIGHT:
            rect.x = (cur_ant->x * CELL_SZ) + CELL_SZ - margin - width;
            rect.y = (cur_ant->y * CELL_SZ) + margin;
            rect.w = height;
            rect.h = width;
            break;
    }

    SDL_RenderFillRect(rend, &rect);
}

int main(int argc, char** argv) {
    if (!parse_args(argc, argv))
        die("Usage: %s <width> <height>\n", argv[0]);

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

    SDL_Event sdl_event;

    // DELME
    ctx.ants[ctx.ant_num]            = malloc(sizeof(ant_t));
    ctx.ants[ctx.ant_num]->x         = 5;
    ctx.ants[ctx.ant_num]->y         = 5;
    ctx.ants[ctx.ant_num]->rotation  = UP;
    ctx.ants[ctx.ant_num]->color_idx = 0;
    ctx.ant_num++;

    while (ctx.running) {
        /* Reset every frame */
        stepping = false;

        /* Parse SDL events */
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
                            ctx.draw_grid = !ctx.draw_grid;
                            break;
                        case SDL_SCANCODE_SPACE:
                            space_pressed = !space_pressed;
                            break;
                        case SDL_SCANCODE_RIGHT:
                            stepping = true;
                            break;
                        default:
                            break;
                    }
                    break;
                /* TODO: Add ants when clicking */
                default:
                    break;
            }
        }

        /* Clear window */
        set_render_color(sdl_renderer, COL_BACKGROUND);
        SDL_RenderClear(sdl_renderer);

        /* Either we pressed space at some point or we are manually stepping */
        if (space_pressed || stepping) {
            /* TODO: Move ants */
        }

        /* Draw cells */
        for (int y = 0; y < ctx.grid_h; y++) {
            for (int x = 0; x < ctx.grid_w; x++) {
                set_render_color(sdl_renderer, ctx.grid[ctx.grid_w * y + x]);

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

        /* Draw grid if active */
        if (ctx.draw_grid)
            draw_grid(sdl_renderer);

        /* Send to renderer */
        SDL_RenderPresent(sdl_renderer);

        if (space_pressed)
            SDL_Delay(DELAY);
        else
            SDL_Delay(1000 / FPS);
    }

    SDL_DestroyRenderer(sdl_renderer);
    SDL_DestroyWindow(sdl_window);
    SDL_Quit();

    return 0;
}
