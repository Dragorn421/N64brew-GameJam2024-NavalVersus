// SPDX-FileCopyrightText: 2024 Dragorn421
// SPDX-License-Identifier: CC0-1.0

#include <libdragon.h>
#include "../../core.h"
#include "../../minigame.h"
#include <t3d/t3d.h>
#include <t3d/t3dmath.h>
#include <t3d/t3dmodel.h>
#include <t3d/t3dskeleton.h>
#include <t3d/t3danim.h>
#include <t3d/t3ddebug.h>

const MinigameDef minigame_def = {
    .gamename = "Naval Versus",
    .developername = "Dragorn421",
    .description = "2v2",
    .instructions = "TBD",
};

surface_t *zbuffer;
T3DViewport top_viewport, bottom_viewport, full_viewport;
T3DModel *suzanne_model;

void minigame_init()
{
    display_init(RESOLUTION_640x480, DEPTH_16_BPP, 3, GAMMA_NONE, FILTERS_RESAMPLE_ANTIALIAS_DEDITHER);
    zbuffer = display_get_zbuf();

    t3d_init((T3DInitParams){});

    top_viewport = t3d_viewport_create();
    bottom_viewport = t3d_viewport_create();
    top_viewport.size[1] = bottom_viewport.size[1] = display_get_height() / 2;
    bottom_viewport.offset[1] = top_viewport.size[1];

    full_viewport = t3d_viewport_create();

    suzanne_model = t3d_model_load("rom:/navalversus/suzanne.t3dm");
}

void minigame_fixedloop(float deltatime)
{
}

void minigame_loop(float deltatime)
{
    rdpq_attach(display_get(), zbuffer);
    t3d_frame_start();

    t3d_viewport_set_projection(&full_viewport, T3D_DEG_TO_RAD(90.0f), 1.0f, 40.0f);
    t3d_viewport_look_at(&full_viewport, &(T3DVec3){{0, 10, 0}}, &(T3DVec3){{0, 0, 0}}, &(T3DVec3){{0, 0, 1}});
    t3d_viewport_attach(&full_viewport);

    t3d_screen_clear_color(RGBA32(50, 100, 50, 0xFF));
    t3d_screen_clear_depth();

    // ???
    uint8_t colorAmbient[4] = {80, 80, 100, 0xFF};
    uint8_t colorDir[4] = {0xEE, 0xAA, 0xAA, 0xFF};
    T3DVec3 lightDirVec = {{-1.0f, 1.0f, 1.0f}};
    t3d_vec3_norm(&lightDirVec);
    t3d_light_set_ambient(colorAmbient);
    t3d_light_set_directional(0, colorDir, &lightDirVec);
    t3d_light_set_count(1);
    T3DMat4FP mtx;
    t3d_mat4fp_identity(&mtx);
    t3d_matrix_push(&mtx);
    rdpq_set_prim_color(RGBA32(255, 255, 255, 255));
    //

    t3d_model_draw(suzanne_model);

    t3d_matrix_pop(1);

    rdpq_detach_show();
}

void minigame_cleanup()
{
    display_close();
    t3d_model_free(suzanne_model);
    t3d_destroy();
}