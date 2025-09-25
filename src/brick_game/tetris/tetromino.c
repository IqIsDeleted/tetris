#include "backend.h"

void move_down(int **mtx) {
  figure_t *fg = updateFigurePosition();
  if (fg->active) {
    figure_position_updating(mtx, *fg, 0);
  }

  if (can_move(mtx, *fg, Down_dir)) {
    fg->block[0].i++;
    fg->block[1].i++;
    fg->block[2].i++;
    fg->block[3].i++;

  } else {
    fg->active = 0;
  }

  figure_position_updating(mtx, *fg, FILLED_CELL);
  check_full_line(mtx);
}

void move_horizontal(int **mtx, int direction) {
  figure_t *fg = updateFigurePosition();
  if (fg->active) {
    figure_position_updating(mtx, *fg, 0);
  }
  if (can_move(mtx, *fg, direction)) {
    fg->block[0].j += direction;
    fg->block[1].j += direction;
    fg->block[2].j += direction;
    fg->block[3].j += direction;
  }
  figure_position_updating(mtx, *fg, FILLED_CELL);
}

bool can_move(int **mtx, figure_t fg, int dir) {
  bool ret = true;

  for (int k = 0; ret && k < 4; k++) {
    int new_i = fg.block[k].i;
    int new_j = fg.block[k].j;

    switch (dir) {
      case Down_dir:
        new_i++;
        break;
      case Left_dir:
        new_j--;
        break;
      case Right_dir:
        new_j++;
        break;
    }

    if (new_i >= HEIGHT || new_j < 0 || new_j >= WIDTH) {
      ret = false;
    }

    if (ret && (mtx[new_i][new_j] == FILLED_CELL)) {
      ret = false;
    }
  }

  return ret;
}

bool can_rotate(int **field, block_t *blocks) {
  bool ret = true;

  for (int k = 0; ret && k < 4; k++) {
    int i = blocks[k].i;
    int j = blocks[k].j;

    if (i >= HEIGHT || j >= WIDTH) {
      ret = false;
    }

    if (ret && (field[i][j] == FILLED_CELL)) {
      ret = false;
    }
  }

  return ret;
}

void rotate_figure() {
  figure_t *figure = updateFigurePosition();
  GameInfo_t *game = currentGameInfo();

  if (figure->active) {
    figure_position_updating(game->next, *figure, 0);
  }

  block_t new_blocks[4];
  block_t center = figure->block[0];

  for (int k = 0; k < 4; k++) {
    int di = figure->block[k].i - center.i;
    int dj = figure->block[k].j - center.j;

    new_blocks[k].i = center.i + dj;
    new_blocks[k].j = center.j - di;
  }

  if (can_rotate(game->next, new_blocks) && figure->form != 'O') {
    for (int k = 0; k < 4; k++) {
      figure->block[k] = new_blocks[k];
    }
  }
  figure_position_updating(game->next, *figure, FILLED_CELL);
}

/*
 * Copyright (c) 2024 Eugen <dqeefcrcf@gmail.com>
 * License: S21
 */
