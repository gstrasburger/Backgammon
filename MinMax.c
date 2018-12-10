/* MinMax.c
 * The structure of a min-max trie, as well as the function for building one
 * By Gabe Strasburger and Will Leet
 */

#include "backgammon.h"
#include <stdio.h>
#include <stdlib.h>

struct mmNode{
  int mm;
  double prob;
  struct ChoiceNode * children;
  struct ChoiceNode * favorite;
  int numchildren;
  int * dice;
};

struct ChoiceNode{
  //keeps track of whether the node is a result of maximizer's or minimizer's choices
  int mm;
  double value;
  struct mmNode * children;
  int * moves;
  struct board B;
};

//This function should never directly alter the board passed to it! Always copy first!
void childlist(struct mmNode * node, struct board * b, int * dice, int turn){
}

mmNode * mmBuild(double probability, int * d,struct board * b, int minmax, int depth, int turn){
  struct mmNode * node=malloc(sizeof(struct mmNode));
  node->prob=probability;
  node->dice=d;
  node->mm=minmax*-1;
  childlist(node);
  return node;
}

ChoiceNode * ChoiceBuild(int * m, board * b, int minmax, int depth){
  struct ChoiceNode * node=malloc(sizeof(struct ChoiceNode));
  node->mm=minmax;
  node->moves=m;
  if(depth==0){
    node->value=BoardVal(b);
  }
  else{
    node->children=malloc(21*sizeof(struct mmNode));
    double punit=1.0000/36.0000
    for(i=0;i<6;i++){
      int * A=malloc(4*sizeof(int));
      for(j=0;j<4;j++){
	A[j]=i+1;
      }
      node->children[i]=mmBuild(punit,A,b,depth-1);
      free(A);
    }
    int index=5;
    for(i=0;i<5;i++){
      for(i=j+1;j<6){
	index++;
	int A[2]={i+1,j+1}
	node->children[index]=mmBuild(punit*2,A,b,depth-1);
      }
    }
  }
  return node;
}

int * TrieSearch(struct mmNode * head){
  for(i=0;i<head->numchildren;i++){
    int v=ChoiceSearch(head->children[i]);
    if(head->mm==1){
      if(head->value==NULL || v>head->value){
        head->value=v;
        head->favorite=head->children[i];
      }
    }
    else{
      if(head->value==NULL || v<head->value){
        head->value=v;
        head->favorite=head->children[i];
      } 
    }
  return head->favorite->moves;
}

double mmSearch(struct mmNode * node){
  for(i=0;i<node->numchildren;i++){
    int v=ChoiceSearch(head->children[i]);
    if(node->mm==1){
      if(node->value==NULL || v>node->value){
        node->value=v;
        node->favorite=node->children[i];
      }
    }
    else{
      if(node->value==NULL || v<node->value){
        node->value=v;
        node->favorite=head->children[i];
      }
    }
  }
  return node->value*node->prob;
}

double ChoiceSearch(struct ChoiceNode * node){
  if(node->children==NULL){
    return node->value;
  }
  else{
    for(i=0;i<21;i++){
      value=value+mmSearch(node->children);
    }
  }
  return node->value;
}
