#ifndef TETRIS_BRICK_GAME_FRONT
#define TETRIS_BRICK_GAME_FRONT

#include <ncurses.h>
#include <stdio.h>

#include "../../brick_game/tetris/backend.h"

#define GAME_TERMINATE 27
#define GAME_START 10

#define ACTION_NONE -1
#define RIGHT_ARROW 261
#define LEFT_ARROW 260
#define DOWN_ARROW 258

#define LIMIT_COLS_UP 0
#define LIMIT_COLS_DOWN 18
#define LIMIT_ROWS_DOWN 20
#define RIGHT_MENU_FENCE 11
#define LIMIT_COLS_RIGHT 21
#define RIGHT_PANEL_START_COL 11

#define MAIN_TEXT_ROW 8

#define SCORE_ROW 0
#define NUM_SCORE_ROW 1
#define HIGH_SCORE_ROW 3
#define NUM_HIGH_SCORE_ROW 4
#define LEVEL_ROW 6
#define NUM_LEVEL_ROW 7
#define NEXT_FIGURE_TEXT_ROW 9
#define NEXT_FIGURE_ROW 10
#define SPEED_ROW 13
#define NUM_SPEED_ROW 14

void game_print();
void init_front();
UserAction_t menu();
void print_start_menu();
void new_print_fld(GameInfo_t game);
void print_right_panel(int i, int j, bool *print_flag, GameInfo_t game);
void next_figure_info_update(int mtx[2][6]);
void cleanup();
UserAction_t get_game_action();
void game_over_print();

#endif

/*
 * Copyright (c) 2024 Eugen <dqeefcrcf@gmail.com>
 * License: S21
 */
