#ifndef COMMAND_H
#define COMMAND_H
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include "parser.h"
#include "liste.h"

#define NB_FUNC_MOVE 2

static char * moveFuncList[NB_FUNC_MOVE] = {"RandomWayPoint", "LinearWayPoint"};

struct aquarium {
  int size[2];
  char * name;
  struct ListView * lvUsed;
  struct ListView * lvReady;
  struct ListFish * lf;
}mainAq;

struct view {
    int num;
    int socket_fd;
    int pos[2];
    int size[2];
};


struct fish {
  char * name;
  int started;
  int pos[2];
  int size[2];
  int* (*move)(int * pos);
  struct ListPos * lp;
};

void initMainAq(int size[],char * name, struct ListView * lv);
char * hello(struct List *l,char * cmd);
int isViewInAquarium(struct view * v, struct aquarium * aq);
char * addView(struct List *l,char * cmd);
char * delView(struct List *l,char * cmd);
char * showAquarium(struct List *l,char * cmd);
int validFuncMove(char * name);
char * addFish(struct List *l,char * cmd);
char * delFish(struct List *l, char *cmd);
char * startFish(struct List *l, char *cmd);
int * RandomWayPoint(int * pos);
int * LinearWayPoint(int * pos);
char * status(struct List *l, char *cmd);
int nbLine(char * name);
char * loadAquarium(struct List *l, char * cmd);
char * saveAquarium(struct List * l, char * cmd);
void updateFish();
int inView(int posV[],int sizeV[],int posF[]);
char * getFishes(struct List * l, char * cmd);
char * getFishesContinuously(struct List * l, char * cmd);
char * pingPong(struct List *l, char * cmd);
char * logOut(struct List *l, char * cmd);
void endGame();





#endif
