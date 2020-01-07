#ifndef LIST_H
#define LIST_H
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "parser.h"
#include "command.h"
#define NB_POS 200

struct NodeFish {
  struct fish * f;
  struct NodeFish * next;
  struct NodeFish * previous;
};

struct ListFish {
  int len;
  struct NodeFish *first;
  struct NodeFish *last;
};

struct NodeView {
  struct view * v;
  struct NodeView * next;
  struct NodeView * previous;
};

struct ListView{
  int len;
  struct NodeView *first;
  struct NodeView *last;
};


struct NodePos {
  int pos[2];
  struct NodePos * next;
  struct NodePos * previous;
};

struct ListPos {
  int len;
  struct NodePos * first;
  struct NodePos * last;
};

struct NodeView * createNodeView(struct view * v);
void affListView(struct ListView * l);
void addNodeView(struct NodeView * nv,struct ListView * l);
struct NodeView * deleteNodeView(int id,struct  ListView * l);
struct NodeView * getNodeView(int i,struct ListView * l);
struct NodeView * findViewIDsocket(int id,struct ListView * lv);
int findView(int id,struct ListView * lv);
void initView(struct view * v,int num,int pos[],int size[],int socket_fd);
struct NodeView * copyNode(struct NodeView *n);

void initFish(struct fish * f,char * name,int pos[],int size[],char * nameFunc);
struct NodeFish * deleteNodeFish(char * name, struct ListFish * l);
void addNodeFish(struct NodeFish * nf, struct ListFish * l );
struct NodeFish * createNodeFish(struct fish * f);
int findNodeFish(char * name, struct ListFish * l);
void affListFish(struct ListFish * l);
void startNodeFish(char * name, struct ListFish * l);

struct NodePos * deleteNodePos(struct ListPos * l);
void addNodePos(struct NodePos * np, struct ListPos * l );
struct NodePos * createNodePos(int pos[]);
void affListPos(struct ListPos * l);

struct ListPos * calculatePos(int* (*move)(int *), int init_pos[]);
int * RandomWayPoint(int * pos);

void freeListPos(struct ListPos * l);
void freeFish(struct fish *f);
void freeListFish(struct ListFish * l);
void freeListView(struct ListView * l);




























#endif
