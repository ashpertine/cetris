#include "cetris.h"
#include "layouts.h"
#include "grid.h"
#include <locale.h>
#include <ncurses.h>
#include <panel.h>
#include <stdlib.h>
#include <time.h>

WINDOW *create_newwin(int height, int width, int starty, int startx);

layout_set get_rand_lo_set(void) {
  Tetromino new = (Tetromino)(rand() % NUM_TETROMINOS);
  return get_ttlayout(new);
}

void wprint_tetromino(WINDOW *local_win, ttshape_state *shape_state, int del) {
  int i = 0;
  int col = 0;
  int col_ui = 0; // ui representation of column; column on the ui has 1 extra
                  // space than internal_grid
  int row = 0;
  int longest_col = 0;
  int lowest_row = 0;

  ttpt *layout = shape_state->lo_set.layouts[shape_state->ori];
  int color_pair = get_color_pair(shape_state->lo_set.name);
  int pair_to_use = del ? 0 : color_pair;
  wattrset(local_win, COLOR_PAIR(pair_to_use));

  // +1 ONLY FOR UI to compensate for the extra space due to the borders,
  // internal grid starts at 0
  for (i = 0; i < BLOCK_COUNT; i++) {
    col_ui = layout[i].col * 2 + (shape_state->x * 2) + 1;
    col = layout[i].col + shape_state->x;
    if (col > longest_col)
      longest_col = col;
    row = layout[i].row + shape_state->y + 1;
    if (row > lowest_row) {
      lowest_row = row - 1; // get rid of the border
    }
    mvwprintw(local_win, row, col_ui, "  ");
  }

  shape_state->longest_x = longest_col;
  shape_state->lowest_y = lowest_row;
  wattrset(local_win, A_NORMAL);
  wrefresh(local_win);
}

void wprint_horiz_shift_tetromino(WINDOW *local_win, ttshape_state *shape_state,
                                  int is_left) {
  int fact = is_left ? -1 : 1;
  int should_mv = 1;
  if ((!is_left && shape_state->longest_x + fact >= AREA_WIDTH) ||
      (is_left && shape_state->x + fact < 0))
    return;

  ttpt *current_layout = shape_state->lo_set.layouts[shape_state->ori];
  int x_after = shape_state->x + fact;
  for (int i = 0; i < BLOCK_COUNT; i++) {
    if (internal_grid[shape_state->y + current_layout[i].row]
                     [x_after + current_layout[i].col].occ) {
      should_mv = 0;
    }
  }

  if (should_mv) {
    // delete by resetting colors to default
    wprint_tetromino(local_win, shape_state, 1);
    shape_state->x = shape_state->x + fact;
    // set new position and print
    wprint_tetromino(local_win, shape_state, 0);
  }
}

void wprint_lower_tetrimino(WINDOW *local_win, ttshape_state *shape_state) {
  int curr_y = shape_state->y;
  int reach = shape_state->lowest_y + 1;
  if (reach >= AREA_HEIGHT) {
    shape_state->must_change = 1;
    return;
  }

  ttpt *current_layout = shape_state->lo_set.layouts[shape_state->ori];
  for (int j = 0; j < BLOCK_COUNT; j++) {
    if (internal_grid[shape_state->y + current_layout[j].row + 1]
                     [shape_state->x + current_layout[j].col].occ) {

      shape_state->must_change = 1;
      return;
    }
  }

  // delete by resetting colors to default
  wprint_tetromino(local_win, shape_state, 1);
  shape_state->y = curr_y + 1;
  wprint_tetromino(local_win, shape_state, 0);
}

Orientation get_new_rotate_ori(Orientation ori, int anti_clockwise) {
  // anti_clockwise = -1 orientation, clockwise = +1 orientation
  return anti_clockwise ? (ori + (MAX_ORIENTATIONS)-1) % MAX_ORIENTATIONS
                        : (ori + 1) % MAX_ORIENTATIONS;
}

void attempt_rotation(ttshape_state *shape_state, Orientation new_rotate_ori) {
  // modify x and y values to fit new rotation
  int can_rotate = 0;
  int curr_x = shape_state->x;
  int curr_y = shape_state->y;
  ttpt *wall_kick_arr = get_wall_kick(new_rotate_ori, shape_state);
  int i_x = wall_kick_arr[0].col; // just here for initialization
  int i_y = wall_kick_arr[0].row;
  ttpt *new_layout = shape_state->lo_set.layouts[new_rotate_ori];

  for (int wall_kick_i = 0; wall_kick_i < NUM_ROT_ATTEMPTS; wall_kick_i++) {
    i_x = wall_kick_arr[wall_kick_i].col;
    i_y = wall_kick_arr[wall_kick_i].row;
    int rotation_available = 1;

    for (int j = 0; j < BLOCK_COUNT; j++) {
      int r = new_layout[j].row + curr_y + i_y;
      int c = new_layout[j].col + curr_x + i_x;

      if (r < 0 || r >= AREA_HEIGHT || c < 0 || c >= AREA_WIDTH ||
          internal_grid[r][c].occ) {
        rotation_available = 0;
        break;
      }
    }

    if (rotation_available) {
      can_rotate = 1;
      break;
    }
  }

  if (can_rotate) {
    shape_state->x = curr_x + i_x;
    shape_state->y = curr_y + i_y;
    shape_state->ori = new_rotate_ori;
  }
}

void wprint_rotate_tetrimino(WINDOW *local_win, ttshape_state *shape_state,
                             int anti_clockwise) {
  if (shape_state->lo_set.name == SQUARE)
    return;

  Orientation curr_ori = shape_state->ori;
  Orientation new_rotate_ori = get_new_rotate_ori(curr_ori, anti_clockwise);

  // delete by resetting colors to default
  wprint_tetromino(local_win, shape_state, 1);
  attempt_rotation(shape_state, new_rotate_ori);
  wprint_tetromino(local_win, shape_state, 0);
}

void init_settings(void) {
  setlocale(LC_ALL, "");
  initscr(); /* Start curses mode 		  */
  curs_set(0);
  start_color();
  raw();
  keypad(stdscr, TRUE);
  noecho();
  timeout(50);

  init_color(COLOR_PINK, 1000, 753, 796);

  init_pair(COLOR_PAIR_STRAIGHT, COLOR_CYAN, COLOR_CYAN);
  init_pair(COLOR_PAIR_SQUARE, COLOR_YELLOW, COLOR_YELLOW);
  init_pair(COLOR_PAIR_TSHAPE, COLOR_MAGENTA, COLOR_MAGENTA);
  init_pair(COLOR_PAIR_SKEW, COLOR_GREEN, COLOR_GREEN);
  init_pair(COLOR_PAIR_INVERSESKEW, COLOR_PINK, COLOR_PINK);
  init_pair(COLOR_PAIR_LSHAPE, COLOR_RED, COLOR_RED);
  init_pair(COLOR_PAIR_INVERSELSHAPE, COLOR_BLUE, COLOR_BLUE);
}

WINDOW *init_main_win(void) {
  WINDOW *main_win;
  int main_height = AREA_HEIGHT + 2;
  int main_width = AREA_WIDTH * 2 + 2;
  int main_starty = (LINES - main_height) / 2;
  int main_startx = (COLS - main_width) / 2;
  main_win = create_newwin(main_height, main_width, main_starty, main_startx);
  keypad(main_win, TRUE);
  wtimeout(main_win, 50);

  return main_win;
}

ttshape_state init_shape_state(void) {
  layout_set og_layout_set = get_rand_lo_set();

  ttshape_state state = {.lo_set = og_layout_set,
                         .ori = SPAWN,
                         .x = 0,
                         .y = 0,
                         .longest_x = 0,
                         .lowest_y = 0,
                         .must_change = 0};
  return state;
}

void init_main_win_act(WINDOW *main_win, ttshape_state *state) {
  int c;
  int running = 1;
  unsigned int tick_counter = 0;
  const int fall_interval = 20;

  while (running) {
    wprint_tetromino(main_win, state, 0);
    c = wgetch(main_win);
    switch (c) {
    case 'q':
      running = 0;
      break;
    case KEY_LEFT:
      /* code */
      wprint_horiz_shift_tetromino(main_win, state, 1);
      break;
    case KEY_RIGHT:
      wprint_horiz_shift_tetromino(main_win, state, 0);
      break;
    case KEY_DOWN:
      wprint_lower_tetrimino(main_win, state);
      break;
    case 'e': // anticlockwise
      wprint_rotate_tetrimino(main_win, state, 1);
      break;
    case 'r': // clockwise
      wprint_rotate_tetrimino(main_win, state, 0);
      break;
    default:
      break;
    }

    tick_counter++;
    if (tick_counter >= (unsigned)fall_interval) {
      wprint_lower_tetrimino(main_win, state);
      tick_counter = 0;
    }

    if (state->must_change) {
      for (int i = 0; i < BLOCK_COUNT; i++) {
        int aff_row = state->y + state->lo_set.layouts[state->ori][i].row;
        int aff_col = state->x + state->lo_set.layouts[state->ori][i].col;
        internal_grid[aff_row][aff_col].occ = 1;
        internal_grid[aff_row][aff_col].color = get_color_pair(state->lo_set.name);
        
      }

      *state = init_shape_state();
    }

    wrefresh(main_win);
  }
}

int main() {
  init_settings();
  srand(time(NULL));
  ttshape_state state = init_shape_state();

  printw("press q to quit");
  refresh();
  WINDOW *main_win = init_main_win();
  init_main_win_act(main_win, &state);

  endwin();
  return 0;
}

WINDOW *create_newwin(int height, int width, int starty, int startx) {
  WINDOW *local_win;

  local_win = newwin(height, width, starty, startx);
  box(local_win, 0, 0); /* 0, 0 gives default characters
                         * for the vertical and horizontal
                         * lines			*/
  wrefresh(local_win);  /* Show that box 		*/

  return local_win;
}
