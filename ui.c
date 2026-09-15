#include <locale.h>
#include <ncurses.h>
#include <panel.h>

#define BLOCK_COUNT 4
#define AREA_HEIGHT 20
#define AREA_WIDTH 22
#define EFFECTIVE_WIDTH AREA_WIDTH - 1 * 2

typedef enum {
  STRAIGHT,
  SQUARE,
  TSHAPE,
  SKEW,
  LSHAPE,
  INVERSELSHAPE
} Tetromino;

typedef enum {
  COLOR_PAIR_STRAIGHT = 1,
  COLOR_PAIR_SQUARE,
  COLOR_PAIR_TSHAPE,
  COLOR_PAIR_SKEW,
  COLOR_PAIR_LSHAPE,
  COLOR_PAIR_INVERSELSHAPE,
  COLOR_RESET
} TetrominoColorPair;

typedef struct TTShape {
  int row;
  int col;
} ttshape;

typedef struct TtShapeState {
  Tetromino current_shape;
  int x;
  int y;
  int longest_x;
} ttshape_state;

static ttshape STRAIGHT_LAYOUT[BLOCK_COUNT] = {{0, 0}, {0, 1}, {0, 2}, {0, 3}};

static ttshape SQUARE_LAYOUT[BLOCK_COUNT] = {{0, 0}, {1, 0}, {0, 1}, {1, 1}};

static ttshape TSHAPE_LAYOUT[BLOCK_COUNT] = {{1, 0}, {1, 1}, {1, 2}, {0, 1}};

static ttshape SKEW_LAYOUT[BLOCK_COUNT] = {{0, 0}, {0, 1}, {1, 1}, {1, 2}};

static ttshape LSHAPE_LAYOUT[BLOCK_COUNT] = {{0, 0}, {0, 1}, {0, 2}, {1, 0}};

static ttshape INVERSELSHAPE_LAYOUT[BLOCK_COUNT] = {
    {0, 0}, {0, 1}, {0, 2}, {1, 2}};

ttshape *get_ttshape(Tetromino tt) {
  switch (tt) {
  case STRAIGHT:
    return STRAIGHT_LAYOUT;
  case SQUARE:
    return SQUARE_LAYOUT;
  case TSHAPE:
    return TSHAPE_LAYOUT;
  case SKEW:
    return SKEW_LAYOUT;
  case LSHAPE:
    return LSHAPE_LAYOUT;
  case INVERSELSHAPE:
    return INVERSELSHAPE_LAYOUT;
  default:
    return 0;
    break;
  }
}

int get_color_pair(Tetromino tt) {
  switch (tt) {
  case STRAIGHT:
    return COLOR_PAIR_STRAIGHT;
  case SQUARE:
    return COLOR_PAIR_SQUARE;
  case TSHAPE:
    return COLOR_PAIR_TSHAPE;
  case SKEW:
    return COLOR_PAIR_SKEW;
  case LSHAPE:
    return COLOR_PAIR_LSHAPE;
  case INVERSELSHAPE:
    return COLOR_PAIR_INVERSELSHAPE;
  default:
    return 0;
    break;
  }
}

void wprint_tetromino(WINDOW *local_win, ttshape_state *shape_state, int del) {
  int i = 0;
  int col = 0;
  int longest_col = 0;
  int row = 0;
  ttshape *layout = get_ttshape(shape_state->current_shape);
  int color_pair = get_color_pair(shape_state->current_shape);
  int pair_to_use = del ? 0 : color_pair;
  wattrset(local_win, COLOR_PAIR(pair_to_use));

  for (i = 0; i < BLOCK_COUNT; i++) {
    col = layout[i].col * 2 + shape_state->x;
    longest_col = col > longest_col ? col : longest_col;


    row = layout[i].row + shape_state->y;
    refresh();
    mvwprintw(local_win, row, col, "  ");
  }

  shape_state->longest_x = longest_col;
  wattrset(local_win, A_NORMAL);
  wrefresh(local_win);
}

void wprint_horiz_shift_tetromino(WINDOW *local_win, ttshape_state *shape_state, int is_left,
                                  int fact) {
  int curr_x = shape_state->x;
  // delete by resetting colors to default
  wprint_tetromino(local_win, shape_state, 1);
  if (is_left) {
    shape_state->x = curr_x - fact > 0 ? curr_x - fact : curr_x;
  } else {
    shape_state->x = shape_state->longest_x + fact < EFFECTIVE_WIDTH ? curr_x + fact : curr_x;
  }

  // set new position and print
  wprint_tetromino(local_win, shape_state, 0);
}

WINDOW *create_newwin(int height, int width, int starty, int startx);

void init_settings(void) {
  setlocale(LC_ALL, "");
  initscr(); /* Start curses mode 		  */
  curs_set(0);
  start_color();
  raw();
  keypad(stdscr, TRUE);
  noecho();

  init_pair(COLOR_PAIR_STRAIGHT, COLOR_CYAN, COLOR_CYAN);
  init_pair(COLOR_PAIR_SQUARE, COLOR_YELLOW, COLOR_YELLOW);
  init_pair(COLOR_PAIR_TSHAPE, COLOR_MAGENTA, COLOR_MAGENTA);
  init_pair(COLOR_PAIR_SKEW, COLOR_GREEN, COLOR_GREEN);
  init_pair(COLOR_PAIR_LSHAPE, COLOR_RED, COLOR_RED);
  init_pair(COLOR_PAIR_INVERSELSHAPE, COLOR_BLUE, COLOR_BLUE);
}

int main() {
  init_settings();
  WINDOW *main_win;
  int c;
  int main_height = AREA_HEIGHT;
  int main_width = AREA_WIDTH;
  int main_starty = (LINES - main_height) / 2;
  int main_startx = (COLS - main_width) / 2;

  ttshape_state state = {
    .current_shape = TSHAPE,
    .x = 1,
    .y = 1,
    .longest_x = 1
  };

  printw("press q to quit");
  refresh();
  main_win = create_newwin(main_height, main_width, main_starty, main_startx);
  keypad(main_win, TRUE);
  wprint_tetromino(main_win, &state, 0);
  while ((c = wgetch(main_win)) != 'q') {
    if (c == KEY_LEFT) {
      wprint_horiz_shift_tetromino(main_win, &state, 1, 1);
    } else if (c == KEY_RIGHT) {
      wprint_horiz_shift_tetromino(main_win, &state, 0, 1);
    }
    wrefresh(main_win);
  }

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
