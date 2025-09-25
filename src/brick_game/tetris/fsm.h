#ifndef FSM_H
#define FSM_H

#include "backend.h"

typedef enum {
  STATE_START,
  STATE_GAME,
  STATE_PAUSE,
  STATE_GAME_OVER
} tetris_state;

typedef enum {
  Start_signal,
  Game_signal,
  Pause_signal,
  GameOver_signal
} signals;

void get_signal(signals input);
tetris_state *current_fsm_state();

#endif

/*
 * Copyright (c) 2024 Eugen <dqeefcrcf@gmail.com>
 * License: S21
 */