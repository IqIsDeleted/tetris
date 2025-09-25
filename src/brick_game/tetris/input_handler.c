#include "backend.h"

void down_process(int **field, bool hold) {
  figure_t *current_figure = updateFigurePosition();
  if (hold) {
    while (current_figure->active) {
      move_down(field);
    }
  } else {
    move_down(field);
  }
}

void process_game_actions(GameInfo_t *game, UserAction_t action, bool hold) {
  switch (action) {
    case Left:
      move_horizontal(game->next, Left_dir);
      break;

    case Right:
      move_horizontal(game->next, Right_dir);
      break;

    case Up:
      break;
    case Down:
      down_process(game->next, hold);
      break;

    case Action:
      rotate_figure();
      break;
    default:
      break;
  }
}

void userInput(UserAction_t action, bool hold) {
  GameInfo_t *game = currentGameInfo();
  tetris_state *state = current_fsm_state();

  if (action == Terminate) {
    get_signal(GameOver_signal);
  }

  switch (*state) {
    case STATE_START:
      if (action == Start) {
        get_signal(Game_signal);
      }
      break;

    case STATE_GAME:
      if (action == Pause) {
        get_signal(Pause_signal);
        game->pause = true;
      } else {
        process_game_actions(game, action, hold);
      }
      break;

    case STATE_PAUSE:
      if (action == Pause) {
        get_signal(Game_signal);
        game->pause = false;
      }
      break;

    case STATE_GAME_OVER:
  }
}

/*
 * Copyright (c) 2024 Eugen <dqeefcrcf@gmail.com>
 * License: S21
 */
