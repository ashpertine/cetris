#include "cetris.h"

TetrominoColorPair get_color_pair(Tetromino tt) {
  switch (tt) {
  case STRAIGHT:
    return COLOR_PAIR_STRAIGHT;
  case SQUARE:
    return COLOR_PAIR_SQUARE;
  case TSHAPE:
    return COLOR_PAIR_TSHAPE;
  case SKEW:
    return COLOR_PAIR_SKEW;
  case INVERSESKEW:
    return COLOR_PAIR_INVERSESKEW;
  case LSHAPE:
    return COLOR_PAIR_LSHAPE;
  case INVERSELSHAPE:
    return COLOR_PAIR_INVERSELSHAPE;
  default:
    return 0;
    break;
  }
}