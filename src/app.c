#include "app.h"

static void app_tick(App *app);

static void randomize_mesh(App *app) {
    for (size_t i = 0; i < app->test_mesh.num_points; i += 3) {
        float ox = (rand() / (float)RAND_MAX * 2.0f - 1.0f) * 3.0f;
        float oy = (rand() / (float)RAND_MAX * 2.0f - 1.0f) * 1.0f;
        float oz = rand() / (float)RAND_MAX * -10.0f;
        float s = rand() / (float)RAND_MAX * 2.0f;

        for (size_t k = 0; k < 3; ++k) {
            float x = (rand() / (float)RAND_MAX * 2.0f - 1.0f) * s;
            float y = (rand() / (float)RAND_MAX * 2.0f - 1.0f) * s;
            float z = (rand() / (float)RAND_MAX * 2.0f - 1.0f) * s;
            app->test_mesh.points[i + k] = vec3(ox + x, oy + y, oz + z);
        }
    }
}

bool app_init(App *app) {
    *app = (App) {0};

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        puts(SDL_GetError());
        return false;
    }

    if (SDL_CreateWindowAndRenderer(
        640,
        480,
        0,
        &app->window,
        &app->renderer
    ) != 0) {
        puts(SDL_GetError());
        return false;
    }

    rend3_init(&app->rend, app->renderer);
    app->rend.cam.pos.z = 2.0f;
    
    // app->test_mesh.num_points = 6;
    // app->test_mesh.points = calloc(6, sizeof(Vec3));
    // app->test_mesh.points[0] = vec3(-1.0f, -1.0f, 0.0f);
    // app->test_mesh.points[1] = vec3(0.0f, 1.0f, 0.0f);
    // app->test_mesh.points[2] = vec3(1.0f, -1.0f, 0.0f);
    // app->test_mesh.points[3] = vec3(1.0f, -1.0f, 0.0f);
    // app->test_mesh.points[4] = vec3(1.0f, 1.0f, -1.0f);
    // app->test_mesh.points[5] = vec3(1.0f, -1.0f, -2.0f);

    app->test_mesh.num_points = 60;
    app->test_mesh.points = calloc(app->test_mesh.num_points, sizeof(Vec3));
    randomize_mesh(app);

    app->running = true;

    return true;
}   

void app_destroy(App *app) {
    free(app->test_mesh.points);

    if (app->renderer) {
        SDL_DestroyRenderer(app->renderer);
    }
    if (app->window) {
        SDL_DestroyWindow(app->window);
    }
    SDL_Quit();
}

void app_run(App *app) {
    SDL_Event e;
    for (;;) {
        while (SDL_PollEvent(&e)) {
            switch (e.type) {
            case SDL_QUIT:
                app->running = false;
                break;
            case SDL_KEYUP:
                if (e.key.keysym.sym == SDLK_ESCAPE) {
                    app->running = false;
                } else if (e.key.keysym.sym == SDLK_RETURN) {
                    randomize_mesh(app);
                }
                break;
            }
        }

        Uint8 const *keys = SDL_GetKeyboardState(NULL);
        if (keys[SDL_SCANCODE_W]) camera_move(&app->rend.cam, 0.0f, 0.01f);
        if (keys[SDL_SCANCODE_S]) camera_move(&app->rend.cam, M_PI, 0.01f);
        if (keys[SDL_SCANCODE_A]) camera_move(&app->rend.cam, M_PI / -2.0f, 0.01f);
        if (keys[SDL_SCANCODE_D]) camera_move(&app->rend.cam, M_PI / 2.0f, 0.01f);
        if (keys[SDL_SCANCODE_LEFT]) app->rend.cam.rotY -= 0.01f;
        if (keys[SDL_SCANCODE_RIGHT]) app->rend.cam.rotY += 0.01f;

        if (!app->running) {
            break;
        }

        app_tick(app);
        SDL_Delay(16);
    }
}

static void app_tick(App *app) {
    SDL_SetRenderDrawColor(app->renderer, 0, 0, 0, 255);
    SDL_RenderClear(app->renderer);
    SDL_SetRenderDrawColor(app->renderer, 255, 0, 0, 255);
    rend3_draw_mesh(&app->rend, &app->test_mesh);
    SDL_RenderPresent(app->renderer);
}