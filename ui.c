#include <locale.h>
#include <ncurses.h>
#include <panel.h>
#include <stdlib.h>
#include <time.h>

#define COLOR_PINK 8
#define BLOCK_COUNT 4
#define AREA_HEIGHT 20
#define AREA_WIDTH 22
#define NUM_TETROMINOS 7
#define EFFECTIVE_HEIGHT AREA_HEIGHT - 2
#define EFFECTIVE_WIDTH AREA_WIDTH - 2

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

typedef struct TTShape {
  int row;
  int col;
} ttshape;

typedef struct TtShapeState {
  Tetromino current_shape;
  int x;
  int y;
  int longest_x;
  int lowest_y;
  int must_change; // 0 if the current shape can continue to be used, 1 if it
                   // needs to be changed for a new shape in the next iteration
} ttshape_state;

static ttshape STRAIGHT_LAYOUT[BLOCK_COUNT] = {{0, 0}, {0, 1}, {0, 2}, {0, 3}};
static ttshape SQUARE_LAYOUT[BLOCK_COUNT] = {{0, 0}, {1, 0}, {0, 1}, {1, 1}};
static ttshape TSHAPE_LAYOUT[BLOCK_COUNT] = {{1, 0}, {1, 1}, {1, 2}, {0, 1}};
static ttshape SKEW_LAYOUT[BLOCK_COUNT] = {
    {0, 0}, {0, 1}, {1, 1}, {1, 2}}; // also known as Z
static ttshape INVERSESKEW_LAYOUT[BLOCK_COUNT] = {
    {1, 0}, {1, 1}, {0, 1}, {0, 2}}; // also known as S
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
  case INVERSESKEW:
    return INVERSESKEW_LAYOUT;
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

Tetromino get_rand_shape(Tetromino *prev_shape) {
  srand(time(NULL));
  Tetromino new = (Tetromino)(rand() % NUM_TETROMINOS);
  if (prev_shape == NULL)
    return new;

  while (new == *prev_shape) {
    new = (Tetromino)(rand() % (NUM_TETROMINOS + 1));
  }

  return new;
}

void wprint_tetromino(WINDOW *local_win, ttshape_state *shape_state, int del) {
  int i = 0;
  int col = 0;
  int longest_col = 0;
  int lowest_row = 0;
  int row = 0;
  ttshape *layout = get_ttshape(shape_state->current_shape);
  int color_pair = get_color_pair(shape_state->current_shape);
  int pair_to_use = del ? 0 : color_pair;
  wattrset(local_win, COLOR_PAIR(pair_to_use));

  for (i = 0; i < BLOCK_COUNT; i++) {
    col = layout[i].col * 2 + shape_state->x;
    if (col > longest_col)
      longest_col = col;
    row = layout[i].row + shape_state->y;
    if (row > lowest_row)
      lowest_row = row;

    refresh();
    mvwprintw(local_win, row, col, "  ");
  }

  shape_state->longest_x = longest_col;
  shape_state->lowest_y = lowest_row;
  wattrset(local_win, A_NORMAL);
  wrefresh(local_win);
}

void wprint_horiz_shift_tetromino(WINDOW *local_win, ttshape_state *shape_state,
                                  int is_left, int fact) {
  int curr_x = shape_state->x;
  // delete by resetting colors to default
  wprint_tetromino(local_win, shape_state, 1);
  if (is_left) {
    shape_state->x = curr_x - fact > 0 ? curr_x - fact : curr_x;
  } else {
    shape_state->x = shape_state->longest_x + fact < EFFECTIVE_WIDTH
                         ? curr_x + fact
                         : curr_x;
  }

  // set new position and print
  wprint_tetromino(local_win, shape_state, 0);
}

void wprint_lower_tetrimino(WINDOW *local_win, ttshape_state *shape_state,
                            int fact) {
  int curr_y = shape_state->y;
  if (shape_state->lowest_y + fact > EFFECTIVE_HEIGHT) {
    shape_state->must_change = 1;
    return;
  }

  // delete by resetting colors to default
  wprint_tetromino(local_win, shape_state, 1);
  shape_state->y = curr_y + fact;

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
  int main_height = AREA_HEIGHT;
  int main_width = AREA_WIDTH;
  int main_starty = (LINES - main_height) / 2;
  int main_startx = (COLS - main_width) / 2;
  main_win = create_newwin(main_height, main_width, main_starty, main_startx);
  keypad(main_win, TRUE);
  wtimeout(main_win, 50);

  return main_win;
}

void init_main_win_act(WINDOW *main_win, ttshape_state *state) {
  int c;
  int x_fact = 2;
  int y_fact = 1;
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
      wprint_horiz_shift_tetromino(main_win, state, 1, x_fact);
      break;
    case KEY_RIGHT:
      wprint_horiz_shift_tetromino(main_win, state, 0, x_fact);
      break;
    case KEY_DOWN:
      wprint_lower_tetrimino(main_win, state, y_fact);
      break;
    default:
      break;
    }

    tick_counter++;
    if (tick_counter >= (unsigned)fall_interval) {
      wprint_lower_tetrimino(main_win, state, y_fact);
      tick_counter = 0;
    }

    if (state->must_change) {
      state->current_shape = get_rand_shape(NULL);
      state->x = 1;
      state->y = 1;
      state->longest_x = 0;
      state->lowest_y = 0;
      state->must_change = 0;
    }

    wrefresh(main_win);
  }
}

int main() {
  init_settings();
  ttshape_state state = {.current_shape = get_rand_shape(NULL),
                         .x = 1,
                         .y = 1,
                         .longest_x = 0,
                         .lowest_y = 0};

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
