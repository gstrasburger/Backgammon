/* display.c
 * the function which displays the backgammon board in ASCII form
 * by Gabe Strasburger
 */

#include "backgammon.h"
#include "stdio.h"
#include "stdlib.h"

void display(struct board *B){
  printf("%s","\n   W-home B-home \n\n");
  printf("%s","  _______________\n");
  for(int i=1;i<13;i++){
    if(i<10){
      printf("%i |",i);
    }
    else{
      printf("%i|",i);
    }
    int spaceval=B->spaces[i];
    if(spaceval==0){
      printf("%s","-----");
    }
    else{
      char color;
      if(spaceval>0){
	color='W';
      }
      else{
	color='B';
      }
      spaceval=abs(spaceval);
      if(spaceval>5){
	printf("(%cx%i)",color,spaceval);
      }
      else{
	for(int j=1;j<6;j++){
	  if(j<=spaceval){
	    printf("%c",color);
	  }
	  else{
	    printf("%c",'-');
	  }
	}
      }
    }
    printf("%s"," | ");
    spaceval=B->spaces[25-i];
    if(spaceval==0){
      printf("%s","-----");
    }
    else{
      char color;
      if(spaceval>0){
        color='W';
      }
      else{
        color='B';
      }
      spaceval=abs(spaceval);
      if(spaceval>5){
        printf("(%cx%i)",color,spaceval);
      }
      else{
        for(int j=1;j<6;j++){
          if(j<=(5-spaceval)){
            printf("%c",'-');
          }
          else{
            printf("%c",color);
	  }
        }
      }
    }
    printf("%c%i",'|',25-i);
    if(i==3){
      printf("%s","  Inner");
    }
    if(i==4){
      printf("%s","  Tables");
    }
    if(i==6){
      printf("\n  |==Wx%i===Bx%i==|b",B->wbar,B->bbar);
    }
    printf("%c",'\n');
  }
  printf("%s","   -------------\n");
  printf("Black pieces beared off: %i\n",B->boff);
  printf("White pieces beared off: %i\n\n",B->woff);
}
