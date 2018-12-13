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
      int space=0;
      int move=0;
      int alt=0;
      /*
       *Hi Nina and/or Robbie!
       *This is where some of your code goes.
       *Things it should do:
       *Tell the user what dice they can use. At this point in the program, dice have already been rolled, and are stored in an array of 4 integers conveniently named dice. Entries in dice which contain 0's are either there just to store moves from doubles, or are dice that have already been used.
       *Set the above variable "space" to the initial space of the piece the player wants to move
       *Set the above variable "move" to the number of spaces the piece is to be moved
       *If the player has rolled 2 different dice and hasn't yet used one of them, it should set the above variable "alt" to the unused die. If they rolled doubles or they only have one unused die remaining, set alt to -1.
       *The next few comments also have things for you to do, I've begun them all with "--N&R"
       */	
      int out;
      printf("Move: %d Space: %d", move, space);
      if(turn==1){
	out=whitemove(B,space,move,alt);
      }
      else{
	out=blackmove(B,space,move,alt);
      }
      //--N&R If out is not 0, you should display the print statement associated with the output below.
      if(out==0){
	//--N&R Idk how you two want to handle displaying, but rn this is where our display function is happening. We just didn't want to make the player see the same board over and over again when they fucked up, so we only diplayed the board update when they successfully made a move.
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
      //--N&R Here you should take any dice that they used out of the dice array.
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

