#include <locale.h>
#include <ncurses.h>
#include <panel.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>

#define COLOR_PINK 8
#define BLOCK_COUNT 4
#define MAX_ORIENTATIONS 4
#define AREA_HEIGHT 18
#define AREA_WIDTH 10
#define NUM_TETROMINOS 7

typedef enum
{
  STRAIGHT,
  SQUARE,
  TSHAPE,
  SKEW,        // also known as Z
  INVERSESKEW, // also known as S
  LSHAPE,
  INVERSELSHAPE
} Tetromino;

typedef enum
{
  COLOR_PAIR_STRAIGHT = 1,
  COLOR_PAIR_SQUARE,
  COLOR_PAIR_TSHAPE,
  COLOR_PAIR_SKEW,
  COLOR_PAIR_INVERSESKEW,
  COLOR_PAIR_LSHAPE,
  COLOR_PAIR_INVERSELSHAPE,
  COLOR_RESET
} TetrominoColorPair;

typedef struct TtPoint
{
  int row;
  int col;
} ttpt;

typedef struct LayoutSet {
  Tetromino name;
  ttpt layouts[MAX_ORIENTATIONS][BLOCK_COUNT];
  int orientations;
} layout_set;

typedef struct TtShapeState
{
  layout_set lo_set;
  int ori;
  int x;
  int y;
  int longest_x;
  int lowest_y;
  int must_change; // 0 if the current shape can continue to be used, 1 if it
                   // needs to be changed for a new shape in the next iteration
} ttshape_state;

int internal_grid[AREA_HEIGHT][AREA_WIDTH] = {
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
};

static layout_set STRAIGHT_LAYOUT= {
  .name = STRAIGHT,
  .layouts = {
    {{0, 0}, {0, 1}, {0, 2}, {0, 3}},
    {{0, 0}, {1, 0}, {2, 0}, {3, 0}},
  },
  .orientations = 2
};

static layout_set SQUARE_LAYOUT = {
  .name = SQUARE,
  .layouts = {
    {{0, 0}, {1, 0}, {0, 1}, {1, 1}}
  },
  .orientations = 1
};

static layout_set TSHAPE_LAYOUT = {
  .name = TSHAPE,
  .layouts = {
    {{0, 0}, {0, 1}, {0, 2}, {1, 1}},
    {{0, 1}, {1, 1}, {2, 1}, {1, 0}},
    {{1, 0}, {1, 1}, {1, 2}, {0, 1}},
    {{0, 0}, {1, 0}, {2, 0}, {1, 1}}
  },
  .orientations = 4
};

static layout_set SKEW_LAYOUT = { // also known as Z
  .name = SKEW,
  .layouts = {
    {{0, 0}, {0, 1}, {1, 1}, {1, 2}},
    {{0, 1}, {1, 1}, {1, 0}, {2, 0}},
  },
  .orientations = 2
};

static layout_set INVERSESKEW_LAYOUT = { // also known as S
  .name = INVERSESKEW,
  .layouts = {
    {{1, 0}, {1, 1}, {0, 1}, {0, 2}},
    {{0, 0}, {1, 0}, {1, 1}, {2, 1}}
  },
  .orientations = 2
};

static layout_set LSHAPE_LAYOUT = {
  .name = LSHAPE,
  .layouts = {
    {{0, 0}, {0, 1}, {0, 2}, {1, 0}},
    {{0, 0}, {0, 1}, {1, 1}, {2, 1}},
    {{0, 2}, {1, 0}, {1, 1}, {1, 2}},
    {{0, 0}, {1, 0}, {2, 0}, {2, 1}},
  },
  .orientations = 4
};

static layout_set INVERSELSHAPE_LAYOUT = {
  .name = INVERSELSHAPE,
  .layouts = {
      {{0, 0}, {0, 1}, {0, 2}, {1, 2}},
      {{0, 1}, {1, 1}, {2, 0}, {2, 1}},
      {{0, 0}, {1, 0}, {1, 1}, {1, 2}},
      {{0, 0}, {0, 1}, {1, 0}, {2, 0}},
  },
  .orientations = 4
};


WINDOW *create_newwin(int height, int width, int starty, int startx);

layout_set get_ttlayout(Tetromino tt)
{
  switch (tt)
  {
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
    return STRAIGHT_LAYOUT;
    break;
  }
}

int get_color_pair(Tetromino tt)
{
  switch (tt)
  {
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

layout_set get_rand_lo_set(void)
{
  Tetromino new = (Tetromino)(rand() % NUM_TETROMINOS);
  return get_ttlayout(new);
}

void wprint_tetromino(WINDOW *local_win, ttshape_state *shape_state, int del)
{
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
  for (i = 0; i < BLOCK_COUNT; i++)
  {
    col_ui = layout[i].col * 2 + (shape_state->x * 2) + 1;
    col = layout[i].col + shape_state->x;
    if (col > longest_col)
      longest_col = col;
    row = layout[i].row + shape_state->y + 1;
    if (row > lowest_row)
    {
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
                                  int is_left)
{
  int fact = is_left ? -1 : 1;
  int should_mv = 1;
  if ((!is_left && shape_state->longest_x + fact >= AREA_WIDTH) ||
      (is_left && shape_state->x + fact < 0))
    return;

  ttpt *current_layout = shape_state->lo_set.layouts[shape_state->ori];
  int x_after = shape_state->x + fact;
  for (int i = 0; i < BLOCK_COUNT; i++)
  {
    if (internal_grid[shape_state->y + current_layout[i].row]
                     [x_after + current_layout[i].col])
    {
      should_mv = 0;
    }
  }

  if (should_mv)
  {
    // delete by resetting colors to default
    wprint_tetromino(local_win, shape_state, 1);
    shape_state->x = shape_state->x + fact;
    // set new position and print
    wprint_tetromino(local_win, shape_state, 0);
  }
}

void wprint_lower_tetrimino(WINDOW *local_win, ttshape_state *shape_state)
{
  int curr_y = shape_state->y;
  int reach = shape_state->lowest_y + 1;
  if (reach >= AREA_HEIGHT)
  {
    shape_state->must_change = 1;
    return;
  }

  ttpt *current_layout = shape_state->lo_set.layouts[shape_state->ori];
  for (int j = 0; j < BLOCK_COUNT; j++)
  {
    if (internal_grid[shape_state->y + current_layout[j].row + 1]
                     [shape_state->x + current_layout[j].col])
    {

      shape_state->must_change = 1;
      return;
    }
  }

  // delete by resetting colors to default
  wprint_tetromino(local_win, shape_state, 1);
  shape_state->y = curr_y + 1;

  wprint_tetromino(local_win, shape_state, 0);
}

void wprint_rotate_tetrimino(WINDOW *local_win, ttshape_state *shape_state, int anti_clockwise) {
  // anti_clockwise = -1 orientation, clockwise = +1 orientation
  int curr_ori = shape_state->ori;
  int max_ori = shape_state->lo_set.orientations - 1;

  // delete by resetting colors to default
  wprint_tetromino(local_win, shape_state, 1);

  if(curr_ori == 0 && anti_clockwise) {
    shape_state->ori = max_ori;
  }else if(curr_ori == max_ori && !anti_clockwise) {
    shape_state->ori = 0;
  }else {
    shape_state->ori = anti_clockwise ? curr_ori - 1 : curr_ori + 1;
  }

  wprint_tetromino(local_win, shape_state, 0);
}

void init_settings(void)
{
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

WINDOW *init_main_win(void)
{
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

ttshape_state init_shape_state(void)
{
  layout_set og_layout_set = get_rand_lo_set();

  ttshape_state state = {
      .lo_set = og_layout_set,
      .ori = 0,
      .x = 0,
      .y = 0,
      .longest_x = 0,
      .lowest_y = 0,
      .must_change = 0};
  return state;
}


void init_main_win_act(WINDOW *main_win, ttshape_state *state)
{
  int c;
  int running = 1;
  unsigned int tick_counter = 0;
  const int fall_interval = 20;

  while (running)
  {
    wprint_tetromino(main_win, state, 0);
    c = wgetch(main_win);
    switch (c)
    {
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
    if (tick_counter >= (unsigned)fall_interval)
    {
      wprint_lower_tetrimino(main_win, state);
      tick_counter = 0;

      // printw("layout_x: %d\n", state->x);
      // printw("layout_longest_x: %d\n", state->longest_x);
      // refresh();
    }

    if (state->must_change)
    {
      for (int i = 0; i < BLOCK_COUNT; i++)
      {
        internal_grid[state->y + state->lo_set.layouts[state->ori][i].row]
                     [state->x + state->lo_set.layouts[state->ori][i].col] = 1;
      }

      *state = init_shape_state();
    }

    wrefresh(main_win);
  }
}

int main()
{
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

WINDOW *create_newwin(int height, int width, int starty, int startx)
{
  WINDOW *local_win;

  local_win = newwin(height, width, starty, startx);
  box(local_win, 0, 0); /* 0, 0 gives default characters
                         * for the vertical and horizontal
                         * lines			*/
  wrefresh(local_win);  /* Show that box 		*/

  return local_win;
}
