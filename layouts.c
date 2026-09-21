#include "cetris.h"

// Layout sets. Do not change the order.
// each layout set starts from the spawn position and subsequent elements are
// clockwise rotations from the previous element.
layout_set STRAIGHT_LAYOUT = {.name = STRAIGHT,
                              .layouts =
                                  {
                                      {{0, 0}, {0, 1}, {0, 2}, {0, 3}},
                                      {{0, 0}, {1, 0}, {2, 0}, {3, 0}},
                                      {{0, 0}, {0, 1}, {0, 2}, {0, 3}},
                                      {{0, 0}, {1, 0}, {2, 0}, {3, 0}},
                                  },
                              .orientations = 4};

layout_set SQUARE_LAYOUT = {.name = SQUARE,
                            .layouts = {{{0, 0}, {1, 0}, {0, 1}, {1, 1}}},
                            .orientations = 1};

layout_set TSHAPE_LAYOUT = {.name = TSHAPE,
                            .layouts =
                                {
                                    {{1, 0}, {1, 1}, {1, 2}, {0, 1}},
                                    {{0, 0}, {1, 0}, {2, 0}, {1, 1}},
                                    {{0, 0}, {0, 1}, {0, 2}, {1, 1}},
                                    {{0, 1}, {1, 1}, {2, 1}, {1, 0}},
                                },
                            .orientations = 4};

layout_set SKEW_LAYOUT = { // also known as Z
    .name = SKEW,
    .layouts =
        {
            {{0, 0}, {0, 1}, {1, 1}, {1, 2}},
            {{0, 1}, {1, 1}, {1, 0}, {2, 0}},
            {{0, 0}, {0, 1}, {1, 1}, {1, 2}},
            {{0, 1}, {1, 1}, {1, 0}, {2, 0}},
        },
    .orientations = 4};

layout_set INVERSESKEW_LAYOUT = { // also known as S
    .name = INVERSESKEW,
    .layouts =
        {
            {{1, 0}, {1, 1}, {0, 1}, {0, 2}},
            {{0, 0}, {1, 0}, {1, 1}, {2, 1}},
            {{1, 0}, {1, 1}, {0, 1}, {0, 2}},
            {{0, 0}, {1, 0}, {1, 1}, {2, 1}},
        },
    .orientations = 4};

layout_set LSHAPE_LAYOUT = {.name = LSHAPE,
                            .layouts =
                                {
                                    {{0, 2}, {1, 0}, {1, 1}, {1, 2}},
                                    {{0, 0}, {1, 0}, {2, 0}, {2, 1}},
                                    {{0, 0}, {0, 1}, {0, 2}, {1, 0}},
                                    {{0, 0}, {0, 1}, {1, 1}, {2, 1}},
                                },
                            .orientations = 4};

layout_set INVERSELSHAPE_LAYOUT = {.name = INVERSELSHAPE,
                                   .layouts =
                                       {
                                           {{0, 0}, {1, 0}, {1, 1}, {1, 2}},
                                           {{0, 0}, {0, 1}, {1, 0}, {2, 0}},
                                           {{0, 0}, {0, 1}, {0, 2}, {1, 2}},
                                           {{0, 1}, {1, 1}, {2, 0}, {2, 1}},
                                       },
                                   .orientations = 4};

// Wall kicks for normal pieces - spawn to clockwise rotation
ttpt WALL_KICK_0_R[NUM_ROT_ATTEMPTS] = {
    {0, 0}, {0, -1}, {-1, -1}, {+2, 0}, {+2, -1}};

ttpt WALL_KICK_R_0[NUM_ROT_ATTEMPTS] = {
    {0, 0}, {0, +1}, {+1, +1}, {-2, 0}, {-2, +1}};

ttpt WALL_KICK_R_2[NUM_ROT_ATTEMPTS] = {
    {0, 0}, {0, -1}, {0, +2}, {-2, -1}, {+1, +2}};

ttpt WALL_KICK_2_R[NUM_ROT_ATTEMPTS] = {
    {0, 0}, {0, +1}, {0, -2}, {+2, +1}, {-1, -2}};

ttpt WALL_KICK_2_L[NUM_ROT_ATTEMPTS] = {
    {0, 0}, {0, +1}, {-1, +1}, {+2, 0}, {+2, +1}};

ttpt WALL_KICK_L_2[NUM_ROT_ATTEMPTS] = {
    {0, 0}, {0, -1}, {+1, -1}, {-2, 0}, {-2, -1}};

ttpt WALL_KICK_L_0[NUM_ROT_ATTEMPTS] = {
    {0, 0}, {0, -1}, {+1, -1}, {-2, 0}, {-2, -1}};

ttpt WALL_KICK_0_L[NUM_ROT_ATTEMPTS] = {
    {0, 0}, {0, +1}, {-1, +1}, {+2, 0}, {+2, +1}};

// Wall kicks for I piece.
ttpt WALL_KICK_I_0_R[NUM_ROT_ATTEMPTS] = {
    {0, 0}, {0, -2}, {0, +1}, {+1, -2}, {-2, +1}};

ttpt WALL_KICK_I_R_0[NUM_ROT_ATTEMPTS] = {
    {0, 0}, {0, +2}, {0, -1}, {-1, +2}, {+2, -1}};

ttpt WALL_KICK_I_R_2[NUM_ROT_ATTEMPTS] = {
    {0, 0}, {0, -1}, {0, +2}, {-2, -1}, {+1, +2}};

ttpt WALL_KICK_I_2_R[NUM_ROT_ATTEMPTS] = {
    {0, 0}, {0, +1}, {0, -2}, {+2, +1}, {-1, -2}};

ttpt WALL_KICK_I_2_L[NUM_ROT_ATTEMPTS] = {
    {0, 0}, {0, +2}, {0, -1}, {-1, +2}, {+2, -1}};

ttpt WALL_KICK_I_L_2[NUM_ROT_ATTEMPTS] = {
    {0, 0}, {0, -2}, {0, +1}, {+1, -2}, {-2, +1}};

ttpt WALL_KICK_I_L_0[NUM_ROT_ATTEMPTS] = {
    {0, 0}, {0, +1}, {0, -2}, {-2, +1}, {+1, -2}};

ttpt WALL_KICK_I_0_L[NUM_ROT_ATTEMPTS] = {
    {0, 0}, {0, -1}, {0, +2}, {+2, -1}, {-1, +2}};

ttpt WALL_KICK_FALLBACK[NUM_ROT_ATTEMPTS] = {
    {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}};
