#pragma once
#include "cetris.h"

extern gridpoint internal_grid[AREA_HEIGHT][AREA_WIDTH];

int check_row_empty(int row_i, gridpoint (*internal_grid)[AREA_WIDTH]);

int set_row_empty(int row_i, gridpoint (*internal_grid)[AREA_WIDTH]);

void make_empty_rows(gridpoint (*internal_grid)[AREA_WIDTH]);

void flush_empty_rows(gridpoint (*internal_grid)[AREA_WIDTH]);