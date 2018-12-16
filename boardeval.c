/*The function which evaluates the value of the board state
 */

#include "backgammon.h"
#include <stdio.h>
#include <stdlib.h>

static double inbonus = 0.1;
static double offval = 25.5;
static double expected=8.16;

double WbarVal(struct board * b){
  double P=0;
  for(int i=1;i<7;i++){
    for(int j=1;j<7;j++){
      if(b->spaces[i]<-1 && b->spaces[j]<-1){
	P=P+1;
      }
    }
  }
  P=P/36;
  return -1*((expected*P)+(expected*P*P));
}

double BbarVal(struct board * b){
  double P=0;
  for(int i=1;i<7;i++){
    for(int j=1;j<7;j++){
      if(b->spaces[25-i]>1 && b->spaces[25-j]>1){
        P=P+1;
      } 
    }
  }
  P=P/36;
  return ((expected*P)+(expected*P*P));
}

double BoardVal(struct board * b){
  double agg=0;
  for(int i=1;i<25;i++){
    if(b->spaces[i]>0){
      if(i>18){
	agg=agg+((i+inbonus)*b->spaces[i]);
      }
      else{
	agg=agg+(i*b->spaces[i]);
      }
    }
    else if(b->spaces[i]<0){
      if(i<7){
	agg=agg+((i+inbonus)*b->spaces[i]);
      } 
      else{
	agg=agg+(i*b->spaces[i]);
      } 
    }
  }
  agg=agg+(b->woff*offval);
  agg=agg-(b->boff*offval);
  agg=agg+(b->bbar*BbarVal(b));
  agg=agg+(b->bbar*WbarVal(b));
  return agg;
}
