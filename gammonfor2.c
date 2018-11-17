/*gammonfor2.c
 * The allows two players to play backgammon against each other.
 * by Gabe Strasburger
 */

#include "backgammon.h"
#include "stdio.h"
#include "stdlib.h"
#include "time.h"
#include "string.h"

int main(){
  struct board *B=newgame();
  int winner=0;
  int turn=1;
  int *dice=malloc(4*sizeof(int));
  for(int i=0;i<4;i++){
    dice[i]=0;
  }
  display(B);
  int d1;
  int d2;
  int doubles;
  while(winner==0){
    printf("\n\nTurn again, turn again, jiggity jig!\n\n");
    int change=1;
    for(int i=0;i<4;i++){
      if(dice[i]>0){
	change=0;
      }
    }
    //alternates the turn, rolls new dice
    if(change==1){
      turn=turn*-1;
      if(turn==-1){
	printf("%s","Black's turn!\n");
      }
      else{
	printf("%s","White's turn!\n");
      }
      srand(time(0));
      d1=(rand()%6)+1;
      d2=(rand()%6)+1;
      if(d1==d2){
	doubles=1;
	for(int j=0;j<4;j++){
	  dice[j]=d1;
	}
      }
      else{
	dice[0]=d1;
	dice[1]=d2;
	doubles=0;
      }
    }
    printf("%s","You have access to the following moves: ");
    for(int m=0;m<4;m++){
      if(dice[m]>0){
	printf("%i ",dice[m]);
      }
    }
    printf("\n");
    int cango=0;
    //this block determines whether any moves are possible
    if(turn==1){
      if(doubles==1){
	cango=whitecan(B,d1);
      }
      else if(whitecan(B,dice[0])==1 && whitecan(B,dice[1])==1){
	cango=1;
      }
    }
    else{
      if(doubles==1){
        cango=blackcan(B,d1);
      }
      else if(blackcan(B,dice[1])==1 && blackcan(B,dice[0])==1){
        cango=1;
      } 
    }
    //empties dice array when no moves are possible
    if(cango==1){
      for(int k=0;k<4;k++){
	dice[k]=0;
      }
      printf("%s","No possible moves.\n");
    }
    //takes input and makes move
    else{
      printf("Input move in the form of the space to move from followed by which die to use (sequentially), i.e. \"06 2\" (use b to refer to bar or q to quit):");
      
      int space = 0;
      int move=0;
      int alt=0;
      char c1=getchar();
      if(c1=='q'||c1=='Q'){
	exit(0);
      }
      if(c1=='s'||c1=='S'){
	saveboard(B, turn);
	printf("Board saved!\n");
	char c = getchar();
	while(c != '\n'){
	  c = getchar();
	}
	continue;
      }
      if(c1=='l'||c1=='L'){
	struct board * new = loadboard();
	turn = getturn();
	struct board * old = B;
	B = new;
	freeboard(old);
	char c = getchar();
	display(B);
	printf("Board loaded!\n");
	if(turn==1){
	  printf("It is now White's Turn\n");
	}
	else{
	  printf("It is now Black's Turn\n");
	}
	while(c != '\n'){
	  c = getchar();
	}
	continue;
      }
      char c2;
      if(c1!='b'&& c1!='B'){
	c2=getchar();
      }
      getchar();
      char c3=getchar();
      if(c1!='b'&& c1!='B'){
	printf("Space before is %d, c1 is %c, c2 is %c\n",space,c1,c2);
	space=(c1-'0')*10+(c2-'0');
	printf("Space after is %d\n",space);
      }	
      else{
	space=25;
      }
      if(doubles==1){
	move=dice[0];
      }
      else if(c3=='1'){
	move=dice[0];
      }
      else{
	move=dice[1];
      }
      if(doubles==0 && dice[1]!=0){
	alt=dice[1];
      }
      else{
	alt=-1;
      }
      int out;
      printf("Move: %d Space: %d", move, space);
      if(turn==1){
	out=whitemove(B,space,move,alt);
      }
      else{
	out=blackmove(B,space,move,alt);
      }
      if(out==0){
	display(B);
      }
      else if(out==1){
	printf("%s","Something was wrong in the inputs\n");
      }
      else if(out==2){
	printf("%s","You can't move to a space occupied by two or more of your opponents.\n");
      }
      else if(out==3){
	printf("%s","You can't move other pieces while you have pieces ont he bar.\n");
      }
      else if(out==4){
	printf("%s","You don't have any pieces at the space you specified.\n");
      }
      else if(out==5){
	printf("%s","You can’t move pieces off the board without all of your pieces in your inner table.\n");
      }
      else if(out==6){
        printf("%s","If you can use both dice, you must. You can’t use the first in such a way that it prevents you from moving the second.\n");
      }
      if(out==0){
	if(doubles==1){
	  if(dice[3]!=0){
	    dice[3]=0;
	  }
	  else if(dice[2]!=0){
	    dice[2]=0;
	  }
	  else if(dice[1]!=0){
	    dice[1]=0;
	  }
	  else{
	    dice[0]=0;
	  }
	}
	else if(c3-'0'==1){
	  dice[0]=dice[1];
	  dice[1]=0;
	}
	else{
	  dice[1]=0;
	}
      }
      if(B->woff==15){
	winner=1;
	printf("%s","White wins!");
      }
      if(B->boff==15){
	winner=1;
	printf("%s","Black wins!");
      }
    }
    while(getchar()!='\n'){
      continue;
    }
  }
}

