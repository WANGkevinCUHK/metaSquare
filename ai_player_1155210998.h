#ifndef __AlphaBeta__
#define __AlphaBeta__ 1

#define BoardSize 8
#define ALPHA -2147483647
#define BETA 2147483647
#define BLUE 1
#define RED 2
#define depth_1 5
#define depth_2 6
#define depth_3 7
#define depth_4 8
#define ind(x, y) (10 * (x) + (y))

struct gameState__ {
  int board[89];
  int redScore;
  int blueScore;
  int currentPlayer;
};
int inBound(int x, int y);
void init_void(struct gameState__* state);
int countVacancy(const int board[]);
void switchPlayer(struct gameState__* state);
int ai_player(int player, const int* board);
void init_para(struct gameState__* state, const int board[], int player);
int getMap(const int board[], int map[]);
int depth(int x, int layer);
double checkScoreForAi(struct gameState__* state, int move, int mode);
double globalScore(struct gameState__* state, int player);
double globalScoreOneShot(struct gameState__* state, int player);
double Evaluate(struct gameState__* state, int depth, int stk[]);
int oneShot(struct gameState__* state, int move);
double AlphaBeta(struct gameState__* state, int layer, int depth, int stk[],
                 int occupied[], double alpha, double beta, double time);
double AlphaBetaDEEP(struct gameState__* state, int layer, int depth, int stk[],
                     int occupied[], double alpha, double beta, double time,
                     int mapPos[], int len);

#endif
