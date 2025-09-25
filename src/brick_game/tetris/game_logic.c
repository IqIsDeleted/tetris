#include "backend.h"

/*
Contains the history of piece.
*/
static bool history_piece(char piece, unsigned short int mod) {
  static char history[5] = "AAAA\0";
  bool ret = 0;

  if (mod & CHECK) {
    for (int i = 0; !ret && i < 4; ++i) {
      ret = history[i] == piece;
    }
  }

  if (mod & UPDATE_HISTORY) {
    static unsigned short int num = 0;
    if (!ret) {
      history[num++] = piece;
    }
    if (num > 3) {
      num = 0;
    }
  }

  return ret;
}

/*
Creates a figure using a bag.
*/
char figure_create() {
  static char figure_bag[36] = {"JIOLZTSJIOLZTSJIOLZTSJIOLZTSJIOLZTS\0"};
  char all_figure[8] = {"JIOLZTS\0"};

  unsigned short int num = rand() % 35;
  char ret = 'I';

  for (int roll = 6; roll > 0; --roll) {
    if (!history_piece(figure_bag[num], ALL_MOD)) {
      ret = figure_bag[num];

    } else if (roll == 1) {
      ret = figure_bag[num];
      history_piece(figure_bag[num], UPDATE_HISTORY);

    } else {
      num = rand() % 35;
    }
  }

  unsigned short int new = rand() % 7;
  for (; history_piece(all_figure[new], CHECK); new = rand() % 7);

  figure_bag[num] = all_figure[new];

  return ret;
}

void current_score(int destroyed_line) {
  GameInfo_t *game = currentGameInfo();
  switch (destroyed_line) {
    case 1:
      game->score += 100;
      break;
    case 2:
      game->score += 300;
      break;
    case 3:
      game->score += 700;
      break;
    case 4:
      game->score += 1500;
      break;
    default:
      break;
  }
}

figure_t figure_t_fill(char el) {
  figure_t ret = {0};
  ret.form = el;

  static const FigureTemplate templates[] = {
      {'I', {{0, 4}, {0, 3}, {0, 5}, {0, 6}}},
      {'O', {{0, 4}, {0, 5}, {1, 4}, {1, 5}}},
      {'J', {{1, 4}, {0, 4}, {1, 5}, {1, 6}}},
      {'L', {{1, 5}, {0, 5}, {1, 4}, {1, 3}}},
      {'Z', {{0, 5}, {0, 4}, {1, 5}, {1, 6}}},
      {'T', {{1, 4}, {0, 4}, {1, 3}, {1, 5}}},
      {'S', {{0, 4}, {0, 5}, {1, 3}, {1, 4}}},
  };

  for (int i = 0; i < NUMBER_OF_FIGURES; ++i) {
    if (el == templates[i].element) {
      ret.active = 1;
      for (int j = 0; j < 4; ++j) {
        ret.block[j] = templates[i].block[j];
      }
    }
  }

  return ret;
}

static void upd_full_line(int **field, int i) {
  for (; i > 0; --i) {
    memcpy(field[i], field[i - 1], sizeof(int) * WIDTH);
  }

  memset(field[0], 0, sizeof(int) * WIDTH);
}

int check_full_line(int **field) {  // Clears a filled strip and calculates the
                                    // number of cleared lines
  int ret = 0;
  figure_t *fg = updateFigurePosition();

  if (fg->active == 0) {
    for (int i = 0; i < HEIGHT; ++i) {
      int fill_line = FILLED_CELL;
      for (int j = 0; fill_line && j < WIDTH; ++j) {
        fill_line &= *(*(field + i) + j);
      }
      if (fill_line == FILLED_CELL) {
        ret++;
        upd_full_line(field, i);
      }
    }
  }

  current_score(ret);

  return ret;
}

void speed_process(GameInfo_t *game) {
  static clock_t last_time = 0;
  clock_t current_time = clock();

  game->level = game->score / BASE_LEVEL_SCORE;
  if (game->level > MAX_LEVEL) {
    game->level = MAX_LEVEL;
  }

  static const int speed_table[11] = {1000, 900, 800, 700, 600, 500,
                                      400,  300, 200, 150, 100};

  int delay_ms = speed_table[game->level];
  game->speed = delay_ms;

  double elapsed_ms =
      (double)(current_time - last_time) * 1000 / CLOCKS_PER_SEC;

  if (elapsed_ms >= delay_ms) {
    move_down(game->next);
    last_time = current_time;
  }
}

/*
 * Copyright (c) 2024 Eugen <dqeefcrcf@gmail.com>
 * License: S21
 */
