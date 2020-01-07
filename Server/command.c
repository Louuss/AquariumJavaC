#include "command.h"

void initMainAq(int size[],char * name, struct ListView * lv){
  mainAq.size[0] = size[0];
  mainAq.size[1] = size[1];
  mainAq.name = malloc(sizeof(char)*50);
  strcpy(mainAq.name,name);

  mainAq.lvUsed = malloc(sizeof(struct ListView));
  mainAq.lvUsed->len = 0;
  mainAq.lvUsed->first = NULL;
  mainAq.lvUsed->last = NULL;

  mainAq.lvReady = malloc(sizeof(struct ListView));
  mainAq.lvReady->len = 0;
  mainAq.lvReady->first = NULL;
  mainAq.lvReady->last = NULL;
  if(lv !=NULL){
    struct NodeView * current = lv->first;
    struct NodeView * cp = NULL;
    while(current != NULL){
      cp = copyNode(current);
      addNodeView(cp,mainAq.lvReady);
      current = current->next;
    }
  }
  mainAq.lf = malloc(sizeof(struct ListFish));
  mainAq.lf->len = 0;
  mainAq.lf->first = NULL;
  mainAq.lf->last = NULL;
}


char * hello(struct List *l,char * cmd){
  if(mainAq.lvReady->len == 0){
    strcpy(cmd,"no greeting");
    return cmd;
  }
  // printf("Used\n" );
  // affListView(mainAq.lvUsed);
  // printf("\nReady\n" );
  // affListView(mainAq.lvReady);
  int fd = atoi(cmd);
  strcpy(cmd,"greeting N");
  char sid[3];
  int id = -1;
  if(l->len == 1){
    id = mainAq.lvReady->first->v->num;
    mainAq.lvReady->first->v->socket_fd = fd;
    sprintf(sid,"%d",id);
    strcat(cmd,sid);
    addNodeView(deleteNodeView(id,mainAq.lvReady),mainAq.lvUsed);
    return cmd;
  }
  id = atoi((l->last->s)+1);
  if (!findView(id,mainAq.lvReady)){
    id = mainAq.lvReady->first->v->num;
    mainAq.lvReady->first->v->socket_fd = fd;
    sprintf(sid,"%d",id);
    strcat(cmd,sid);
    addNodeView(deleteNodeView(id,mainAq.lvReady),mainAq.lvUsed);
    return cmd;
  }
  sprintf(sid,"%d",id);
  strcat(cmd,sid);

  struct NodeView * tmp = deleteNodeView(id,mainAq.lvReady);
  tmp->v->socket_fd = fd;
  addNodeView(tmp,mainAq.lvUsed);
  return cmd;
}



int isViewInAquarium(struct view * v, struct aquarium * aq){
  if(v->pos[0] >= aq->size[0] || v->pos[0] < 0 || v->pos[1] >= aq->size[1] || v->pos[1] < 0)
    return 0;

  if(v->pos[0]+v->size[0] >= aq->size[0] || v->pos[1]+v->size[1] >= aq->size[1])
    return 0;

  if(v->size[0] < 0 || v->size[1] < 0)
    return 0;

  return 1;
}

char * addView(struct List *l,char * cmd){
  struct view * v = malloc(sizeof(struct view));

  int id = atoi((l->first->next->next->s)+1);
  struct List * l2 = parse(l->first->next->next->next->s,'x');
  int x = atoi(l2->first->s);
  struct List * l3 = parse(l2->last->s,'+');
  int y = atoi(l3->first->s);
  int width = atoi(l3->first->next->s);
  int height = atoi(l3->first->next->next->s);
  int pos[2] = {x,y};
  int size[2] = {width,height};
  initView(v,id,pos,size,-1);

  //printf("%d,%d,%d,%d,%d\n",id,x,y,width,height);
  if(findView(id,mainAq.lvReady) || findView(id,mainAq.lvUsed)){
    strcpy(cmd,"NOK");
    freeList(l2);
    freeList(l3);
    return cmd;
  }
  if(!isViewInAquarium(v,&mainAq)){
    strcpy(cmd,"NOK");
    freeList(l2);
    freeList(l3);
    return cmd;
  }

  struct NodeView * nv = createNodeView(v);
  addNodeView(nv,mainAq.lvReady);
  strcpy(cmd,"OK");
  freeList(l2);
  freeList(l3);
  return cmd;
}

char * delView(struct List *l,char * cmd){
  int id = atoi((l->first->next->next->s)+1);
  printf("%d\n",id);
  if(findView(id,mainAq.lvReady)){
    strcpy(cmd,"OK");
    struct NodeView * nv = deleteNodeView(id,mainAq.lvReady);
    free(nv->v);
    free(nv);
    return cmd;
  }
  if(findView(id,mainAq.lvUsed)){
    strcpy(cmd,"OK");
    struct NodeView * nv = deleteNodeView(id,mainAq.lvUsed);
    free(nv->v);
    free(nv);
    return cmd;
  }
  strcpy(cmd,"NOK");
  return cmd;
}

char * showAquarium(struct List *l,char * cmd){
  (void)(l);
  sprintf(cmd,"%dx%d\n",mainAq.size[0],mainAq.size[1]);
  struct ListView allView;
  allView.len =0;
  allView.first = NULL;
  allView.last = NULL;
  struct NodeView *  current = mainAq.lvReady->first;
  struct NodeView *  tmp = NULL;
  int i=0;
  for(i=0;i<mainAq.lvReady->len;i++){
    tmp = copyNode(current);
    addNodeView(tmp,&allView);
    current = current->next;
  }
  current = mainAq.lvUsed->first;
  i=0;
  for(i=0;i<mainAq.lvUsed->len;i++){
    tmp = copyNode(current);
    addNodeView(tmp,&allView);
    current = current->next;
  }
  i=0;
  current = allView.first;
  char param[100];
  for(i=0;i<allView.len;i++){
    sprintf(param,"N%d %dx%d+%d+%d\n",current->v->num,current->v->pos[0],current->v->pos[1],current->v->size[0],current->v->size[1]);
    strcat(cmd,param);
    current =current->next;
  }


  struct NodeView * cc= allView.first;
  if (cc != NULL){
    while (cc->next!=NULL){
      cc = cc->next;
      free(cc->previous->v);
      free(cc->previous);
    }
    free(cc->v);
    free(cc);
  }
  return cmd;
}

int validFuncMove(char * name){
  int i = 0;
  char cpy[strlen(name)];
  for(i = 0;i<NB_FUNC_MOVE;i++){
    strcpy(cpy, moveFuncList[i]);
    if(!strcmp(name,moveFuncList[i]) || !strcmp(name,strcat(cpy, "\n")))
      return 1;
  }
  return 0;
}


char * addFish(struct List *l,char * cmd){
  char * name = l->first->next->s;
  // printf("%s\n", name);
  if(findNodeFish(name,mainAq.lf)){
    strcpy(cmd,"NOK: ce nom existe déjà");
    return cmd;
  }
  // printf("Ok : 3\n");
  struct List * l1 = parse(l->first->next->next->next->s,',');
  // affList(l1);
  struct List * l2 = parse(l1->first->s,'x');
  // affList(l2);
  int pos[2];
  pos[0] = atoi(l2->first->s);
  pos[1] = atoi(l2->first->next->s);
  // printf("OK: 4\n" );
  if(pos[0] > 100 || pos[1]  > 100 || pos[0] < 0 || pos[1] < 0){
    strcpy(cmd,"NOK");
    freeList(l1);
    freeList(l2);
    return cmd;
  }
  // printf("OK: 5\n" );

  int socket_fd = atoi(cmd);
  // printf("socket id : %d\n",socket_fd);
  struct NodeView * nv = findViewIDsocket(socket_fd,mainAq.lvUsed);
  // printf("OK: 5.1, %p\n",nv);
  pos[0] = pos[0] * nv->v->size[0] / 100 + nv->v->pos[0];
  pos[1] = pos[1] * nv->v->size[1] / 100 + nv->v->pos[1];
  // printf("OK: 6, %d,%d\n",pos[0],pos[1]);

  struct List * l3 = parse(l->first->next->next->next->next->s,',');
  struct List * l4 = parse(l3->first->s,'x');
  //affList(l3);
  int size[2];
  // printf("OK: 7\n" );

  size[0] = atoi(l4->first->s);
  size[1] = atoi(l4->first->next->s);
  char * moveFuncName = l->first->next->next->next->next->next->s;
  //printf("OK 1\n");
  if(!validFuncMove(moveFuncName)){
    strcpy(cmd,"NOK : modèle de mobilté non supporté");
    freeList(l1);
    freeList(l2);
    freeList(l3);
    freeList(l4);
    return cmd;
  }
  // printf("OK 8\n");
  struct fish * f = malloc(sizeof(struct fish));
  initFish(f, name, pos, size, moveFuncName);
  addNodeFish(createNodeFish(f), mainAq.lf);
  // printf("OK: 9\n" );
  strcpy(cmd, "OK");
  freeList(l1);
  freeList(l2);
  freeList(l3);
  freeList(l4);
  return cmd;
}

char * delFish(struct List *l, char *cmd){
  char *name = l->first->next->s;
  if (findNodeFish(name, mainAq.lf)){
    struct NodeFish * nf = deleteNodeFish(name, mainAq.lf);
    freeFish(nf->f);
    free(nf);
    strcpy(cmd, "OK");
    return cmd;
  }
  strcpy(cmd, "NOK : Poisson inexistant");
  return cmd;
}

char * startFish(struct List *l, char *cmd){
  char *name = l->first->next->s;
  if (findNodeFish(name, mainAq.lf)){
    startNodeFish(name, mainAq.lf);
    strcpy(cmd, "OK");
    return cmd;
  }
  strcpy(cmd, "NOK : Poisson inexistant");
  return cmd;
}

int * RandomWayPoint(int * pos){
  (void)(pos);
  int *res = malloc(2*sizeof(int));
  res[0] = rand()% mainAq.size[0];
  res[1] = rand()% mainAq.size[1];
  return res;
}

int * LinearWayPoint(int * pos){
  int * res = malloc(2*sizeof(int));
  if (pos[0] < mainAq.size[0]-20)
    res[0] = pos[0] + 20;
  else
    res[0] = 20;
  res[1] = pos[1];
  return res;
}

char * status(struct List *l, char *cmd){
  (void)(l);
  int n = mainAq.lf->len;
  int i;
  char fish[100];
  struct NodeFish *current = mainAq.lf->first;
  sprintf(fish, "OK : Connecté au contrôleur, %d poisson(s) trouvé(s)\n", n);
  strcpy(cmd, fish);
  // printf("%s\n", cmd);
  for (i=0; i<n; i++){

    sprintf(fish, "%s at %dx%d,%dx%d ", current->f->name,
                                          current->f->pos[0],
                                          current->f->pos[1],
                                          current->f->size[0],
                                          current->f->size[1]);
    strcat(cmd, fish);
    if (current->f->started)
      strcat(cmd, "started\n");
    else
      strcat(cmd, "not started\n");
    current = current->next;
  }
  return cmd;
}

int nbLine(char * name){
  FILE * fd = fopen(name,"r");
  int c;
  int nb_line = 0;
  while ((c = getc(fd)) != EOF){
      if (c == '\n')
          ++nb_line;
  }
  fclose(fd);
  return nb_line;
}

char * loadAquarium(struct List *l, char * cmd){//Cedric, si le client rentre Nawak, ca segfault : on peut mettre une securite avant de load un fichier inexistant??
  char * tobozo = strndup(l->first->next->s,strlen(l->first->next->s));
  printf("Name : %s\n", tobozo);
  int max = nbLine(tobozo);
  FILE * fd = fopen(tobozo,"r");
  if(fd == NULL){
    strcpy(cmd,"NOK");
    return cmd;
  }
  char line[50];
  int cmpt = 1;
  fgets(line,40,fd);
  struct List * l1 = parse(line,'x');
  int size[2] = {atoi(l1->first->s),atoi(l1->last->s)};
  int pos[2] = {-1,-1};
  int sizeLv[2] = {-1,-1};
  int num = -1;
  (void)(num);
  struct ListView * lv = malloc(sizeof(struct ListView));
  lv->len = 0;
  lv->first = NULL;
  lv->last = NULL;
  struct view * v = NULL;
  struct List * l2 = NULL;
  struct List * l3 = NULL;
  freeList(l1);
  while(cmpt < max){
    fgets(line,50,fd);
    l1 = parse(line,' ');
    l2 = parse(l1->first->next->s,'x');
    l3 = parse(l2->first->next->s,'+');
    v = malloc(sizeof(struct view));
    pos[0] = atoi(l2->first->s);
    pos[1] = atoi(l3->first->s);
    sizeLv[0] = atoi(l3->first->next->s);
    sizeLv[1] = atoi(l3->last->s);
    initView(v,atoi(l1->first->s+1), pos, sizeLv,-1);
    addNodeView(createNodeView(v),lv);
    cmpt++;
    freeList(l1);
    freeList(l2);
    freeList(l3);
  }
  initMainAq(size,tobozo,lv);
  free(tobozo);
  freeListView(lv);
  sprintf(cmd,"aquarium loaded (%d display view)!",max-1);
  fclose(fd);
  return cmd;
}

char * saveAquarium(struct List * l, char * cmd){//N'efface pas ce qui existe déjà : impossible "d'effacer" une vue dans l'enregistrement + Ne change pas le nom?
  (void)(l);
  sprintf(cmd,"aquarium saved (%d display view)!",mainAq.lvUsed->len + mainAq.lvReady->len);
  char * name = l->first->next->s;
  printf("le name : %s\n", name);
  int fd = open(name, O_RDWR | O_CREAT | O_TRUNC, S_IRWXU);
  char * data = malloc(sizeof(char)*1000);
  data = showAquarium(NULL,data);
  write(fd,data,strlen(data));
  close(fd);
  free(data);
  return cmd;
}

void updateFish(){
    if (mainAq.lf == NULL) {
	return;
    }
  struct NodeFish * current  = mainAq.lf->first;
  struct fish * cf = NULL;
  while(current != NULL){
    cf = current->f;
    if(cf->started){
        struct NodePos * np = deleteNodePos(cf->lp);
        cf->pos[0] = np->pos[0];
        cf->pos[1] = np->pos[1];
        free(np);
      if(cf->lp->len == 0){
        cf->lp = calculatePos(cf->move,cf->pos);
      }
    }
    current = current->next;
  }
}

int inView(int posV[],int sizeV[],int posF[]){
  return ((posV[0] + sizeV[0] >= posF[0]) &&
         (posV[1] + sizeV[1] >= posF[1]) &&
         (posV[0] <= posF[0]) &&
         (posV[1] <= posF[1]));
}

char * getFishes(struct List * l, char * cmd){
  (void)(l);
  int socket_fd = atoi(cmd);
  struct NodeView * nv = findViewIDsocket(socket_fd,mainAq.lvUsed);
  if(nv == NULL){
    strcpy(cmd,"NOK");
    return cmd;
  }
  struct view * v = nv->v;

  struct ListFish lf;
  lf.first = NULL;
  lf.last = NULL;
  lf.len = 0;

  struct NodeFish * current = mainAq.lf->first;
  while(current != NULL){
    if(inView(v->pos, v->size, current->f->pos)){
        addNodeFish(current,&lf);
    }
    current = current->next;
  }
  strcpy(cmd,"list ");
  current = lf.first;
  char tmp[100];
  int p[2] = {0,0};
  while(current != NULL){
      p[0] = ((current->f->pos[0] - v->pos[0])*100 / v->size[0]) ;
      p[1] = ((current->f->pos[1] - v->pos[1])*100 / v->size[1]);
      sprintf(tmp,"[%s at %dx%d,%dx%d,%d] ",current->f->name,p[0],p[1],current->f->size[0],current->f->size[1], 1);
      strcat(cmd,tmp);
      current = current->next;
  }
  return cmd;
}

char * getFishesContinuously(struct List * l, char * cmd){
    cmd = getFishes(l,cmd);
    return cmd;
  }

char * pingPong(struct List *l, char * cmd) {
    strcpy(cmd, "pong ");
    strcat(cmd, l->first->next->s);
    return cmd;
}

char * logOut(struct List *l, char * cmd) {
  (void)(l);
  int num = 0;
  int socket = atoi(cmd);
  struct NodeView * elt = mainAq.lvUsed->first;
  while (elt != NULL) {
  	if (elt->v->socket_fd == socket) {
  	    num = elt->v->num;
        break;
  	}
  	elt = elt->next;
  }
  addNodeView(deleteNodeView(num,mainAq.lvUsed),mainAq.lvReady);
  strcpy(cmd, "bye");
  return cmd;
}

void endGame(){
  free(mainAq.name);
  freeListView(mainAq.lvUsed);
  freeListView(mainAq.lvReady);
  freeListFish(mainAq.lf);
}

// int main(int argc,char *argv[]){
  //INITIALISATION
  // int size[2] = {1000,1000};
  // initMainAq(size,"mainAq",NULL);

  // TEST loadAquarium
  // char cmd[100];
  // char buffer[100];
  // fgets(cmd,100,stdin);
  // printf("%s\n",cmd);
  // affList(parse(cmd,' '));
  // printf("%s\n", loadAquarium(parse(cmd,' '),buffer));
  // fgets(cmd,100,stdin);
  // printf("%s\n",cmd);
  // affList(parse(cmd,' '));
  // printf("%s\n",showAquarium(parse(cmd,' '),buffer));


  // int size1[2] = {1,1};
  // int pos1[2] = {1,1};
  // struct view v1;
  // initView(&v1,1,pos1,size1,-1);
  //
  // int size2[2] = {2,2};
  // int pos2[2] = {2,2};
  // struct view v2;
  // initView(&v2,2,pos2,size2,-1);
  //
  // int size3[2] = {3,3};
  // int pos3[2] = {3,3};
  // struct view v3;
  // initView(&v3,3,pos3,size3,-1);
  //
  // int size4[2] = {4,4};
  // int pos4[2] = {4,4};
  // struct view v4;
  // initView(&v4,4,pos4,size4,1);

  // struct NodeView * n1 = createNodeView(&v1);
  // struct NodeView * n2 = createNodeView(&v2);
  // struct NodeView * n3 = createNodeView(&v3);
  // struct NodeView * n4 = createNodeView(&v4);
  //
  // addNodeView(n1,mainAq.lvReady);
  // addNodeView(n2,mainAq.lvReady);
  // addNodeView(n3,mainAq.lvReady);
  // addNodeView(n4,mainAq.lvUsed);
  //
  // affListView(mainAq.lvReady);
  // affListView(mainAq.lvUsed);


  //DEBUT TEST HELLO
  // char cmd[100];
  // char buffer[100];
  // int i=0;
  // for(i=0;i<5;i++){
  //   printf("test  : %d\n",i);
  //   fgets(cmd,100,stdin);
  //   strcpy(buffer,"4");
  //   printf("%s\n",cmd);
  //   affList(parse(cmd,' '));
  //   printf("%s\n", hello(parse(cmd,' '),buffer));
  //   printf("\n");
  // }


  // // DEBUT TEST ADD VIEW
  // char cmd[100];
  // char buffer[100];
  // int i=0;
  // for(i=0;i<2;i++){
  //   printf("test  : %d\n",i);
  //   fgets(cmd,100,stdin);
  //   printf("%s\n",cmd);
  //   affList(parse(cmd,' '));
  //   printf("%s\n",addView(parse(cmd,' '),buffer));
  //   affListView(mainAq.lvReady);
  //   printf("\n");
  // }
  //
  // // DEBUT TEST DEl VIEW
  // char cmd[100];
  // char buffer[100];
  // fgets(cmd,100,stdin);
  // printf("%s\n",cmd);
  // affList(parse(cmd,' '));
  // printf("%s\n",delView(parse(cmd,' '),buffer));
  // affListView(mainAq.lvReady);
  // affListView(mainAq.lvUsed);

  //DEBUT TEST SHOW AQUARIUM
  // char cmd[100];
  // char buffer[100];
  // fgets(cmd,100,stdin);
  // printf("%s\n",cmd);
  // affList(parse(cmd,' '));
  // printf("%s\n",showAquarium(parse(cmd,' '),buffer));

  //DEBUT TEST ADD FISH
  //  fgets(cmd,100,stdin);
  //  printf("%s\n",cmd);
  //  affList(parse(cmd,' '));
  //  printf("%s\n", addFish(parse(cmd,' '),buffer));
  //  affListFish(mainAq.lf);
  //
  //  char cmd1[100];
  //  char buffer1[100];
  //  fgets(cmd1,100,stdin);
  //  //printf("%s\n",cmd1);
  //  //affList(parse(cmd1,' '));
  //  printf("%s\n", startFish(parse(cmd1,' '),buffer1));
  //  affListFish(mainAq.lf);
  //
  //  char cmd3[100];
  //  char buffer3[100];
  //  fgets(cmd3,100,stdin);
  //  //printf("%s\n",cmd1);
  //  //affList(parse(cmd1,' '));
  //  printf("%s\n", addFish(parse(cmd3,' '),buffer3));
  //  affListFish(mainAq.lf);
  //
  //  char cmd2[200];
  //  char buffer2[100];
  //  fgets(cmd2,100,stdin);
  //  //printf("%s\n",cmd2);
  //  //affList(parse(cmd2,' '));
  //  printf("%s", status(parse(cmd2,' '),buffer2));
  //  //affListFish(mainAq.lf);

  // fgets(cmd,100,stdin);
  // printf("%s\n",cmd);
  // affList(parse(cmd,' '));
  // printf("%s\n",saveAquarium(parse(cmd,' '),buffer));
  // printf("\n");

  //DEBUT TEST ADD FISH
//   fgets(cmd,100,stdin);
//   printf("%s\n",cmd);
//   affList(parse(cmd,' '));
//   printf("%s\n", getFishes(parse(cmd,' '),buffer));
//
//
//   freeEnd();
//   return 0;
// }
