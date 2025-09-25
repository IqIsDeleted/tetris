#include "test.h"

START_TEST(game_info_base_test) {
  GameInfo_t a = updateCurrentState();
  userInput(Start, 0);

  ck_assert_ptr_nonnull(a.field);
  ck_assert_ptr_nonnull(a.next);

  ck_assert_ptr_nonnull(a.field);
  ck_assert_ptr_nonnull(a.next);

  a = updateCurrentState();
  ck_assert_int_eq(a.level, 0);
  a = updateCurrentState();
  ck_assert_int_eq(a.speed, 1000);

  FILE *f = fopen("game/.tetris_highscore.dat", "rb+");
  int high_score = 0;
  if (f) {
    fread(&high_score, sizeof(int), 1, f);
    high_score ^= 0x8E19B7D4;
    fclose(f);
    ck_assert_int_eq(a.high_score, high_score);
  }

  userInput(Pause, 0);
  a = updateCurrentState();
  ck_assert_int_eq(a.pause, 1);
  userInput(Pause, 0);
  a = updateCurrentState();
  ck_assert_int_eq(a.pause, 0);

  updateCurrentState();
  userInput(Terminate, 0);
  a = updateCurrentState();
  ck_assert_ptr_null(a.field);
  ck_assert_ptr_null(a.next);
}
END_TEST

START_TEST(score_test) {
  GameInfo_t a = updateCurrentState();
  userInput(Start, 0);
  updateCurrentState();
  int previos_score = a.score;
  current_score(4);
  a = updateCurrentState();
  ck_assert_int_eq((a.score - 1500), previos_score);

  updateCurrentState();
  previos_score = a.score;
  current_score(3);
  a = updateCurrentState();
  ck_assert_int_eq((a.score - 700), previos_score);

  updateCurrentState();
  previos_score = a.score;
  current_score(2);
  a = updateCurrentState();
  ck_assert_int_eq((a.score - 300), previos_score);

  updateCurrentState();
  previos_score = a.score;
  current_score(1);
  a = updateCurrentState();
  ck_assert_int_eq((a.score - 100), previos_score);

  GameInfo_t *game_ptr = currentGameInfo();
  for (int i = 0; i < WIDTH - 4; ++i) {
    game_ptr->next[19][i] = FILLED_CELL;
  }
  figure_t *fg = updateFigurePosition();
  fg->block[0].i = HEIGHT - 1;
  fg->block[0].j = 6;
  fg->block[1].i = HEIGHT - 1;
  fg->block[1].j = 7;
  fg->block[2].i = HEIGHT - 1;
  fg->block[2].j = 8;
  fg->block[3].i = HEIGHT - 1;
  fg->block[3].j = 9;
  userInput(Down, 0);

  updateCurrentState();
  previos_score = a.score;
  a = updateCurrentState();
  ck_assert_int_eq((a.score - 100), previos_score);

  updateCurrentState();
  userInput(Terminate, 0);
  a = updateCurrentState();
  ck_assert_ptr_null(a.field);
  ck_assert_ptr_null(a.next);
}
END_TEST

START_TEST(level_test) {
  GameInfo_t a = updateCurrentState();
  userInput(Start, 0);

  int current_level = 10;
  GameInfo_t *game_ptr = currentGameInfo();
  game_ptr->score = 66000;

  a = updateCurrentState();
  ck_assert_int_eq(a.level, current_level);

  updateCurrentState();
  userInput(Terminate, 0);
  a = updateCurrentState();
  ck_assert_ptr_null(a.field);
  ck_assert_ptr_null(a.next);
}
END_TEST

START_TEST(incorrect_input_test) {
  userInput(Start, 0);
  updateCurrentState();
  figure_t *current_position = updateFigurePosition();
  figure_t previos_position = *current_position;

  userInput(56, 0);

  current_position = updateFigurePosition();

  for (int k = 0; k < 4; ++k) {
    ck_assert_int_eq(current_position->block[k].i, previos_position.block[k].i);
    ck_assert_int_eq(current_position->block[k].j, previos_position.block[k].j);
  }

  updateCurrentState();
  userInput(Terminate, 0);
  GameInfo_t a = updateCurrentState();
  ck_assert_ptr_null(a.field);
  ck_assert_ptr_null(a.next);
}
END_TEST

START_TEST(figure_fill_test) {
  static const FigureTemplate templates[] = {
      {'I', {{0, 4}, {0, 3}, {0, 5}, {0, 6}}},
      {'O', {{0, 4}, {0, 5}, {1, 4}, {1, 5}}},
      {'J', {{1, 4}, {0, 4}, {1, 5}, {1, 6}}},
      {'L', {{1, 5}, {0, 5}, {1, 4}, {1, 3}}},
      {'Z', {{0, 5}, {0, 4}, {1, 5}, {1, 6}}},
      {'T', {{1, 4}, {0, 4}, {1, 3}, {1, 5}}},
      {'S', {{0, 4}, {0, 5}, {1, 3}, {1, 4}}},
  };
  for (int i = 0; i < 7; ++i) {
    figure_t fg = figure_t_fill(templates[i].element);

    ck_assert_int_eq(fg.block[0].i, templates[i].block[0].i);
    ck_assert_int_eq(fg.block[0].j, templates[i].block[0].j);

    ck_assert_int_eq(fg.block[1].i, templates[i].block[1].i);
    ck_assert_int_eq(fg.block[1].j, templates[i].block[1].j);

    ck_assert_int_eq(fg.block[2].i, templates[i].block[2].i);
    ck_assert_int_eq(fg.block[2].j, templates[i].block[2].j);

    ck_assert_int_eq(fg.block[3].i, templates[i].block[3].i);
    ck_assert_int_eq(fg.block[3].j, templates[i].block[3].j);

    ck_assert_int_eq(fg.active, true);
  }
}
END_TEST

START_TEST(figure_right_move_test) {
  userInput(Start, 0);
  updateCurrentState();
  figure_t *fg = updateFigurePosition();

  block_t coord[4] = {0};
  coord[0] = fg->block[0];
  coord[1] = fg->block[1];
  coord[2] = fg->block[2];
  coord[3] = fg->block[3];
  userInput(Right, 0);

  for (int k = 0; k < 4; ++k) {
    ck_assert_int_eq(coord[k].i, fg->block[k].i);
    ck_assert_int_eq(coord[k].j + 1, fg->block[k].j);
  }

  updateCurrentState();
  userInput(Terminate, 0);
  GameInfo_t a = updateCurrentState();
  ck_assert_ptr_null(a.field);
  ck_assert_ptr_null(a.next);
}
END_TEST

START_TEST(figure_left_move_test) {
  userInput(Start, 0);
  updateCurrentState();
  figure_t *fg = updateFigurePosition();

  block_t coord[4] = {0};
  coord[0] = fg->block[0];
  coord[1] = fg->block[1];
  coord[2] = fg->block[2];
  coord[3] = fg->block[3];
  userInput(Left, 0);

  for (int k = 0; k < 4; ++k) {
    ck_assert_int_eq(coord[k].i, fg->block[k].i);
    ck_assert_int_eq(coord[k].j - 1, fg->block[k].j);
  }

  updateCurrentState();
  userInput(Terminate, 0);
  GameInfo_t a = updateCurrentState();
  ck_assert_ptr_null(a.field);
  ck_assert_ptr_null(a.next);
}
END_TEST

START_TEST(figure_down_move_test) {
  userInput(Start, 0);
  updateCurrentState();
  figure_t *fg = updateFigurePosition();

  ck_assert_int_eq(fg->active, true);

  block_t previos_crd[4] = {fg->block[0], fg->block[1], fg->block[2],
                            fg->block[3]};

  userInput(Down, 0);
  for (int k = 0; k < 4; ++k) {
    ck_assert_int_eq(fg->block[k].i, previos_crd[k].i + 1);
    ck_assert_int_eq(fg->block[k].j, previos_crd[k].j);
  }

  updateCurrentState();
  userInput(Terminate, 0);
  GameInfo_t a = updateCurrentState();
  ck_assert_ptr_null(a.field);
  ck_assert_ptr_null(a.next);
}
END_TEST

START_TEST(figure_up_move_test) {
  userInput(Start, 0);
  updateCurrentState();
  figure_t *fg = updateFigurePosition();

  block_t coord[4] = {0};
  coord[0] = fg->block[0];
  coord[1] = fg->block[1];
  coord[2] = fg->block[2];
  coord[3] = fg->block[3];
  userInput(Up, 0);

  for (int k = 0; k < 4; ++k) {
    ck_assert_int_eq(coord[k].i, fg->block[k].i);
    ck_assert_int_eq(coord[k].j, fg->block[k].j);
  }

  updateCurrentState();
  userInput(Terminate, 0);
  GameInfo_t a = updateCurrentState();
  ck_assert_ptr_null(a.field);
  ck_assert_ptr_null(a.next);
}
END_TEST

START_TEST(figure_rotate_test) {
  userInput(Start, 0);
  updateCurrentState();
  figure_t *fg = updateFigurePosition();
  updateCurrentState();

  userInput(Down, 0);
  updateCurrentState();

  block_t new_crd[4];
  block_t center = fg->block[0];

  if (fg->form != 'O') {
    for (int k = 0; k < 4; k++) {
      int di = fg->block[k].i - center.i;
      int dj = fg->block[k].j - center.j;

      new_crd[k].i = center.i + dj;
      new_crd[k].j = center.j - di;
    }
  } else {
    for (int k = 0; k < 4; ++k) {
      new_crd[k] = fg->block[k];
    }
  }

  userInput(Action, 0);
  updateCurrentState();
  for (int k = 0; k < 4; ++k) {
    ck_assert_int_eq(new_crd[k].j, fg->block[k].j);
    ck_assert_int_eq(new_crd[k].i, fg->block[k].i);
  }

  updateCurrentState();
  userInput(Terminate, 0);
  GameInfo_t a = updateCurrentState();
  ck_assert_ptr_null(a.field);
  ck_assert_ptr_null(a.next);
}
END_TEST

Suite *test_tetris() {
  Suite *s = suite_create("Tetris Test");
  TCase *tcase_core = tcase_create("Tetris Test");

  tcase_add_test(tcase_core, game_info_base_test);
  tcase_add_test(tcase_core, level_test);
  tcase_add_test(tcase_core, score_test);
  tcase_add_test(tcase_core, incorrect_input_test);
  tcase_add_test(tcase_core, figure_fill_test);
  tcase_add_test(tcase_core, figure_right_move_test);
  tcase_add_test(tcase_core, figure_left_move_test);
  tcase_add_test(tcase_core, figure_down_move_test);
  tcase_add_test(tcase_core, figure_up_move_test);

  tcase_add_test(tcase_core, figure_rotate_test);

  suite_add_tcase(s, tcase_core);

  return s;
}

/*
 * Copyright (c) 2024 Eugen <dqeefcrcf@gmail.com>
 * License: S21
 */
