#pragma once
#include "cetris.h"

extern gridpoint internal_grid[AREA_HEIGHT][AREA_WIDTH];

int check_internal_row_full(int row_i, gridpoint (*internal_grid)[AREA_WIDTH]);

int set_internal_row_empty(int row_i, gridpoint (*internal_grid)[AREA_WIDTH]);

int clear_internal_empty_rows(gridpoint (*internal_grid)[AREA_WIDTH]);

void flush_internal_empty_rows(gridpoint (*internal_grid)[AREA_WIDTH]);
