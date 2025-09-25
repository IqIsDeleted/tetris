#include "fsm.h"

void get_signal(signals input) {
  tetris_state *state = current_fsm_state();
  *state = (tetris_state)input;
}

tetris_state *current_fsm_state() {
  static tetris_state current_state = STATE_START;
  return &current_state;
}

/*
 * Copyright (c) 2024 Eugen <dqeefcrcf@gmail.com>
 * License: S21
 */