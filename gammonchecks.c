/* gammonchecks.c
 * Functions that perform some checks about the possibility of moves
 * By Gabe Strasburger
 */

#include "backgammon.h"
#include "stdio.h"
#include "stdlib.h"

int whitecan(struct board *B, int move){
  printf("call to wcan\n");
  if(move==0){
    return 1;
  }
  if(move==-1){
    return 0;
  }
  if(B->wbar>0){
    if(B->spaces[move]>-2){
      return 0;
    }
    else{
      return 1;
    }
  }
  for(int i=1;i<25;i++){
    if(B->spaces[i]>0){
      if((i+move)>24 && B->wout==0){
	return 0;
      }
      else if((i+move)<25 && B->spaces[i+move]>-2){
	return 0;
      }
    }
  }
  return 1;
}

int waltmove(struct board *B, int alt, int real){
  printf("call to waltmove\n");
  struct board *sim=newgame();
  for(int i=1;i<25;i++){
    copyboard(B,sim);
    if(B->spaces[i]>0){
      if(whitemove(sim,i,alt,-1)==0){
	if(whitecan(sim,real)){
	  freeboard(sim);
	  return 1;
	}
      }
    } 
  }
  if(B->wbar>0){
    copyboard(B,sim);
    if(whitemove(sim,25,alt,-1)==0){
      if(whitecan(sim,real)){
	freeboard(sim);
	return 1;
      }
    }
  }
  freeboard(sim);
  return 0;
}

int blackcan(struct board *B, int move){
  printf("call to bcan\n");
  if(move==0){
    return 1;
  }
  if(move==-1){
    return 0;
  }
  if(B->bbar>0){
    if(B->spaces[25-move]<2){
      return 0;
    }
    else{
      return 1;
    }
  }
  for(int i=1;i<25;i++){
    if(B->spaces[i]<0){
      if((i-move)<1 && B->bout==0){
	return 0;
      }
      else if((i-move)<25 && B->spaces[i-move]<2){
	return 0;
      }
    }
  }
  return 1;
}

int baltmove(struct board *B, int alt, int real){
  printf("call to baltmove\n");
  struct board *sim=newgame();
  for(int i=1;i<25;i++){
    copyboard(B,sim);
    if(B->spaces[i]<0){
      if(blackmove(sim,i,alt,-1)==0){
	if(blackcan(sim,real)){
	  freeboard(sim);
	  return 1;
	}
      }
    } 
  }
  if(B->bbar>0){
    copyboard(B,sim);
    if(blackmove(sim,25,alt,-1)==0){
      if(blackcan(sim,real)){
	freeboard(sim);
	return 1;
      }
    }
  }
  freeboard(sim);
  return 0;
}
