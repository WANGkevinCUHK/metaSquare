#include "ai_player_1155210998.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define eta 0.2
#define max(x, y) ((x) > (y) ? (x) : (y))
#define min(x, y) ((x) > (y) ? (y) : (x))

int map[2][9] = {0, 4, 5, 6, 3, 2, 7, 8, 1, 0, 1, 8, 7, 2, 3, 6, 5, 4};

int inBound(int x, int y) {
  if (x >= 1 && y <= BoardSize && x <= BoardSize && y >= 1)
    return 1;
  else
    return 0;
}

void init_void(struct gameState__* state) {
  for (int i = 1; i <= BoardSize; i++) {
    for (int j = 1; j <= BoardSize; j++) {
      state->board[ind(i, j)] = 0;
    }
  }
  state->redScore = 0;
  state->blueScore = 0;
  state->currentPlayer = BLUE;
}

void init_para(struct gameState__* state, const int board[], int player) {
  for (int i = 1; i <= BoardSize; i++) {
    for (int j = 1; j <= BoardSize; j++) {
      state->board[ind(i, j)] = board[ind(i, j)];
    }
  }
  state->redScore = 0;
  state->blueScore = 0;
  state->currentPlayer = player;
}

int countVacancy(const int board[]) {
  int count = 0;
  for (int x = 1; x <= BoardSize; x++) {
    for (int y = 1; y <= BoardSize; y++) {
      if (board[ind(x, y)] == 0) {
        count++;
      }
    }
  }
  return count;
}

int getMap(const int board[], int mapPos[]) {
  int count = 0;
  for (int x = 1; x <= BoardSize; x++) {
    for (int y = 1; y <= BoardSize; y++) {
      if (board[ind(map[0][x], map[1][y])] == 0) {
        count++;
        mapPos[count] = ind(map[0][x], map[1][y]);
      }
    }
  }
  return count;
}

int depth(int x, int layer) {
  if (x >= layer) return layer;
  return x;
}

void switchPlayer(struct gameState__* state) {
  if (state->currentPlayer == RED)
    state->currentPlayer = BLUE;
  else
    state->currentPlayer = RED;
}

double checkScoreForAi(struct gameState__* state, int move, int mode) {
  int row = move / 10;
  int col = move % 10;
  int player = state->currentPlayer;
  double score = 0;
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
        if (state->board[t1] == 3 - player || state->board[t2] == 3 - player) {
          number = 0;
        }
        if (number == 2 && abs(dx) + abs(dy) >= 1) {
          score +=
              (abs(dx) + abs(dy) + 1) * (abs(dx) + abs(dy) + 1) / (double)4;
        } else if (number == 1 && abs(dx) + abs(dy) >= 1) {
          score += mode * eta * (abs(dx) + abs(dy) + 1) *
                   (abs(dx) + abs(dy) + 1) / (double)2;
        }
      }
    }
  }
  return score;
}

double globalScore(struct gameState__* state, int player) {
  double score = 0;
  state->currentPlayer = player;
  for (int i = 1; i <= BoardSize; i++) {
    for (int j = 1; j <= BoardSize; j++) {
      if (state->board[ind(i, j)] == player) {
        score += checkScoreForAi(state, ind(i, j), 1);
      }
    }
  }
  return score;
}

double globalScoreOneShot(struct gameState__* state, int player) {
  double score = 0;
  state->currentPlayer = player;
  for (int i = 1; i <= BoardSize; i++) {
    for (int j = 1; j <= BoardSize; j++) {
      if (state->board[ind(i, j)] == player) {
        score += checkScoreForAi(state, ind(i, j), 0);
      }
    }
  }
  return score;
}

double Evaluate(struct gameState__* state, int depth, int stk[]) {
  struct gameState__ currentState = *state;
  int player = state->currentPlayer;
  double zeta = 0;
  if (player == RED)
    zeta = 1.2;
  else
    zeta = 1.1;
  for (int i = 1; i <= depth - 1; i++) {
    currentState.board[stk[i]] = currentState.currentPlayer;
    switchPlayer(&currentState);
  }
  return zeta * globalScore(&currentState, player) -
         globalScore(&currentState, 3 - player);
}

int oneShot(struct gameState__* state, int move) {
  struct gameState__ currentState = *state;
  int player = state->currentPlayer;
  currentState.board[move] = player;
  int scoreCurrent = globalScoreOneShot(&currentState, player);
  int scoreAnother = globalScoreOneShot(&currentState, 3 - player);
  if (scoreCurrent > 150 && scoreCurrent - scoreAnother >= 15) return 1;
  return 0;
}

double AlphaBeta(struct gameState__* state, int layer, int depth, int stk[],
                 int occupied[], double alpha, double beta, double time) {
  if ((double)(clock() - time) / CLOCKS_PER_SEC > 4.75 && layer != 1) {
    if (layer % 2 == 1)
      return BETA;
    else
      return ALPHA;
  }

  if (depth == 1) {
    for (int x = 1; x <= BoardSize; x++) {
      for (int y = 1; y <= BoardSize; y++) {
        if (state->board[ind(x, y)] == 0) {
          return ind(x, y);
        }
      }
    }
  }

  if (layer == depth) {
    return Evaluate(state, depth, stk);
  }

  if (layer == 1) {
    double maxScore = ALPHA;
    int ans = 0;
    for (int x = 1; x <= BoardSize; x++) {
      for (int y = 1; y <= BoardSize; y++) {
        if (state->board[ind(map[0][x], map[1][y])] == 0 &&
            occupied[ind(map[0][x], map[1][y])] == 0) {
          if (oneShot(state, ind(map[0][x], map[1][y]))) {
            return ind(map[0][x], map[1][y]);
          }
          stk[layer] = ind(map[0][x], map[1][y]);
          occupied[ind(map[0][x], map[1][y])] = 1;
          double tempScore = AlphaBeta(state, layer + 1, depth, stk, occupied,
                                       alpha, beta, time);
          if (tempScore > maxScore) {
            ans = ind(map[0][x], map[1][y]);
            maxScore = tempScore;
            alpha = max(alpha, maxScore);
          }
          occupied[ind(map[0][x], map[1][y])] = 0;
        }
        double now = clock();
        if ((double)(clock() - time) / CLOCKS_PER_SEC > 4.75) break;
      }
      double now = clock();
      if ((double)(clock() - time) / CLOCKS_PER_SEC > 4.75) break;
    }
    return ans;
  }

  if (layer % 2 == 1) {
    double maxScore = ALPHA;
    for (int x = 1; x <= BoardSize; x++) {
      for (int y = 1; y <= BoardSize; y++) {
        if (state->board[ind(x, y)] == 0 && occupied[ind(x, y)] == 0) {
          stk[layer] = ind(x, y);
          occupied[ind(x, y)] = 1;
          double tempScore = AlphaBeta(state, layer + 1, depth, stk, occupied,
                                       alpha, beta, time);
          maxScore = max(tempScore, maxScore);
          alpha = max(alpha, maxScore);
          occupied[ind(x, y)] = 0;
        }
        if (beta <= alpha) break;
      }
      if (beta <= alpha) break;
    }
    return maxScore;
  }

  else {
    double minScore = BETA;
    for (int x = 1; x <= BoardSize; x++) {
      for (int y = 1; y <= BoardSize; y++) {
        if (state->board[ind(x, y)] == 0 && occupied[ind(x, y)] == 0) {
          stk[layer] = ind(x, y);
          occupied[ind(x, y)] = 1;
          double tempScore = AlphaBeta(state, layer + 1, depth, stk, occupied,
                                       alpha, beta, time);
          minScore = min(tempScore, minScore);
          beta = min(beta, minScore);
          occupied[ind(x, y)] = 0;
        }
        if (beta <= alpha) break;
      }
      if (beta <= alpha) break;
    }
    return minScore;
  }
}

double AlphaBetaDEEP(struct gameState__* state, int layer, int depth, int stk[],
                     int occupied[], double alpha, double beta, double time,
                     int mapPos[], int len) {
  if ((double)(clock() - time) / CLOCKS_PER_SEC > 4.75 && layer != 1) {
    if (layer % 2 == 1)
      return BETA;
    else
      return ALPHA;
  }

  if (depth == 1) {
    for (int x = 1; x <= BoardSize; x++) {
      for (int y = 1; y <= BoardSize; y++) {
        if (state->board[ind(x, y)] == 0) {
          return ind(x, y);
        }
      }
    }
  }

  if (layer == depth) {
    return Evaluate(state, depth, stk);
  }

  if (layer == 1) {
    double maxScore = ALPHA;
    int ans = 0;
    int x = 0;
    for (x = 1; x <= len; x++) {
      if (occupied[mapPos[x]] == 0) {
        if (oneShot(state, mapPos[x])) {
          return mapPos[x];
        }
        stk[layer] = mapPos[x];
        occupied[mapPos[x]] = 1;
        double tempScore = AlphaBetaDEEP(state, layer + 1, depth, stk, occupied,
                                         alpha, beta, time, mapPos, len);
        if (tempScore > maxScore) {
          ans = mapPos[x];
          maxScore = tempScore;
          alpha = max(alpha, maxScore);
        }
        occupied[mapPos[x]] = 0;
      }
      double now = clock();
      if ((double)(clock() - time) / CLOCKS_PER_SEC > 4.75) break;
    }
    return ans;
  }

  if (layer % 2 == 1) {
    double maxScore = ALPHA;
    for (int x = 1; x <= len; x++) {
      if (occupied[mapPos[x]] == 0) {
        stk[layer] = mapPos[x];
        occupied[mapPos[x]] = 1;
        double tempScore = AlphaBetaDEEP(state, layer + 1, depth, stk, occupied,
                                         alpha, beta, time, mapPos, len);
        maxScore = max(tempScore, maxScore);
        alpha = max(alpha, maxScore);
        occupied[mapPos[x]] = 0;
      }
      if (beta <= alpha) break;
    }
    return maxScore;
  }

  else {
    double minScore = BETA;
    for (int x = 1; x <= len; x++) {
      if (occupied[mapPos[x]] == 0) {
        stk[layer] = mapPos[x];
        occupied[mapPos[x]] = 1;
        double tempScore = AlphaBetaDEEP(state, layer + 1, depth, stk, occupied,
                                         alpha, beta, time, mapPos, len);
        minScore = min(tempScore, minScore);
        beta = min(beta, minScore);
        occupied[mapPos[x]] = 0;
      }
      if (beta <= alpha) break;
    }

    return minScore;
  }
}

int ai_player(int player, const int* board) {
  struct gameState__ nowState;
  init_para(&nowState, board, player);
  int stk[105] = {0};
  int occupied[105] = {0};
  int ans = 0;
  if (countVacancy(board) >= 32) {
    ans = AlphaBeta(&nowState, 1, depth(countVacancy(board), depth_1), stk,
                    occupied, ALPHA, BETA, clock());
  } else if (countVacancy(board) >= 16) {
    int mapPos[105] = {0};
    int len = getMap(board, mapPos);
    ans = AlphaBetaDEEP(&nowState, 1, depth(countVacancy(board), depth_2), stk,
                        occupied, ALPHA, BETA, clock(), mapPos, len);
  } else if (countVacancy(board) >= 8) {
    int mapPos[105] = {0};
    int len = getMap(board, mapPos);
    ans = AlphaBetaDEEP(&nowState, 1, depth(countVacancy(board), depth_3), stk,
                        occupied, ALPHA, BETA, clock(), mapPos, len);
  } else {
    int mapPos[105] = {0};
    int len = getMap(board, mapPos);
    ans = AlphaBetaDEEP(&nowState, 1, depth(countVacancy(board), depth_4), stk,
                        occupied, ALPHA, BETA, clock(), mapPos, len);
  }
  return ans;
}