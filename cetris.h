#pragma once

#define COLOR_PINK 8
#define BLOCK_COUNT 4
#define MAX_ORIENTATIONS 4
#define NUM_ROT_ATTEMPTS 5
#define AREA_HEIGHT 18
#define AREA_WIDTH 10
#define NUM_TETROMINOS 7

typedef enum {
  STRAIGHT,
  SQUARE,
  TSHAPE,
  SKEW,        // also known as Z
  INVERSESKEW, // also known as S
  LSHAPE,
  INVERSELSHAPE
} Tetromino;

typedef enum {
  COLOR_PAIR_STRAIGHT = 1,
  COLOR_PAIR_SQUARE,
  COLOR_PAIR_TSHAPE,
  COLOR_PAIR_SKEW,
  COLOR_PAIR_INVERSESKEW,
  COLOR_PAIR_LSHAPE,
  COLOR_PAIR_INVERSELSHAPE,
  COLOR_RESET
} TetrominoColorPair;

typedef enum {
  SPAWN,
  RIGHT,
  TWO,
  LEFT,
} Orientation;

typedef struct TtPoint {
  int row;
  int col;
} ttpt;

typedef struct LayoutSet {
  Tetromino name;
  ttpt layouts[MAX_ORIENTATIONS][BLOCK_COUNT];
  int orientations;
} layout_set;

typedef struct TtShapeState {
  layout_set lo_set;
  Orientation ori;
  int x;
  int y;
  int longest_x;
  int lowest_y;
  int must_change; // 0 if the current shape can continue to be used, 1 if it
                   // needs to be changed for a new shape in the next iteration
} ttshape_state;

extern int internal_grid[AREA_HEIGHT][AREA_WIDTH];
