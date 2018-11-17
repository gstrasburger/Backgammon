/* backgammon.h
 * The grandiloquent header file of the glorious backgammon program of the
 * effulgent Systems Programming class, year of our lord Two Thousand Eighteen
 */

struct board{
  int * spaces;
  int bbar;
  int wbar;
  int bout;
  int wout;
  int boff;
  int woff;
};

#ifndef BOARD
#define BOARD

struct board * newgame();
void copyboard(struct board *B, struct board *new);
void freeboard(struct board *B);
void saveboard(struct board *B, int turn);
struct board * loadboard();
int getturn();

#endif

#ifndef MOVE
#define MOVE

int whitemove(struct board * B, int space, int move, int alt);
int blackmove(struct board * B, int space, int move, int alt);

#endif

#ifndef ALT
#define ALT

int waltmove(struct board * B, int alt, int real);
int baltmove(struct board * B, int alt, int real);

#endif

#ifndef CAN
#define CAN

int whitecan(struct board * B, int move);
int blackcan(struct board * B, int move);

#endif

#ifndef DISPLAY
#define DISPLAY

void display(struct board * B);

#endif
