#pragma once

#include <stddef.h>
#include <SDL2/SDL.h>

typedef struct Vec3 {
    float x, y, z;
} Vec3;

static inline Vec3 vec3(float x, float y, float z) {
    return (Vec3) {
        .x = x,
        .y = y,
        .z = z,
    };
}

static inline Vec3 vec3_add(Vec3 lhs, Vec3 rhs) {
    return vec3(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z);
}

static inline Vec3 vec3_neg(Vec3 v) {
    return vec3(-v.x, -v.y, -v.z);
}

static inline Vec3 vec3_muls(Vec3 v, float s) {
    return vec3(v.x * s, v.y * s, v.z * s);
}

static inline Vec3 vec3_divs(Vec3 v, float s) {
    return vec3(v.x / s, v.y / s, v.z / s);
}

static inline float vec3_magn2(Vec3 v) {
    return v.x*v.x + v.y*v.y + v.z*v.z;
}

typedef struct Camera {
    Vec3 pos;
    float rotX, rotY;
    float dist_to_screen;
    float znear;
    float viewport_width, viewport_height;
} Camera;

Vec3 camera_dir_vec(Camera const *cam, float extra_angle);
static inline void camera_move(Camera *cam, float extra_angle, float distance) {
    cam->pos = vec3_add(cam->pos, vec3_muls(camera_dir_vec(cam, extra_angle), distance));
}

typedef struct Rend3 {
    Camera cam;
    SDL_Renderer *r;
} Rend3;

typedef struct Mesh {
    Vec3 *points;
    size_t num_points;
} Mesh;

void rend3_init(Rend3 *ren, SDL_Renderer *r);
void rend3_draw_mesh(Rend3 *ren, Mesh const *mesh);