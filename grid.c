#include "cetris.h"
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

int check_row_empty(int row_i, gridpoint (*internal_grid)[AREA_WIDTH]) {
    int empty = 0;
    for(int col_i = 0; col_i < AREA_WIDTH; col_i++) {
        if(internal_grid[row_i][col_i].occ) {
            empty = 1;
            break;
        }
    }
    
    return empty;
}

int set_row_empty(int row_i, gridpoint (*internal_grid)[AREA_WIDTH]) {
    for(int col_i = 0; col_i < AREA_WIDTH; col_i++) {
        internal_grid[row_i][col_i].occ = 0;
        internal_grid[row_i][col_i].color = 0;
    }
}


void make_empty_rows(gridpoint (*internal_grid)[AREA_WIDTH]) {
    int row_i = AREA_HEIGHT - 1;
    while(row_i >= 0) {
        if(check_row_empty(row_i, internal_grid)) {
            set_row_empty(row_i, internal_grid);
        }

        row_i--;
    }
}

// next: re-render grid
void flush_empty_rows(gridpoint (*internal_grid)[AREA_WIDTH]) {
    gridpoint new_grid[AREA_HEIGHT][AREA_WIDTH] = {0};
    int internal_row_i = AREA_HEIGHT - 1;
    int new_row_i = AREA_HEIGHT - 1;
    while(internal_row_i >= 0) {
        int is_empty = check_row_empty(internal_row_i, internal_grid);
        if(is_empty) { // skip this row index
          internal_row_i--;
          continue;
        }

        for(int col_i = 0; col_i < AREA_WIDTH; col_i++) {
            new_grid[new_row_i][col_i].occ = internal_grid[internal_row_i][col_i].occ;
            new_grid[new_row_i][col_i].color = internal_grid[internal_row_i][col_i].color;
        }

        new_row_i = --internal_row_i;
    }

    memcpy(internal_grid, new_grid, sizeof(new_grid));
}


