#include "front.h"

void print_right_panel(int i, int j, bool *print_flag, GameInfo_t game) {
  int next[2][6] = {0};

  switch (i) {
    case SCORE_ROW:
      if (print_flag[0]) {
        printw("%-12s", "Score:");
      }
      print_flag[0] = false;
      break;

    case NUM_SCORE_ROW:
      if (print_flag[1]) {
        printw("%-12d", game.score);
      }
      print_flag[1] = false;
      break;

    case HIGH_SCORE_ROW:
      if (print_flag[2]) {
        printw("%-12s", "High score:");
      }
      print_flag[2] = false;
      break;

    case NUM_HIGH_SCORE_ROW:
      if (print_flag[3]) {
        printw("%-12d", game.high_score);
      }
      print_flag[3] = false;
      break;

    case LEVEL_ROW:
      if (print_flag[4]) {
        printw("%-12s", "Level:");
      }
      print_flag[4] = false;
      break;

    case NUM_LEVEL_ROW:
      if (print_flag[5]) {
        printw("%-12d", game.level);
      }
      print_flag[5] = false;
      break;

    case NEXT_FIGURE_TEXT_ROW:
      if (print_flag[6]) {
        printw("%-12s", "Next figure:");
      }
      print_flag[6] = false;
      break;

    case NEXT_FIGURE_ROW:
      next_figure_info_update(next);
      printw("%c ", next[0][j - 12] ? '*' : ' ');
      break;

    case NEXT_FIGURE_ROW + 1:
      next_figure_info_update(next);
      printw("%c ", next[1][j - 12] ? '*' : ' ');
      break;

    case SPEED_ROW:
      if (print_flag[7]) {
        printw("%-12s", "Speed:");
      }
      print_flag[7] = false;
      break;

    case NUM_SPEED_ROW:
      if (print_flag[8]) {
        printw("%-12d", game.speed);
      }
      print_flag[8] = false;
      break;

    default:
      printw("  ");
      break;
  }
}

void new_print_fld(GameInfo_t game) {
  clear();

  game.pause ? printw("# # # #  PAUSE  # # # # ")
             : printw("# # # # # # # # # # # # ");
  printw("# # # # # # #\n");

  for (int i = 0; i < LIMIT_COLS_RIGHT; ++i) {
    bool print_flag[9] = {true, true, true, true, true, true, true, true, true};

    for (int j = 0; j < 19; ++j) {
      if (j == LIMIT_COLS_UP || j == LIMIT_COLS_DOWN || i == LIMIT_ROWS_DOWN ||
          j == RIGHT_MENU_FENCE) {
        printw("# ");

      } else if (j > RIGHT_PANEL_START_COL) {
        print_right_panel(i, j, print_flag, game);

      } else {
        printw("%c ", game.field[i][j - 1] ? '*' : ' ');
      }
    }

    printw("\n");
  }

  refresh();
}

void print_start_menu() {
  bool print_flag[4] = {1, 1, 1, 1};

  for (int i = 0; i < HEIGHT + 2; ++i) {
    for (int j = 0; j < WIDTH + 2; ++j) {
      if (i == 0 || i == LIMIT_COLS_RIGHT || j == 0 || j == 11) {
        printw("# ");

      } else if (i == MAIN_TEXT_ROW) {
        print_flag[0] ? printw("%-20s", "  For start") : 0;
        print_flag[0] = false;

      } else if (i == MAIN_TEXT_ROW + 1) {
        print_flag[1] ? printw("%-20s", "      press Enter.") : 0;
        print_flag[1] = false;

      } else if (i == MAIN_TEXT_ROW + 2) {
        print_flag[2] ? printw("%-20s", "  For quit") : 0;
        print_flag[2] = false;

      } else if (i == MAIN_TEXT_ROW + 3) {
        print_flag[3] ? printw("%-20s", "      press Esc.") : 0;
        print_flag[3] = false;

      } else {
        printw("  ");
      }
    }

    printw("\n");
  }
}

void game_over_print() {
  int action = -1;

  while (action != Terminate) {
    bool print_flag[2] = {1, 1};
    clear();

    for (int i = 0; i < HEIGHT + 2; ++i) {
      for (int j = 0; j < WIDTH + 2; ++j) {
        if (i == 0 || i == LIMIT_COLS_RIGHT || j == 0 || j == 11) {
          printw("# ");

        } else if (i == MAIN_TEXT_ROW) {
          print_flag[0] ? printw("%-20s", "     Game Over") : 0;
          print_flag[0] = false;

        } else if (i == MAIN_TEXT_ROW + 1) {
          print_flag[1] ? printw("%-20s", " Press Esc for exit") : 0;
          print_flag[1] = false;

        } else {
          printw("  ");
        }
      }

      printw("\n");
    }

    action = get_game_action();

    refresh();
  }
}

UserAction_t menu() {
  UserAction_t ret = ERR;

  while (ret != Start && ret != Terminate) {
    clear();

    print_start_menu();

    ret = get_game_action();

    refresh();
  }

  return ret;
}

void game_print() {
  bool exit = false;

  while (!exit) {
    GameInfo_t game = updateCurrentState();

    if (game.field == NULL) {
      exit = true;

    } else {
      new_print_fld(game);

      get_game_action();
    }
  }
}

int main() {
  init_front();

  if (menu() == Start) {
    game_print();
    game_over_print();
  }

  cleanup();

  return 0;
}

/*
 * Copyright (c) 2024 Eugen <dqeefcrcf@gmail.com>
 * License: S21
 */
