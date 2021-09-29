#pragma once

#include <stdbool.h>
#include <SDL2/SDL.h>

#include "rend3.h"

typedef struct App {
    Rend3 rend;
    Mesh test_mesh;
    SDL_Window *window;
    SDL_Renderer *renderer;
    bool running;
} App;

bool app_init(App *app);
void app_destroy(App *app);
void app_run(App *app);