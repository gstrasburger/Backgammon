/* gammonboard.c
 * defines the board struct and includes the board constructing function
 * Gabe Strasburger
 */

#include "backgammon.h"
#include "stdio.h"
#include "stdlib.h"

struct board * newgame(){
  struct board * B = malloc(sizeof(struct board));
  int size=25*sizeof(int);
  B->spaces = malloc(size);
  for(int i=0;i<24;i++){
    B->spaces[i]=0;
  }
  B->spaces[1]=2;
  B->spaces[6]=-5;
  B->spaces[8]=-3;
  B->spaces[12]=5;
  B->spaces[13]=-5;
  B->spaces[17]=3;
  B->spaces[19]=5;
  B->spaces[24]=-2;
  B->bbar=0;
  B->wbar=0;
  B->bout=10;
  B->wout=10;
  B->boff=0;
  B->woff=0;
  return B;
}

void saveboard(struct board *B, int turn){
  FILE *file_pointer;
  file_pointer = fopen("savedboard.txt","w");
  fprintf(file_pointer, "%i\n",turn);
  for(int i=1;i<25;i++){
    fprintf(file_pointer, "%i\n", B->spaces[i]);
  }
  fprintf(file_pointer, "%i\n",B->bbar);
  fprintf(file_pointer, "%i\n",B->wbar);
  fprintf(file_pointer, "%i\n",B->bout);
  fprintf(file_pointer, "%i\n",B->wout);
  fprintf(file_pointer, "%i\n",B->boff);
  fprintf(file_pointer, "%i\n",B->woff); 
  fclose(file_pointer);
}  

int getturn(){
    FILE *fp;
  char c;
  struct board * B = malloc(sizeof(struct board));
  int size=25*sizeof(int);
  B->spaces = malloc(size);
  fp = fopen("savedboard.txt","r");
  c = fgetc(fp);
  if(c=='-'){
    return -1;
  }
  else {
    return 1;
  }
}

struct board * loadboard(){
  FILE *fp;
  char c;
  struct board * B = malloc(sizeof(struct board));
  int size=25*sizeof(int);
  B->spaces = malloc(size);
  fp = fopen("savedboard.txt","r");
  if(fp==NULL){
    fprintf(stderr,"No saved board availible");
    exit(1);
  }
  int isnegative = 0;
  int result = 0;
  while('\n' != (c = fgetc(fp))){
    continue;
  }
  for(int i = 1; i < 31; i++){
    c = fgetc(fp);
    if(c=='-'){
      //printf("Found a negative\n");
      isnegative = 1;
      c = fgetc(fp);
    }
    while(c != '\n'){
      //printf("Found a %c\n",c);
      result = result * 10;
      result = result + (c - '0');
      c = fgetc(fp);
    }
    if(isnegative==1){
      isnegative = 0;
      result = result * -1;
    }
    if(i==25){
      B->bbar = result;
    }
    else if(i==26){
      B->wbar=result;
    }
    else if(i==27){
      B->bout = result;
    }
    else if(i==28){
      B->wout = result;
    }
    else if(i==29){
      B->boff = result;
    }
    else if(i==30){
      B->woff = result;
    }
    else{
      B->spaces[i] = result;
    }
    result = 0;   
  }
  fclose(fp);
  return B;
}
  

void copyboard(struct board *B,struct board *new){
  for(int i=0;i<24;i++){
    new->spaces[i]=B->spaces[i];
  }
  new->bbar=B->bbar;
  new->wbar=B->wbar;
  new->bout=B->bout;
  new->wout=B->wout;
  new->boff=B->boff;
  new->woff=B->woff;
}

void freeboard(struct board *B){
  free(B->spaces);
  free(B);
}


  
