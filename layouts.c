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

layout_set get_ttlayout(Tetromino tt) {
  switch (tt) {
  case STRAIGHT:
    return STRAIGHT_LAYOUT;
  case SQUARE:
    return SQUARE_LAYOUT;
  case TSHAPE:
    return TSHAPE_LAYOUT;
  case SKEW:
    return SKEW_LAYOUT;
  case INVERSESKEW:
    return INVERSESKEW_LAYOUT;
  case LSHAPE:
    return LSHAPE_LAYOUT;
  case INVERSELSHAPE:
    return INVERSELSHAPE_LAYOUT;
  default:
    return STRAIGHT_LAYOUT;
    break;
  }
}

ttpt *get_wall_kick(Orientation new_rotate_ori, ttshape_state *shape_state) {
  Orientation curr_ori = shape_state->ori;
  int is_I = shape_state->lo_set.name == STRAIGHT;
  if (new_rotate_ori == RIGHT) {

    if (curr_ori == SPAWN) {
      return is_I ? WALL_KICK_I_0_R : WALL_KICK_0_R;
    } else if (curr_ori == TWO) {
      return is_I ? WALL_KICK_I_2_R : WALL_KICK_2_R;
    }
  } else if (new_rotate_ori == LEFT) {

    if (curr_ori == SPAWN) {
      return is_I ? WALL_KICK_I_0_L : WALL_KICK_0_L;
    } else if (curr_ori == TWO) {
      return is_I ? WALL_KICK_I_2_L : WALL_KICK_2_L;
    }
  } else if (new_rotate_ori == TWO) {

    if (curr_ori == RIGHT) {
      return is_I ? WALL_KICK_I_R_2 : WALL_KICK_R_2;
    } else if (curr_ori == LEFT) {
      return is_I ? WALL_KICK_I_L_2 : WALL_KICK_L_2;
    }
  } else { // spawn

    if (curr_ori == RIGHT) {
      return is_I ? WALL_KICK_I_R_0 : WALL_KICK_R_0;
    } else if (curr_ori == LEFT) {
      return is_I ? WALL_KICK_I_L_0 : WALL_KICK_L_0;
    }
  }

  return WALL_KICK_FALLBACK;
}
