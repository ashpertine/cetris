#pragma once
#include "cetris.h"

extern layout_set STRAIGHT_LAYOUT;
extern layout_set SQUARE_LAYOUT;
extern layout_set TSHAPE_LAYOUT;
extern layout_set SKEW_LAYOUT;        // also known as a Z
extern layout_set INVERSESKEW_LAYOUT; // also known as an S
extern layout_set LSHAPE_LAYOUT;
extern layout_set INVERSELSHAPE_LAYOUT;

// Wall kicks for normal pieces - spawn to clockwise rotation
extern ttpt WALL_KICK_0_R[NUM_ROT_ATTEMPTS];
extern ttpt WALL_KICK_R_0[NUM_ROT_ATTEMPTS];
extern ttpt WALL_KICK_R_2[NUM_ROT_ATTEMPTS];
extern ttpt WALL_KICK_2_R[NUM_ROT_ATTEMPTS];
extern ttpt WALL_KICK_2_L[NUM_ROT_ATTEMPTS];
extern ttpt WALL_KICK_L_2[NUM_ROT_ATTEMPTS];
extern ttpt WALL_KICK_L_0[NUM_ROT_ATTEMPTS];
extern ttpt WALL_KICK_0_L[NUM_ROT_ATTEMPTS];

// Wall kicks for I piece.
extern ttpt WALL_KICK_I_0_R[NUM_ROT_ATTEMPTS];
extern ttpt WALL_KICK_I_R_0[NUM_ROT_ATTEMPTS];
extern ttpt WALL_KICK_I_R_2[NUM_ROT_ATTEMPTS];
extern ttpt WALL_KICK_I_2_R[NUM_ROT_ATTEMPTS];
extern ttpt WALL_KICK_I_2_L[NUM_ROT_ATTEMPTS];
extern ttpt WALL_KICK_I_L_2[NUM_ROT_ATTEMPTS];
extern ttpt WALL_KICK_I_L_0[NUM_ROT_ATTEMPTS];
extern ttpt WALL_KICK_I_0_L[NUM_ROT_ATTEMPTS];
extern ttpt WALL_KICK_FALLBACK[NUM_ROT_ATTEMPTS];
