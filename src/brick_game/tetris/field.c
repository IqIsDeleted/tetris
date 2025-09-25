#include "../../gui/cli/front.h"
#include "backend.h"

static void field_create(int rows, int cols, int ***result) {
  *result = calloc(rows, sizeof(int *));
  int *data = calloc(rows * cols, sizeof(int));

  if (!*result || !data) {
    free(*result);
    free(data);
    *result = NULL;
  }

  for (int i = 0; i < rows; ++i) {
    (*result)[i] = &data[i * cols];
  }
}

static void field_remove(int ***A) {
  if (*A) {
    free((*A)[0]);
    free(*A);
  }
  *A = NULL;
}

void figure_position_updating(int **mtx, figure_t fg, int new_cell) {
  mtx[fg.block[0].i][fg.block[0].j] = new_cell;
  mtx[fg.block[1].i][fg.block[1].j] = new_cell;
  mtx[fg.block[2].i][fg.block[2].j] = new_cell;
  mtx[fg.block[3].i][fg.block[3].j] = new_cell;
}

figure_t *updateFigurePosition() {
  static figure_t current = {0};
  return &current;
}

figure_t *updateNextFigure() {
  static figure_t next = {0};
  return &next;
}

static void start_process() {
  srand(time(NULL));

  figure_t *next = updateNextFigure();
  *next = figure_t_fill(figure_create());
}

void update_high_score(int new_high_score) {
  FILE *f = fopen("game/.tetris_highscore.dat", "wb");
  new_high_score ^= 0x8E19B7D4;

  if (f) {
    fwrite(&new_high_score, sizeof(int), 1, f);
    fclose(f);
  }
}

int read_high_score() {
  FILE *f = fopen("game/.tetris_highscore.dat", "rb+");
  int high_score = 0;

  if (f) {
    fread(&high_score, sizeof(int), 1, f);
    high_score ^= 0x8E19B7D4;
    fclose(f);
  } else {
    update_high_score(high_score);
  }

  return high_score;
}

void game_initialize(GameInfo_t *current) {
  static bool initialized = false;
  if (!initialized) {
    start_process();
    memset(current, 0, sizeof(GameInfo_t));
    current->high_score = read_high_score();
    field_create(HEIGHT, WIDTH, &current->field);
    field_create(HEIGHT, WIDTH, &current->next);
    initialized = true;
  }
}

GameInfo_t *currentGameInfo() {
  static GameInfo_t current;

  return &current;
}

void game_terminate() {
  GameInfo_t *game = currentGameInfo();
  field_remove(&game->field);
  field_remove(&game->next);
}

static bool check_end(int **field) {
  bool ret = false;
  for (int i = 0; i < 1; i++) {
    for (int j = 0; j < WIDTH; j++) {
      if (field[i][j] != 0) {
        ret = true;
      }
    }
  }
  return ret;
}

bool game_over(GameInfo_t *game) {
  figure_t *current_figure = updateFigurePosition();
  return (game->next[current_figure->block[0].i][current_figure->block[0].j] ||
          game->next[current_figure->block[1].i][current_figure->block[1].j] ||
          game->next[current_figure->block[2].i][current_figure->block[2].j] ||
          game->next[current_figure->block[3].i][current_figure->block[3].j]);
}

void process_game_state() {
  GameInfo_t *game = currentGameInfo();
  figure_t *current_figure = updateFigurePosition();
  figure_t *next_figure = updateNextFigure();

  if (current_figure->active == 0) {
    *current_figure = *next_figure;
    *next_figure = figure_t_fill(figure_create());

    if (game_over(game) || check_end(game->next)) {
      get_signal(GameOver_signal);
    } else {
      figure_position_updating(game->next, *current_figure, FILLED_CELL);
    }
  }

  if (game->field && game->next) {
    memcpy(game->field[0], game->next[0], sizeof(int) * HEIGHT * WIDTH);
  }

  speed_process(game);

  if (game->score > game->high_score) {
    game->high_score = game->score;
    update_high_score(game->high_score);
  }
}

GameInfo_t updateCurrentState() {
  GameInfo_t *game = currentGameInfo();
  tetris_state *state = current_fsm_state();

  game_initialize(game);

  switch (*state) {
    case STATE_START:
      break;

    case STATE_GAME: {
      process_game_state();
      break;
    }

    case STATE_PAUSE:
      break;

    case STATE_GAME_OVER:
      game_terminate();
      break;

    default:
      break;
  }

  return *game;
}

/*
 * Copyright (c) 2024 Eugen <dqeefcrcf@gmail.com>
 * License: S21
 */
