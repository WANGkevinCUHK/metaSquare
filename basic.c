#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "ai_player_1155210998.h"

void displayState(struct gameState__ *);
int askForMove(struct gameState__ *);
int checkStepScore(struct gameState__ *, int);
void updateGameState(struct gameState__ *, int, int);
int checkEnd(struct gameState__ *);
int displayWinner(struct gameState__ *);

int main() {
  struct gameState__ game;
  int mode, isValid;
  init_void(&game);
  printf(
      "Choose the game mode\n(1. Human v/s Human    2. Human v/s "
      "Computer):\nYou "
      "choose:");

  do {
    scanf("%d", &mode);
    if (mode == 1) {
      isValid = 1;
      while (1) {
        displayState(&game);
        int move = askForMove(&game);
        if (move == -1) break;
        updateGameState(&game, move, checkStepScore(&game, move));

        if (checkEnd(&game)) {
          displayWinner(&game);
          break;
        }
      }
    }

    else if (mode == 2) {
      isValid = 1;
      while (1) {
        displayState(&game);

        if (game.currentPlayer == BLUE) {
          int move = askForMove(&game);
          if (move == -1) break;
          updateGameState(&game, move, checkStepScore(&game, move));
        }

        else {
          int move = ai_player(2, game.board);
          printf("2(O) to move: RED move to:%d.\n", move);
          updateGameState(&game, move, checkStepScore(&game, move));
        }

        if (checkEnd(&game)) {
          displayWinner(&game);
          break;
        }
      }
    }

    else {
      isValid = 0;
      printf("Invalid game mode, please enter a valid game mode (1/2): ");
    }

  } while (!isValid);
  return 0;
}

void tellPlayer(int player) {
  if (player == 1) {
    printf("1(#) to move: BLUE move to:");
  } else if (player == 2) {
    printf("2(O) to move: RED move to:");
  }
}

void printfDISC(int disc) {
  if (disc == 0) {
    printf(". ");
  } else if (disc == 1) {
    printf("# ");
  } else {
    printf("O ");
  }
}

void displayBoard(struct gameState__ *state) {
  printf("   1 2 3 4 5 6 7 8\n");
  for (int i = 1; i <= BoardSize; i++) {
    printf("%d  ", i);
    for (int j = 1; j <= BoardSize; j++) {
      printfDISC(state->board[ind(i, j)]);
    }
    printf("\n");
  }
}

void displayState(struct gameState__ *state) {
  if (state->currentPlayer == 1)
    printf("This is BLUE's turn\n");
  else
    printf("This is RED's turn\n");

  printf("[Blue's Score = %d ; Red's Score = %d ]\n", state->blueScore,
         state->redScore);
  displayBoard(state);
}

int isValidMove(int move) {
  int row = move / 10;
  int col = move % 10;
  return inBound(row, col);
}

int askForMove(struct gameState__ *state) {
  tellPlayer(state->currentPlayer);
  int move, isValid;
  do {
    if (getchar() == EOF) return -1;
    scanf("%d", &move);
    if (state->board[move] == 0 && isValidMove(move))
      isValid = 1;

    else if (!isValidMove(move)) {
      isValid = 0;
      printf(
          "Input out of the game board, please "
          "input again (in [11, 88]): ");
    }

    else {
      isValid = 0;
      printf(
          "Input invalid, the provided location is not "
          "empty, please input again: ");
    }

  } while (!isValid);
  return move;
}

int checkEnd(struct gameState__ *state) {
  if (state->redScore > 150 || state->blueScore > 150) {
    if (abs(state->redScore - state->blueScore) >= 15) {
      return 1;
    }
  }
  int count = countVacancy(state->board);
  if (count > 0)
    return 0;
  else
    return 1;
}

int checkStepScore(struct gameState__ *state, int move) {
  int row = move / 10;
  int col = move % 10;
  int player = state->currentPlayer;
  int score = 0;
  for (int i = 1; i <= BoardSize; i++) {
    for (int j = 1; j <= BoardSize; j++) {
      if (state->board[ind(i, j)] == player) {
        int dx = j - col;
        int dy = i - row;
        if (!inBound(row - dx, col + dy) || !inBound(i - dx, j + dy)) continue;
        int t1 = ind(row - dx, col + dy);
        int t2 = ind(i - dx, j + dy);
        int number = 0;
        if (state->board[t1] == player) {
          number += 1;
        }
        if (state->board[t2] == player) {
          number += 1;
        }
        if (number == 2 && abs(dx) + abs(dy) >= 1) {
          if (state->currentPlayer == RED) {
            printf(
                "RED gains %d more points by formulating the square "
                "{%d,%d,%d,%d"
                "}\n",
                (abs(dx) + abs(dy) + 1) * (abs(dx) + abs(dy) + 1), move,
                ind(i, j), ind(row - dx, col + dy), ind(i - dx, j + dy));
          } else {
            printf(
                "Blue gains %d more points by formulating the square "
                "{%d,%d,%d,%d"
                "}\n",
                (abs(dx) + abs(dy) + 1) * (abs(dx) + abs(dy) + 1), move,
                ind(i, j), ind(row - dx, col + dy), ind(i - dx, j + dy));
          }

          score += (abs(dx) + abs(dy) + 1) * (abs(dx) + abs(dy) + 1);
        }
      }
    }
  }
  return score;
}

void updateGameState(struct gameState__ *state, int move, int score) {
  state->board[move] = state->currentPlayer;
  if (state->currentPlayer == RED) {
    state->redScore += score;
  } else {
    state->blueScore += score;
  }
  switchPlayer(state);
}

int displayWinner(struct gameState__ *state) {
  displayState(state);
  printf("GAME OVER\n");
  if (state->redScore > state->blueScore) {
    printf("RED WINS with a score of %d\n", state->redScore);
    return -1;
  } else if (state->blueScore > state->redScore) {
    printf("BLUE WINS with a score of %d\n", state->blueScore);
    return 1;
  } else {
    printf("It is a DRAW\n");
    return 0;
  }
}

/**
 * ESTR 1002 Problem Solving by Programming
 *
 * Course Project
 *
 * I declare that the assignment here submitted is original
 * except for source material explicitly acknowledged,
 * and that the same or closely related material has not been
 * previously submitted for another course.
 * I also acknowledge that I am aware of University policy and
 * regulations on honesty in academic work, and of the disciplinary
 * guidelines and procedures applicable to breaches of such
 * policy and regulations, as contained in the website.
 *
 * University Guideline on Academic Honesty:
 * http://www.cuhk.edu.hk/policy/academichonesty/
 *
 * Student Name :WANG Qunzhong
 * Student ID : 1155210998
 * Date : 29/11/2023
 */