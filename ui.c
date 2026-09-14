#include <ncurses.h>
#include <locale.h>
#include <panel.h>

#define BLOCK_COUNT 4

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

static ttshape STRAIGHT_LAYOUT[BLOCK_COUNT] = {
  {0, 0}, {0, 1}, {0, 2}, {0, 3}
};

static ttshape SQUARE_LAYOUT[BLOCK_COUNT] = {
  {0, 0}, {1, 0}, {0, 1}, {1, 1}
};

static ttshape TSHAPE_LAYOUT[BLOCK_COUNT] = {
  {1, 0}, {1, 1}, {1, 2}, {0, 1}
};

static ttshape SKEW_LAYOUT[BLOCK_COUNT] = {
  {0, 0}, {0, 1}, {1, 1}, {1, 2}
};

static ttshape LSHAPE_LAYOUT[BLOCK_COUNT] = {
  {0, 0}, {0, 1}, {0, 2}, {1, 0}
};

static ttshape INVERSELSHAPE_LAYOUT[BLOCK_COUNT] = {
  {0, 0}, {0, 1}, {0, 2}, {1, 2}
};

ttshape *get_ttshape(Tetromino tt) {
  switch (tt) {
  case STRAIGHT: return STRAIGHT_LAYOUT;
  case SQUARE: return SQUARE_LAYOUT;
  case TSHAPE: return TSHAPE_LAYOUT;
  case SKEW: return SKEW_LAYOUT;
  case LSHAPE: return LSHAPE_LAYOUT;
  case INVERSELSHAPE: return INVERSELSHAPE_LAYOUT;
  default:
    return 0;
    break;
  }
}

int get_color_pair(Tetromino tt) {
  switch(tt) {
    case STRAIGHT: return COLOR_PAIR_STRAIGHT;
    case SQUARE: return COLOR_PAIR_SQUARE;
    case TSHAPE: return COLOR_PAIR_TSHAPE;
    case SKEW: return COLOR_PAIR_SKEW;
    case LSHAPE: return COLOR_PAIR_LSHAPE;
    case INVERSELSHAPE: return COLOR_PAIR_INVERSELSHAPE;
    default:
      return 0;
      break;
  }
}

void wprint_tetronimo(WINDOW *local_win, Tetromino tt, int y, int x, int del) {
  int i = 0;
  int col = 0;
  int row = 0;
  ttshape *layout = get_ttshape(tt);
  int color_pair = get_color_pair(tt);
    !del ? wattron(local_win, COLOR_PAIR(color_pair)) : wattron(local_win, COLOR_PAIR(0));
    for(i = 0; i < BLOCK_COUNT; i++) {
      col = layout[i].col * 2 + x;
      row = layout[i].row + y;
      mvwprintw(local_win, row, col, "  ");
    }
    wattroff(local_win, COLOR_PAIR(color_pair));
    wrefresh(local_win);
}

void wprint_shift_tetronimo(WINDOW *local_win, Tetromino tt, int is_left, int fact) {
  wprint_tetronimo(local_win, SKEW, 8, 2, 0);
  wprint_tetronimo(local_win, SKEW, 8, 2, 1);
}

WINDOW *create_newwin(int height, int width, int starty, int startx);

void init_settings(void) {
  setlocale(LC_ALL, "");
  initscr();			/* Start curses mode 		  */
  curs_set(0);
  start_color();
  raw();
  keypad(stdscr, TRUE);
  noecho();

  init_pair(COLOR_PAIR_STRAIGHT,      COLOR_CYAN,    COLOR_CYAN);
  init_pair(COLOR_PAIR_SQUARE,        COLOR_YELLOW,  COLOR_YELLOW);
  init_pair(COLOR_PAIR_TSHAPE,        COLOR_MAGENTA, COLOR_MAGENTA);
  init_pair(COLOR_PAIR_SKEW,          COLOR_GREEN,   COLOR_GREEN);
  init_pair(COLOR_PAIR_LSHAPE,        COLOR_RED,   COLOR_RED);
  init_pair(COLOR_PAIR_INVERSELSHAPE, COLOR_BLUE,    COLOR_BLUE);
}

int main() {
	init_settings();
  WINDOW *main_win;
  int c;
  int main_height = 20;
	int main_width = 22;
	int main_starty = (LINES - main_height) / 2;
	int main_startx = (COLS - main_width) / 2;
  
  printw("press q to quit");
  refresh();
  main_win = create_newwin(main_height, main_width, main_starty, main_startx);

  while((c = wgetch(main_win)) != 'q') {
    wrefresh(main_win);
  }

	endwin();
	return 0;
}

WINDOW *create_newwin(int height, int width, int starty, int startx){	
  WINDOW *local_win;

	local_win = newwin(height, width, starty, startx);
	box(local_win, 0, 0);		/* 0, 0 gives default characters
					 * for the vertical and horizontal
					 * lines			*/
	wrefresh(local_win);		/* Show that box 		*/

	return local_win;
}