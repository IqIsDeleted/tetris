#include "test.h"

int main() {
  Suite *s = test_tetris();
  SRunner *sr = srunner_create(s);
  srunner_run_all(sr, CK_NORMAL);
  srunner_free(sr);

  return 0;
}

/*
 * Copyright (c) 2024 Eugen <dqeefcrcf@gmail.com>
 * License: S21
 */
