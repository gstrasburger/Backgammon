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
  struct ChoiceNode ** children;
  struct ChoiceNode * favorite;
  int numchildren;
  int * dice;
};

struct ChoiceNode{
  //keeps track of whether the node is a result of maximizer's or minimizer's choices
  int mm;
  double value;
  struct mmNode** children;
  int * moves;
  struct board B;
};

struct LLnode{
  struct board * b;
  int * moves;
  struct LLnode * next;
};

struct LL{
  struct LLnode * head;
  struct LLnode * tail;
  int length;
};

struct LL * newlist(){
  struct LL* list=malloc(sizeof(struct LL));
  struct LLnode* h=malloc(sizeof(struct LLnode));
  struct LLnode* t=malloc(sizeof(struct LLnode));
  list->head=h;
  list->tail=t;
  list->length=0;
  return list;
}

void add(struct LL* list, struct board* bo, int* m){
  struct LLnode* new=malloc(sizeof(struct LLnode));
  new->next=list->head->next;
  new->b=bo;
  new->moves=m;
  list->head->next=new;
  list->length++;
}

int search(struct LL* list, struct board* bo){
  struct LLnode* current=list->head->next;
  while(current!=list->tail){
    if(compareboards(bo,current->b)==1){
      return 1;
    }
    current=current->next;
  }
  return 0;
}

void LLfree(struct LL* list){
  struct LLnode* tofree=list->head->next;
  while(tofree!=list->tail){
    struct LLnode* soon=tofree->next;
    freeboard(tofree->b);
    if(tofree->moves!=NULL){
      free(tofree->moves);
    }
    free(tofree);
    tofree=soon;
  }
  free(list->head);
  free(list->tail);
  free(list);
}

//This function should never directly alter the board passed to it! Always copy first!
int childlist(struct mmNode * node, struct board * b, int * dice, int mm, int depth,int root){
  //maximizer, i.e. white
  if(mm==1){
    //not doubles
    if(dice[2]==0){
      struct LL* L1=newlist();
      struct LL* L2=newlist();
      struct LL* final=newlist();
      struct board* temp=newgame();
      copyboard(b,temp);
      int* A;
      //if there are white pieces on the bar move one using the first die
      if(whitemove(temp,25,dice[0],dice[1])==0){
	if(root==1){
	  A=malloc(8*sizeof(int));
	  A[0]=25;
	  A[1]=dice[0];
	}else{
	  A=NULL;
	}
	add(L1,temp,A);
	struct board* temp=newgame();
	copyboard(b,temp);
      //if not
      }else{
	//create a new entry in L1 with each space with white pieces moved by the first die
	for(int i=1;i<25;i++){
	  if(b->spaces[i]>0){
	    if(whitemove(temp,i,dice[0],dice[1])==0){
	      if(root==1){
		A=malloc(8*sizeof(int));
		A[0]=i;
		A[1]=dice[0];
	      }else{
		A=NULL;
	      }
	      add(L1,temp,A);
	      struct board* temp=newgame();
	      copyboard(b,temp);
	    }
	  }
	}
      }
      //if L1 is empty then no moves were possible with the first die alone, just make L1 a board that's the same as the original
      if(L1->length==0){
	if(root==1){
          A=malloc(8*sizeof(int));
          A[0]=25;
	  A[1]=dice[0];
	}else{
          A=NULL;
        }
	add(L1,b,A);
      }
      //same thing about the bar but with the second die
      if(whitemove(temp,25,dice[1],dice[0])==0){
	if(root==1){
          A=malloc(8*sizeof(int));
          A[0]=25;
          A[1]=dice[1];
	}else{
          A=NULL;
        }
	add(L2,temp,A);
        struct board* temp=newgame();
        copyboard(b,temp);
	//play all the moves that can be done with just the second die
      }else{
	for(int i=1;i<25;i++){
	  if(b->spaces[i]>0){
	    if(whitemove(temp,i,dice[1],dice[0])==0){
	      if(root==1){
		A=malloc(8*sizeof(int));
		A[0]=i;
		A[1]=dice[0];
	      }else{
		A=NULL;
	      }
	      add(L2,temp,A);
	      struct board* temp=newgame();
	      copyboard(b,temp);
	    }
	  }
	} 
      }
      //if there was originally more than one piece on the bar or it was impossible to get a piece off the bar, then just take the other move from the bar, and make that single board the only child
      if(L1->head->next->b->wbar>0 && L2->head->next->b->wbar>0){
	whitemove(L1->head->next->b,25,dice[1],-1);
	node->numchildren=1;
	node->children=malloc(sizeof(struct ChoiceNode));
	if(root==1){
	  L1->head->next->moves[2]=25;
	  L1->head->next->moves[3]=dice[1];
	  A=malloc(8*sizeof(int));
	  for(int j=0;j<8;j++){
	    A[j]=L1->head->next->moves[j];
	  }
	}
	else{
	  A=NULL;
	}
	temp=newgame();
	copyboard(L1->head->next->b,temp);
	node->children[0]=ChoiceBuild(A,temp,mm,depth);
	LLfree(L1);
	LLfree(L2);
	LLfree(final);
	return 0;
	//all cases where at least one board in L1 or L2 has no pieces on the bar
      }else{
	struct LLnode* current=L1->head->next;
	temp=newgame();
	copyboard(current->b,temp);
	//take all the boards in L1, use d2 in every way possible, if the result is not a duplicate, add it to final
	while(current!=L1->tail){
	  for(int i=1;i<25;i++){
	    if(current->b->spaces[i]>0){
	      if(whitemove(temp,i,dice[1],-1)==0){
		if(search(final,temp)==0){
		  if(root==1){
		    A=malloc(8*sizeof(int));
		    for(int j=0;j<8;j++){
		      A[j]=current->moves[j];
		    }
		    A[2]=i;
		    A[3]=dice[1];
		  }else{
		    A=NULL;
		  }
		  add(final,temp,A);
		  temp=newgame();
		  copyboard(current->b,temp);
		}
	      }
	    }
	  }
	  current=current->next;
	}
	LLfree(L1);
	current=L2->head->next;
	temp=newgame();
	copyboard(current->b,temp);
	//take all the boards in L2, use d1 in every way possible, if the result is not a duplicate, add it to final
	while(current!=L2->tail){
          for(int i=1;i<25;i++){
            if(current->b->spaces[i]>0){
              if(whitemove(temp,i,dice[0],-1)==0){
		if(search(final,temp)==0){
                  if(root==1){
                    A=malloc(8*sizeof(int));
                    for(int j=0;j<8;j++){
                      A[j]=current->moves[i];
                    }
                    A[2]=i;
                    A[3]=dice[0];
                  }else{
                    A=NULL;
                  }
                  add(final,temp,A);
                  temp=newgame();
                  copyboard(current->b,temp);
		}
	      } 
            }
          }
	  current=current->next;
        }
	LLfree(L2);
	node->numchildren=final->length;
	node->children=malloc(final->length*sizeof(struct ChoiceNode));
	current=final->head->next;
	//Finally! Build the array of children.
	for(int i=0;i<final->length;i++){
	  node->children[i]=ChoiceBuild(current->moves,current->b,mm,depth);
	}
	return 0;
      }
    }else{
      //Here's where the code for maximizer rolling doubles goes!
    }
  }else{
    if(dice[2]==0){
      struct LL* L1=newlist();
      struct LL* L2=newlist();
      struct LL* final=newlist();
      struct board* temp=newgame();
      int* A=NULL;
      copyboard(b,temp);
      if(blackmove(temp,25,dice[0],dice[1])==0){
        add(L1,temp,A);
        struct board* temp=newgame();
        copyboard(b,temp);
      }else{
	//create a new entry in L1 with each space with white pieces moved by the first die
	for(int i=1;i<25;i++){
	  if(b->spaces[i]>0){
	    if(blackmove(temp,i,dice[0],dice[1])==0){
	      add(L1,temp,A);
	      struct board* temp=newgame();
	      copyboard(b,temp);
	    }
	  }
	}
      }
      //if L1 is empty then no moves were possible with the first die alone, just make a board that's the same as the original
      if(L1->length==0){
        add(L1,b,A);
      }
      //same thing about the bar but with the second die
      if(blackmove(temp,25,dice[1],dice[0])==0){
        add(L2,temp,A);
        struct board* temp=newgame();
        copyboard(b,temp);
	//play all the moves that can be done with just the second
      }else{
	for(int i=1;i<25;i++){
	  if(b->spaces[i]>0){
	    if(blackmove(temp,i,dice[1],dice[0])==0){
	      add(L2,temp,A);
	      struct board* temp=newgame();
	      copyboard(b,temp);
	    }
	  }
	}
      }
      //if there was originally more than one piece on the bar or it was impossible to get a piece off the bar, then just take the other move from the bar, and make that single board the only child 
      if(L1->head->next->b->bbar>0 && L2->head->next->b->bbar>0){
	blackmove(L1->head->next->b,25,dice[1],-1);
	node->numchildren=1;
	node->children=malloc(sizeof(struct ChoiceNode));
	temp=newgame();
	copyboard(L1->head->next->b,temp);
	node->children[0]=ChoiceBuild(A,temp,mm,depth);
	LLfree(L1);
	LLfree(L2);
	LLfree(final);
	return 0;
	//all cases where at least one board in L1 or L2 has no pieces on the bar
      }else{
        struct LLnode* current=L1->head->next;
        temp=newgame();
        copyboard(current->b,temp);
	//take all the boards in L1, use d2 in every way possible, if the result is not a duplicate, add it to final
	while(current!=L1->tail){
	  for(int i=1;i<25;i++){
	    if(current->b->spaces[i]>0){
	      if(blackmove(temp,i,dice[1],-1)==0){
		if(search(final,temp)==0){
		  add(final,temp,A);
                  temp=newgame();
                  copyboard(current->b,temp);
                }
              }
            }
          }
          current=current->next;
        }
        LLfree(L1);
        current=L2->head->next;
        temp=newgame();
        copyboard(current->b,temp);
	//take all the boards in L2, use d1 in every way possible, if the result is not a duplicate, add it to final
	while(current!=L2->tail){
	  for(int i=1;i<25;i++){
	    if(current->b->spaces[i]>0){
	      if(blackmove(temp,i,dice[0],-1)==0){
		if(search(final,temp)==0){
		  add(final,temp,A);
                  temp=newgame();
                  copyboard(current->b,temp);
                }
              }
            }
          }
          current=current->next;
        }
        LLfree(L2);
        node->numchildren=final->length;
        node->children=malloc(final->length*sizeof(struct ChoiceNode));
        current=final->head->next;
        //Finally! Build the array of children.
	for(int i=0;i<final->length;i++){
          node->children[i]=ChoiceBuild(current->moves,current->b,mm,depth);
        }
        return 0;
      }
    }else{
      //Here's where the code for minimizer rolling doubles goes!
    }
  }
}

struct mmNode * mmBuild(double probability, int * d,struct board * b, int minmax, int depth,int root){
  struct mmNode * node=malloc(sizeof(struct mmNode));
  node->prob=probability;
  node->dice=d;
  node->mm=minmax*-1;
  childlist(node,b,d,node->mm,depth,root);
  return node;
}

struct ChoiceNode * ChoiceBuild(int * m, struct board * b, int minmax, int depth){
  struct ChoiceNode * node=malloc(sizeof(struct ChoiceNode));
  node->mm=minmax;
  node->moves=m;
  if(depth==0){
    node->value=BoardVal(b);
  }
  else{
    //node->children=malloc(21*sizeof(struct mmNode));
    node->children=malloc(15*sizeof(struct mmNode));
    double punit=1.0000/36.0000;
    //for(i=0;i<6;i++){
    //int * A=malloc(4*sizeof(int));
    //for(j=0;j<4;j++){
    //A[j]=i+1;
    //}
    //node->children[i]=mmBuild(punit,A,b,depth-1);
    //free(A);
    //}
    int index=0;
    //int index=5;
    for(int i=0;i<5;i++){
      for(int j=i+1;j<6;j++){
	index++;
	int A[2]={i+1,j+1};
	node->children[index]=mmBuild(punit*2,A,b,minmax,depth-1,0);
      }
    }
  }
  return node;
}

int * TrieSearch(struct mmNode * head){
  int value;
  for(int i=0;i<head->numchildren;i++){
    int v=ChoiceSearch(head->children[i]);
    if(head->mm==1){
      if(value==NULL || v>value){
        value=v;
        head->favorite=head->children[i];
      }
    }
    else{
      if(value==NULL || v<value){
        value=v;
        head->favorite=head->children[i];
      } 
    }
  return head->favorite->moves;
  }
  int A[8]={0,0,0,0,0,0,0,0};
  return A;
}

double mmSearch(struct mmNode * node){
  int value;
  for(int i=0;i<node->numchildren;i++){
    int v=ChoiceSearch(node->children[i]);
    if(node->mm==1){
      if(value==NULL || v>value){
        value=v;
        node->favorite=node->children[i];
      }
    }
    else{
      if(value==NULL || v<value){
        value=v;
        node->favorite=node->children[i];
      }
    }
  }
  return value*node->prob;
}

double ChoiceSearch(struct ChoiceNode * node){
  if(node->children==NULL){
    return node->value;
  }
  else{
    for(int i=0;i<21;i++){
      node->value=node->value+mmSearch(node->children[i]);
    }
  }
  return node->value;
}
