#ifndef TETRIS_BRICK_GAME_BACK
#define TETRIS_BRICK_GAME_BACK

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "fsm.h"

/*
For field
*/

#define HEIGHT 20
#define WIDTH 10
#define FILLED_CELL 1

/*
For random figures
*/

#define ALL_MOD 3
#define UPDATE_HISTORY 2
#define CHECK 1
#define NUMBER_OF_FIGURES 7

#define SUCCESS 0
#define FAILURE 1

#define BASE_LEVEL_SCORE 600
#define MAX_LEVEL 10

typedef struct {
  unsigned short int i;
  unsigned short int j;
} block_t;

typedef struct {
  block_t block[4];
  bool active;
  char form;
} figure_t;

typedef struct {
  char element;
  block_t block[4];
} FigureTemplate;

typedef enum direction { Left_dir = -1, Down_dir, Right_dir } dir_t;

typedef enum UserAction {
  Start,
  Pause,
  Terminate,
  Left,
  Right,
  Up,
  Down,
  Action
} UserAction_t;

typedef struct GameInfo {
  int **field;
  int **next;
  int score;
  int high_score;
  int level;
  int speed;
  int pause;
} GameInfo_t;

void userInput(UserAction_t action, bool hold);
GameInfo_t updateCurrentState();

char figure_create();

GameInfo_t *currentGameInfo();
figure_t *updateFigurePosition();
figure_t *updateNextFigure();
bool can_move(int **mtx, figure_t fg, int dir);
void move_down(int **mtx);
void move_horizontal(int **mtx, int direction);
int check_full_line(int **field);
figure_t figure_t_fill(char el);
void figure_position_updating(int **mtx, figure_t fg, int new_cell);
void rotate_figure();
void current_score(int destroyed_line);
void game_terminate();
void update_high_score(int new_high_score);
void speed_process(GameInfo_t *game);
bool game_over(GameInfo_t *game);

#endif

/*
 * Copyright (c) 2024 Eugen <dqeefcrcf@gmail.com>
 * License: S21
 */