#include "rend3.h"

#include <math.h>
#include <stdbool.h>

Vec3 camera_dir_vec(Camera const *cam, float extra_angle) {
    float const a = cam->rotY + extra_angle;
    Vec3 v;
    v.x = sinf(a);
    v.y = 0.0f;
    v.z = -cosf(a);
    return v;
}

static Vec3 camera_transform(Camera const *cam, Vec3 world_vec) {
    // translate
    world_vec = vec3_add(world_vec, vec3_neg(cam->pos));

    // rotate
    Vec3 v = world_vec;
    float const cosY = cosf(-cam->rotY);
    float const sinY = sinf(-cam->rotY);
    v.x = cosY * world_vec.x - sinY * world_vec.z;
    v.z = cosY * world_vec.z + sinY * world_vec.x;
    // todo: z

    return v;
}

static bool project_vec3(Camera const *cam, Vec3 *v) {
    *v = camera_transform(cam, *v);
    if (-v->z < cam->znear) {
        return false;
    }
    *v = vec3_divs(vec3_muls(*v, cam->dist_to_screen), -v->z);
    v->y = -v->y;

    v->x /= cam->viewport_width;
    v->y /= cam->viewport_height;
    return true;
}

void rend3_init(Rend3 *ren, SDL_Renderer *r) {
    *ren = (Rend3) {0};
    SDL_Rect viewport = {0};
    SDL_RenderGetViewport(r, &viewport);

    ren->r = r;
    ren->cam = (Camera) {
        .pos = vec3(0.0f, 0.0f, 0.0f),
        .dist_to_screen = 730.0f,
        .znear = 0.15f,
        .viewport_width = viewport.w,
        .viewport_height = viewport.h,
    };
}

static void rend3_draw_triangle(Rend3 *rend, Vec3 a, Vec3 b, Vec3 c) {
    bool a_ok = project_vec3(&rend->cam, &a);
    bool b_ok = project_vec3(&rend->cam, &b);
    bool c_ok = project_vec3(&rend->cam, &c);

    float x1 = (a.x * 0.5f + 0.5f) * rend->cam.viewport_width;
    float y1 = (a.y * 0.5f + 0.5f) * rend->cam.viewport_height;
    float x2 = (b.x * 0.5f + 0.5f) * rend->cam.viewport_width;
    float y2 = (b.y * 0.5f + 0.5f) * rend->cam.viewport_height;
    float x3 = (c.x * 0.5f + 0.5f) * rend->cam.viewport_width;
    float y3 = (c.y * 0.5f + 0.5f) * rend->cam.viewport_height;

    if (a_ok && b_ok)
        SDL_RenderDrawLine(rend->r, x1, y1, x2, y2);
    if (b_ok && c_ok)
        SDL_RenderDrawLine(rend->r, x2, y2, x3, y3);
    if (c_ok && a_ok)
        SDL_RenderDrawLine(rend->r, x3, y3, x1, y1);
}

void rend3_draw_mesh(Rend3 *ren, Mesh const *mesh) {
    if (mesh->num_points < 3) {
        return;
    }

    for (size_t i = 0; i < mesh->num_points; i += 3) {
        Vec3 a = mesh->points[i + 0];
        Vec3 b = mesh->points[i + 1];
        Vec3 c = mesh->points[i + 2];
        rend3_draw_triangle(ren, a, b, c);
    }
}