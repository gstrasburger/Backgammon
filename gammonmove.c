/* gammonmove.c
 * the move functions for our backgammon game
 * by Gabe Strasburger
 */

#include "backgammon.h"
#include "stdio.h"
#include "stdlib.h"

//Moves white pieces. Space is the current space of the piece to be moved, move is the number of spaces to move it, and alt is the other move a player could make (if any). Returns 0 if it works, different positive returns represent different errors.
int whitemove(struct board * B, int space, int move, int alt){
  struct board *old=newgame();
  copyboard(B,old);
  if(move<1||move>6){
    freeboard(old);
    return 1;
  }
  //an input of 25 represents the bar
  if(space<1||space>25){
    freeboard(old);
    return 1;
  }
  if(space==25){
    //There are white pieces on the bar
    if(B->wbar>0){
      //There are 2 or more black pieces at the destination
      if(B->spaces[move]<-1){
	freeboard(old);
	return 2;
      }
      //There is exactly one black piece at the destination
      else if (B->spaces[move]==-1){
	B->spaces[move]=1;
	B->wbar--;
	B->bout++;
	B->bbar++;
	if(whitecan(B,alt)==1){
	  if(waltmove(old,alt,move)==1){
	    B->spaces[move]=-1;
	    B->wbar++;
	    B->bout--;
	    B->bbar--;
	    freeboard(old);
	    return 6;
	  }
	}
	freeboard(old);
	return 0;
      }
      //destination is empty or contains white pieces
      else{
	B->spaces[move]++;
	B->wbar--;
	if(whitecan(B,alt)==1){
          if(waltmove(old,alt,move)==1){
            B->spaces[move]--;
            B->wbar++;
	    freeboard(old);
            return 6;
          }
        }
	freeboard(old);
	return 0;
      }
    }
    //There are no white pieces on the bar
    else{
      freeboard(old);
      return 4;
    }
  }
  else if (B->wbar>0){
    freeboard(old);
    return 3;
  }
  if (B->spaces[space]<1){
    freeboard(old);
    return 4;
  }
  int dest=space+move;
  if(dest>24){
    if(B->wout>0){
      freeboard(old);
      return 5;
    }
    else{
      B->spaces[space]--;
      B->woff++;
      if(whitecan(B,alt)==1){
	if (waltmove(old,alt,move)==1){
	  B->spaces[space]++;
	  B->woff--;
	  freeboard(old);
	  return 6;
	}
      }
      freeboard(old);
      return 0;
    }
  }
  if(B->spaces[dest]<-1){
    freeboard(old);
    return 2;
  }
  int hit=0;
  if(B->spaces[dest]==-1){
    hit=1;
    B->bbar++;
    B->spaces[dest]=1;
    if(dest<7){
      B->bout++;
      }
  }
  else{
    B->spaces[dest]++;
  }
  if(dest>18){
    B->wout--;
  }
  B->spaces[space]--;
  if(whitecan(B,alt)==1){
    if(waltmove(old,alt,move)==1){
      B->spaces[space]++;
      B->spaces[dest]--;
      if(hit==1){
	B->spaces[dest]++;
	B->bbar--;
	if(dest<7){
	    B->bout--;
	}
      }
      if(dest>18){
	B->wout++;
      }
    }
    freeboard(old);
    return 6;
  }
  freeboard(old);
  return 0;
}

int blackmove(struct board * B, int space, int move, int alt){
  struct board *old=newgame();
  copyboard(B,old);
  if(move<1||move>6){
    freeboard(old);
    return 1;
  }
  //an input of 25 represents the bar
  if(space<1||space>25){
    freeboard(old);
    return 1;
  }
  if(space==25){
    //There are black pieces on the bar
    if(B->bbar>0){
      //There are 2 or more black pieces at the destination
      if(B->spaces[25-move]>1){
	freeboard(old);
	return 2;
      }
      //There is exactly one black piece at the destination
      else if (B->spaces[25-move]==1){
	B->spaces[25-move]=-1;
	B->bbar--;
	B->wout++;
	B->wbar++;
	if(blackcan(B,alt)==1){
	  if(baltmove(old,alt,move)==1){
	    B->spaces[25-move]=1;
	    B->bbar++;
	    B->wout--;
	    B->wbar--;
	    freeboard(old);
	    return 6;
	  }
	}
	freeboard(old);
	return 0;
      }
      //destination is empty or contains black pieces
      else{
	B->spaces[25-move]--;
	B->bbar--;
	if(blackcan(B,alt)==1){
          if(baltmove(old,alt,move)==1){
            B->spaces[25-move]++;
            B->bbar++;
	    freeboard(old);
            return 6;
          }
        }
	freeboard(old);
	return 0;
      }
    }
    //There are no white pieces on the bar
    else{
      freeboard(old);
      return 4;
    }
  }
  else if (B->bbar>0){
    freeboard(old);
    return 3;
  }
  if (B->spaces[space]>-1){
    freeboard(old);
    return 4;
  }
  int dest=space-move;
  if(dest<1){
    if(B->bout>0){
      freeboard(old);
      return 5;
    }
    else{
      B->spaces[space]++;
      B->boff++;
      if(blackcan(B,alt)==1){
	if (baltmove(old,alt,move)==1){
	  B->spaces[space]--;
	  B->boff--;
	  freeboard(old);
	  return 6;
	}
      }
      freeboard(old);
      return 0;
    }
  }
  if(B->spaces[dest]>1){
    freeboard(old);
    return 2;
  }
  int hit=0;
  if(B->spaces[dest]==1){
    hit=1;
    B->wbar++;
    B->spaces[dest]=-1;
    if(dest>18){
      B->wout++;
    }
  }
  else{
    B->spaces[dest]--;
  }
  if(dest<7){
    B->bout--;
  }
  B->spaces[space]++;
  if(blackcan(B,alt)==1){
    if(baltmove(old,alt,move)==1){
      B->spaces[space]--;
      B->spaces[dest]++;
      if(hit==1){
	B->spaces[dest]++;
	B->wbar--;
	if(dest>18){
	  B->wout--;
	}
      }
      if(dest<7){
	B->bout++;
      }
    }
    freeboard(old);
    return 6;
  }
  freeboard(old);
  return 0;
}
