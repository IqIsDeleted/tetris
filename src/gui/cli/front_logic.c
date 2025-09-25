#include "front.h"

void next_figure_info_update(int mtx[2][6]) {
  memset(mtx, 0, sizeof(int) * 2 * 6);
  figure_t *next = updateNextFigure();

  if (next) {
    static const FigureTemplate templates[] = {
        {'I', {{0, 2}, {0, 1}, {0, 3}, {0, 4}}},
        {'O', {{0, 2}, {0, 3}, {1, 2}, {1, 3}}},
        {'J', {{1, 2}, {0, 2}, {1, 3}, {1, 4}}},
        {'L', {{1, 3}, {0, 3}, {1, 2}, {1, 1}}},
        {'Z', {{0, 3}, {0, 2}, {1, 3}, {1, 4}}},
        {'T', {{1, 2}, {0, 2}, {1, 1}, {1, 3}}},
        {'S', {{0, 2}, {0, 3}, {1, 1}, {1, 2}}},
    };

    int i = 0;
    for (; i < NUMBER_OF_FIGURES; ++i) {
      if (next->form == templates[i].element) {
        for (int j = 0; j < 4; ++j) {
          mtx[templates[i].block[j].i][templates[i].block[j].j] = '*';
        }
      }
    }
  }
}

UserAction_t get_game_action() {
  static const struct {
    int key;
    UserAction_t action;
  } keymap[] = {
      {'D', Right},          {'d', Right},
      {KEY_RIGHT, Right},    {RIGHT_ARROW, Right},

      {'A', Left},           {'a', Left},
      {KEY_LEFT, Left},      {LEFT_ARROW, Left},

      {'S', Down},           {'s', Down},
      {KEY_DOWN, Down},      {DOWN_ARROW, Down},
      {'X', Down},           {'x', Down},

      {'P', Pause},          {'p', Pause},

      {'C', Action},         {'c', Action},
      {'Z', Action},         {'z', Action},

      {KEY_ENTER, Start},    {GAME_START, Start},

      {KEY_EXIT, Terminate}, {GAME_TERMINATE, Terminate},

  };

  int ret = ACTION_NONE;
  int ch = getch();

  if (ch != ERR) {
    for (size_t i = 0; i < sizeof(keymap) / sizeof(keymap[0]); i++) {
      if (ch == keymap[i].key) {
        ret = keymap[i].action;
      }
    }
  }

  userInput(ret, (ret == Down) && !(ch == 'X' || ch == 'x'));

  return ret;
}

void init_front() {
  initscr();
  curs_set(0);
  nodelay(stdscr, TRUE);
  cbreak();
  noecho();
  keypad(stdscr, TRUE);
  ESCDELAY = 25;
}

void cleanup() {
  curs_set(1);
  echo();
  endwin();
}

/*
 * Copyright (c) 2024 Eugen <dqeefcrcf@gmail.com>
 * License: S21
 */
