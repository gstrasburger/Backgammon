#include "backgammon.h"
#include <stdio.h>
 
int main(){
  struct board * b=newgame();
  int d[4]={4,6,0,0};
  printf("Building mmnode\n");
  struct mmNode * t=mmBuild(1,d,b,1,0,1);
  printf("Running Triesearch\n");
  int * moves=TrieSearch(t);
  for(int i=0;i<8;i++){
    printf("%i\n",moves[i]);
  }
}
