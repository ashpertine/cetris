#include "cetris.h"
#include <ncurses.h>
#include <string.h>

gridpoint internal_grid[AREA_HEIGHT][AREA_WIDTH] = {0};

/*
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
*/

int check_internal_row_full(int row_i, gridpoint (*internal_grid)[AREA_WIDTH]) {
  int full = 1;
  for (int col_i = 0; col_i < AREA_WIDTH; col_i++) {
    if (!internal_grid[row_i][col_i].occ) {
      full = 0;
      break;
    }
  }

  return full;
}

int check_internal_row_empty(int row_i,
                             gridpoint (*internal_grid)[AREA_WIDTH]) {
  int empty = 1;
  for (int col_i = 0; col_i < AREA_WIDTH; col_i++) {
    if (internal_grid[row_i][col_i].occ) {
      empty = 0;
      break;
    }
  }

  return empty;
}

void set_internal_row_empty(int row_i, gridpoint (*internal_grid)[AREA_WIDTH]) {
  for (int col_i = 0; col_i < AREA_WIDTH; col_i++) {
    internal_grid[row_i][col_i].occ = 0;
    internal_grid[row_i][col_i].color = 0;
  }
}

int clear_internal_empty_rows(gridpoint (*internal_grid)[AREA_WIDTH]) {
  int row_i = AREA_HEIGHT - 1;
  int has_changed = 0;
  while (row_i >= 0) {
    if (check_internal_row_full(row_i, internal_grid)) {
      has_changed = 1;
      set_internal_row_empty(row_i, internal_grid);
    }

    row_i--;
  }

  return has_changed;
}

void flush_internal_empty_rows(gridpoint (*internal_grid)[AREA_WIDTH]) {
  gridpoint new_grid[AREA_HEIGHT][AREA_WIDTH] = {0};
  int internal_row_i = AREA_HEIGHT - 1;
  int new_row_i = AREA_HEIGHT - 1;
  while (internal_row_i >= 0) {
    int is_empty = check_internal_row_empty(internal_row_i, internal_grid);
    if (is_empty) { // skip this row index
      internal_row_i--;
      continue;
    }

    for (int col_i = 0; col_i < AREA_WIDTH; col_i++) {
      new_grid[new_row_i][col_i].occ = internal_grid[internal_row_i][col_i].occ;
      new_grid[new_row_i][col_i].color =
          internal_grid[internal_row_i][col_i].color;
    }
    internal_row_i--;
    new_row_i--;
  }

  memcpy(internal_grid, new_grid, sizeof(new_grid));
}
