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
  struct board * B;
  int leaf;
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

void freemm(struct mmNode * tofree){
  free(tofree->children);
  free(tofree->favorite);
  free(tofree->dice);
  free(tofree);
}

void freechoice(struct ChoiceNode * tofree){
  free(tofree->children);
  free(tofree->moves);
  freeboard(tofree->B);
  free(tofree);
}

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
  for(int i = 0; i < list->length; i++){
    if(compareboards(bo,current->b)==1){
      return 1;
    }
    current=current->next;
  }
  return 0;
}

void LLfree(struct LL* list,int root){
  struct LLnode* tofree=list->head->next;
  for(int i = 0; i < list->length; i++){
    struct LLnode* soon=tofree->next;
    //printf("Freeing board\n");
    freeboard(tofree->b);
    if(root==1){
      //printf("Freeing moves\n");
      free(tofree->moves);
    }
    //printf("Freeing node\n");
    free(tofree);
    tofree=soon;
  }
  //printf("Freeing sentinels\n");
  free(list->head);
  free(list->tail);
  //printf("Freeing list\n");
  free(list);
  //printf("Done!\n");
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
	  for(int ii = 0; ii < 8; ii++){
	    A[ii]=0;
	  }
	  A[0]=25;
	  A[1]=dice[0];
	}else{
	  A=NULL;
	}
	printf("%d %d %d %d %d %d %d %d\n",A[0],A[1],A[2],A[3],A[4],A[5],A[6],A[7]);
	add(L1,temp,A);
	temp=newgame();
	copyboard(b,temp);
      //if not
      }else{
	//create a new entry in L1 with each space with white pieces moved by the first die
	for(int i=1;i<25;i++){
	  if(b->spaces[i]>0){
	    if(whitemove(temp,i,dice[0],dice[1])==0){
	      if(root==1){
		A=malloc(8*sizeof(int));
		for(int z = 0; z < 8; z++){
		  A[z]=0;
		}
		printf("First space is %d, second space is %d\n",i,dice[0]);
		A[0]=i;
		A[1]=dice[0];
	      }else{
		A=NULL;
	      }
	      printf("%d %d %d %d %d %d %d %d\n",A[0],A[1],A[2],A[3],A[4],A[5],A[6],A[7]);
	      add(L1,temp,A);
	      temp=newgame();
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
	printf("%d %d %d %d %d %d %d %d\n",A[0],A[1],A[2],A[3],A[4],A[5],A[6],A[7]);
	add(L1,b,A);
      }
      //same thing about the bar but with the second die
      if(whitemove(temp,25,dice[1],dice[0])==0){
	if(root==1){
          A=malloc(8*sizeof(int));
	  for(int y = 0; y < 8; y++){
	    A[y]=0;
	  }
          A[0]=25;
          A[1]=dice[1];
	}else{
          A=NULL;
        }
	printf("%d %d %d %d %d %d %d %d\n",A[0],A[1],A[2],A[3],A[4],A[5],A[6],A[7]);
	add(L2,temp,A);
        temp=newgame();
        copyboard(b,temp);
	//play all the moves that can be done with just the second die
      }else{
	for(int i=1;i<25;i++){
	  if(b->spaces[i]>0){
	    if(whitemove(temp,i,dice[1],dice[0])==0){
	      if(root==1){
		A=malloc(8*sizeof(int));
		for(int jj = 0; jj < 8; jj++){
		  A[jj]=0;
		}
		printf("First space is %d, second space is %d\n",i,dice[1]);
		A[0]=i;
		A[1]=dice[1];
	      }else{
		A=NULL;
	      }
	      printf("%d %d %d %d %d %d %d %d\n",A[0],A[1],A[2],A[3],A[4],A[5],A[6],A[7]);
	      add(L2,temp,A);
	      temp=newgame();
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
	LLfree(L1,root);
	LLfree(L2,root);
	LLfree(final,root);
	return 0;
	//all cases where at least one board in L1 or L2 has no pieces on the bar
      }else{
	struct LLnode* current=L1->head->next;
	temp=newgame();
	copyboard(current->b,temp);
	//take all the boards in L1, use d2 in every way possible, if the result is not a duplicate, add it to final
	for(int j=0; j < L1->length; j++){
	  for(int i=1;i<25;i++){
	    if(current->b->spaces[i]>0){
	      if(whitemove(temp,i,dice[1],-1)==0){
		if(search(final,temp)==0){
		  if(root==1){
		    A=malloc(8*sizeof(int));
		    for(int k=0;k<8;k++){
		      A[k]=current->moves[k];
		    }
		    printf("Third space is %d, fourth space is %d\n",i,dice[1]);
		    A[2]=i;
		    A[3]=dice[1];
		  }else{
		    A=NULL;
		  }
		  printf("%d %d %d %d %d %d %d %d\n",A[0],A[1],A[2],A[3],A[4],A[5],A[6],A[7]);
		  add(final,temp,A);
		  temp=newgame();
		  copyboard(current->b,temp);
		}
	      }
	    }
	  }
	  current=current->next;
	}
	LLfree(L1,root);
	current=L2->head->next;
	temp=newgame();
	copyboard(current->b,temp);
	//take all the boards in L2, use d1 in every way possible, if the result is not a duplicate, add it to final
	for(int j = 0; j<L2->length; j++){
          for(int i=1;i<25;i++){
            if(current->b->spaces[i]>0){
              if(whitemove(temp,i,dice[0],-1)==0){
		if(search(final,temp)==0){
                  if(root==1){
                    A=malloc(8*sizeof(int));
                    for(int k=0;k<8;k++){
                      A[k]=current->moves[k];
                    }
		    printf("Third space is %d, fourth space is %d\n",i,dice[0]);
                    A[2]=i;
                    A[3]=dice[0];
                  }else{
                    A=NULL;
                  }
		  printf("%d %d %d %d %d %d %d %d\n",A[0],A[1],A[2],A[3],A[4],A[5],A[6],A[7]);
                  add(final,temp,A);
                  temp=newgame();
                  copyboard(current->b,temp);
		}
	      } 
            }
          }
	  current=current->next;
        }
	LLfree(L2,root);
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
	LLfree(L1,root);
	LLfree(L2,root);
	LLfree(final,root);
	return 0;
	//all cases where at least one board in L1 or L2 has no pieces on the bar
      }else{
        struct LLnode* current=L1->head->next;
        temp=newgame();
        copyboard(current->b,temp);
	//take all the boards in L1, use d2 in every way possible, if the result is not a duplicate, add it to final
	for(int j = 0; j<L1->length; j++){
	  for(int i=1;i<25;i++){
	    printf("%d\n",i);
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
        LLfree(L1,root);
        current=L2->head->next;
        temp=newgame();
        copyboard(current->b,temp);
	//take all the boards in L2, use d1 in every way possible, if the result is not a duplicate, add it to final
	for(int j = 0; j < L2->length;j++){
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
        LLfree(L2,root);
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
  node->mm=minmax;
  childlist(node,b,d,node->mm,depth,root);
  return node;
}

struct ChoiceNode * ChoiceBuild(int * m, struct board * b, int minmax, int depth){
  struct ChoiceNode * node=malloc(sizeof(struct ChoiceNode));
  node->mm=minmax;
  node->moves=m;
  if(depth==0){
    node->value=BoardVal(b);
    node->leaf=1;
  }
  else{
    node->leaf=0;
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
	node->children[index]=mmBuild(punit*2,A,b,minmax*-1,depth-1,0);
      }
    }
  }
  return node;
}

int * TrieSearch(struct mmNode * head){
  int value = 2000;
  for(int i=0;i<head->numchildren;i++){
    int v=ChoiceSearch(head->children[i]);
    if(head->mm==1){
      if(value==2000 || v>value){
        value=v;
        head->favorite=head->children[i];
      }
    }
    else{
      if(value==2000 || v<value){
        value=v;
        head->favorite=head->children[i];
      } 
    }
  }
  int * result = malloc(sizeof(int)*8);
  for(int j = 0; j < 8; j++){
    result[j]=head->favorite->moves[j];
  }
  for(int k = 0;k<head->numchildren;k++){
    printf("Freeing choice node\n");
    freechoice(head->children[k]);
  }
  printf("Freeing mm node\n");
  freemm(head);
  return result;
}

double mmSearch(struct mmNode * node){
  int value = 2000;
  printf("%d\n",node->mm);
  for(int i=0;i<node->numchildren;i++){
    printf("%d\n",i);
    int v=ChoiceSearch(node->children[i]);
    printf("Freeing choice node\n");
    freechoice(node->children[i]);
    if(node->mm==1){
      if(value== 2000 || v>value){
        value=v;
        node->favorite=node->children[i];
      }
    }
    else{
      if(value== 2000 || v<value){
        value=v;
        node->favorite=node->children[i];
      }
    }
  }
  return value*node->prob;
}

double ChoiceSearch(struct ChoiceNode * node){
  if(node->leaf==1){
    return node->value;
  }
  else{
    for(int i=0;i<21;i++){
      node->value=node->value+mmSearch(node->children[i]);
      printf("Freeing mm node\n");
      freemm(node->children[i]);
    }
  }
  return node->value;
}
