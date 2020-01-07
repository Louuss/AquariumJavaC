#include "liste.h"


struct NodeView * createNodeView(struct view * v){
  struct NodeView * nv = malloc(sizeof(struct NodeView));
  nv->v = v;
  nv->previous = NULL;
  nv->next = NULL;
  return nv;
}


void affListView(struct ListView * l){
  if(l->len == 0){
    printf("liste vide");
  }
  else {
    struct NodeView * current = l->first;
    while(current != NULL){

      printf(" N%d %dx%d+%d+%d \n", current->v->num,(current->v->pos)[0],
                                                              (current->v->pos)[1],(current->v->size)[0],
                                                              (current->v->size)[1]);
      current = current->next;
    }
  }
}

void affListPos(struct ListPos * l){
  if(l->len == 0){
    printf("liste vide");
  }
  else {
    struct NodePos * current = l->first;
    while(current != NULL){

      printf("(%d,%d)\n", (current->pos)[0],
                          (current->pos)[1]);
      current = current->next;
    }
  }
}

void affListFish(struct ListFish * l){
  if(l->len == 0){
    printf("liste vide");
  }
  else {
    struct NodeFish * current = l->first;
    while(current != NULL){

      printf("%s %dx%d,%dx%d, started:%d \n", current->f->name,(current->f->pos)[0],
                                                              (current->f->pos)[1],(current->f->size)[0],
                                                              (current->f->size)[1],
                                                              current->f->started);
      //affListPos(current->f->lp);
      current = current->next;
    }
  }
}

void addNodeView(struct NodeView * nv,struct ListView * l){
  if(l->len == 0){
    l->last = nv;
    l->first = nv;
    l->len = 1;
  }
  else {
    struct NodeView * current = l->first;
    while(current != NULL){
      if(current->v->num > nv->v->num){
        nv->previous = current->previous;
        if(current->previous != NULL)
          current->previous->next = nv;
        else
          l->first = nv;
        current->previous = nv;
        nv->next = current;
        (l->len)++;
        return;
      }
      current = current->next;
    }
    nv->previous = l->last;
    l->last->next = nv;
    l->last = nv;
    (l->len)++;
  }
}



struct NodeView * deleteNodeView(int id,struct  ListView * l){
  if (l->len == 0)
    return NULL;
  struct NodeView * current = l->first;
  while (current != NULL){
    if(current->v->num == id){
      if(current->previous != NULL)
        current->previous->next = current->next;
      else
        l->first = current->next;
      if(current->next != NULL)
        current->next->previous = current->previous;
      else
        l->last = current->previous;
      (l->len)--;
      current->next = NULL;
      current->previous = NULL;
      return current;
    }
    current = current->next;
  }
  return NULL;
}


struct NodeView * getNodeView(int i,struct ListView * l){
  if(i > l->len)
    return NULL;
  struct NodeView * current = l->first;
  int j = 0;
  for(j=0;j<i;j++){
    current = current->next;
  }
  return current;
}

struct NodeView * findViewIDsocket(int id,struct ListView * lv){
  struct NodeView * current = lv->first;
  while (current != NULL){
    if(current->v->socket_fd == id)
      return current;
    current = current->next;
  }
  return NULL;
}

int findView(int id,struct ListView * lv){
  struct NodeView * current = lv->first;
  while (current != NULL){
    if(current->v->num == id)
      return 1;
    current = current->next;
  }
  return 0;
}

void initView(struct view * v,int num,int pos[],int size[],int socket_fd){
  v->num = num;
  v->pos[0] = pos[0];
  v->pos[1] = pos[1];
  v->size[0] = size[0];
  v->size[1] = size[1];
  v->socket_fd = socket_fd;
}


struct NodeView * copyNode(struct NodeView *n){
  struct NodeView * res = malloc(sizeof(struct NodeView));
  res->v = malloc(sizeof(struct view));
  initView(res->v,n->v->num,n->v->pos,n->v->size,n->v->socket_fd);
  res->next = NULL;
  res->previous =NULL;
  return res;
}

struct NodeFish * createNodeFish(struct fish * f){
  struct NodeFish * nf = malloc(sizeof(struct NodeFish));
  nf->f = f;
  nf->previous = NULL;
  nf->next = NULL;
  return nf;
}

void addNodeFish(struct NodeFish * nf, struct ListFish * l ){
  if(l->len == 0){
    l->last = nf;
    l->first = nf;
    l->len = 1;
  }
  else {
    nf->previous = l->last;
    l->last->next = nf;
    l->last = nf;
    (l->len)++;
  }
}

struct NodeFish * deleteNodeFish(char * name, struct ListFish * l){
  if (l->len == 0)
    return NULL;
  struct NodeFish * current = l->first;
  while (current != NULL){
    if(!strcmp(current->f->name,name)){
      if(current->previous != NULL)
        current->previous->next = current->next;
      else
        l->first = current->next;
      if(current->next != NULL)
        current->next->previous = current->previous;
      else
        l->last = current->previous;
      (l->len)--;
      current->next = NULL;
      current->previous = NULL;
      return current;
    }
    current = current->next;
  }
  return NULL;
}


int findNodeFish(char * name, struct ListFish * l){
  struct NodeFish * current = l->first;
  char cpy[strlen(name)];
  while (current != NULL){
    strcpy(cpy, current->f->name);
    printf("%s\n", cpy);
    if(!strcmp(current->f->name, name) || !strcmp(strcat(cpy, "\n"), name))
      return 1;
    current = current->next;
  }
  return 0;
}

void startNodeFish(char * name, struct ListFish * l){
  struct NodeFish * current = l->first;
  char cpy[strlen(name)];
  int found = 0;
  while (current != NULL && !found){
    strcpy(cpy, current->f->name);
    printf("%s\n", cpy);
    if(!strcmp(current->f->name, name) || !strcmp(strcat(cpy, "\n"), name)){
      current->f->started = 1;
      found = 1;
    }
    else
      current = current->next;
  }
}

struct NodePos * createNodePos(int pos[]){
  struct NodePos * np = malloc(sizeof(struct NodePos));
  (np->pos)[0] = pos[0];
  (np->pos)[1] = pos[1];
  np->previous = NULL;
  np->next = NULL;
  return np;
}


void addNodePos(struct NodePos * np, struct ListPos * l ){
  if(l->len == 0){
    l->last = np;
    l->first = np;
    l->len = 1;
  }
  else {
    np->previous = l->last;
    l->last->next = np;
    l->last = np;
    (l->len)++;
  }
}


struct NodePos * deleteNodePos(struct ListPos * l){
  if (l->len == 0)
    return NULL;
  struct NodePos * np = l->first;
  l->first = np->next;
  if (np->next != NULL)
    np->next->previous = NULL;
  l->len = l->len -1;
  np->next = NULL;
  np->previous = NULL;
  return np;
}



void * attribueFunc(char * name){
  if(!strcmp(name,moveFuncList[0]))
    return RandomWayPoint;
  else if(!strcmp(name,moveFuncList[1]))
    return LinearWayPoint;
  else{
    assert(0);
    return NULL;
  }
}

struct ListPos * calculatePos(int* (*move)(int *), int init_pos[]){
  int i;
  struct ListPos * res = malloc(sizeof(struct ListPos));
  res->len = 0;
  res->first = NULL;
  res->last = NULL;
  //int *pos = move(init_pos);
  int * t = move(init_pos);
  addNodePos(createNodePos(t), res);
  free(t);
  for (i = 1; i<NB_POS; i++){
    //pos = move(res->last->pos);
    t = move(res->last->pos);
    addNodePos(createNodePos(t), res);
    free(t);
  }
  return res;
}



void initFish(struct fish * f,char * name,int pos[],int size[],char * nameFunc){
  f->name = malloc(sizeof(char)*strlen(name)+1);
  strcpy(f->name,name);
  f->pos[0] = pos[0];
  f->pos[1] = pos[1];
  f->size[0] = size[0];
  f->size[1] = size[1];
  f->move = attribueFunc(nameFunc);
  f->lp = calculatePos(f->move, f->pos);
  f->started = 0;
}

void freeListPos(struct ListPos * l){
  struct NodePos * current = l->first;
  if (current != NULL){
    while (current->next!=NULL){
      current = current->next;
      free(current->previous);
    }
    free(current);
  }
  free(l);
}

void freeFish(struct fish *f){
  free(f->name);
  freeListPos(f->lp);
  free(f);
}

void freeListFish(struct ListFish * l){
  struct NodeFish * current = l->first;
  if (current != NULL){
    while (current->next!=NULL){
      current = current->next;
      freeFish(current->previous->f);
      free(current->previous);
    }
    freeFish(current->f);
    free(current);
  }
  free(l);
}

void freeListView(struct ListView * l){
  struct NodeView * current = l->first;
  if (current != NULL){
    while (current->next!=NULL){
      current = current->next;
      free(current->previous->v);
      free(current->previous);
    }
    free(current->v);
    free(current);
  }
  free(l);
}
