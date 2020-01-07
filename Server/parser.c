#include "parser.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "command.h"

void freeNode(struct Node * n){
  free(n->s);
  free(n);
}

void freeList(struct List * l){
    if(l->len == 0){
	free(l);
	return;
    }
    struct Node * current = l->first;
    //printf("%p\n", current);
    struct Node * tmp = NULL;
    if (current != NULL){
	//printf("%p, %p\n", current, current->next);
	while (current->next != NULL){
	    tmp = current->next;
	    freeNode(current);
	    current = tmp;
	    //printf("%p, %p\n", current, current->next);
	}
	freeNode(current);
    }
    free(l);
}

void add(char * src,struct List*l){
  struct Node * n = malloc(sizeof(struct Node));
  n->s = malloc(sizeof(char)*strlen(src)+1);
  n->next = NULL;
  strcpy(n->s,src);
  if(l->len == 0){
    l->last = n;
    l->first = n;
    l->len = 1;
    return;
  }
  l->last->next = n;
  l->last = n;
  l->len = l->len +1;
}


void affList(struct List * l){
  if(l->len <= 0){
    return;
  }
  int i =0;
  struct Node * n = l->first;
  for(i=0;i<l->len;i++){
      //printf("%s\n",n->s);
    n = n->next;
  }
}

char *get_sub_part(char * main_part, char sep){
    if (strchr(main_part,sep) == NULL) {
	     return main_part;
    }
    return strndup(main_part, strchr(main_part,sep) - main_part);
    /*int len = strchr(main_part,sep) - main_part;
    if(len > 0)
	return strndup(main_part,len); // attention sa fait un malloc
	return main_part;*/
}

struct List * parse(char * src,char sep){
    if (src[strlen(src)-1] == '\n') {
	     src[strlen(src)-1] = '\0';
    }

  struct List * res = malloc(sizeof(struct List));
  res->len = 0;
  char * cpy = malloc(sizeof(char)*strlen(src)+1);
  char * freeCpy = cpy;
  strcpy(cpy,src);

  char *part = get_sub_part(cpy,sep);
  unsigned int len = strlen(part);
  add(part,res);
  if(!strcmp(part,cpy)){
      //free(part);
    free(cpy);
    return res;
  }
  cpy = cpy + len + 1; //Rajouter sécu si commande en un mot (cf en dessous)
  free(part);

  while(strlen(cpy) > 0){
      //printf("%s -> %lu\n", cpy, strlen(cpy));
      part = get_sub_part(cpy,sep);
      len = strlen(part);
      add(part,res);
      //free(part);
      if (len == strlen(cpy)) {
	  cpy = cpy + len;//Quand arrivait à la fin du mot : overflow (dans le meilleur des cas)
      } else {
	  free(part);
	  cpy = cpy + len + 1;
      }
      //printf("? = %lu\n", strlen(cpy));

  }
  free(freeCpy);
  return res;
}

int isFormatAddViewValid(char * src){
  struct List * p1 = parse(src,'x');
  struct List * p2 = parse(p1->first->next->s,'+');
  //printf("%d, %d\n", p1->len, p2->len);
  if(p1->len == 2){
    if(atoi(p1->first->s) != 0 || (p1->first->s)[0] == '0'){
      if(p2->len == 3){
	       if( ( atoi(p2->first->s) != 0 || (p2->first->s)[0] == '0') &&
	        ( atoi(p2->first->next->s) != 0 || (p2->first->next->s)[0] == '0') &&
	         ( atoi(p2->first->next->next->s) != 0 || (p2->first->next->next->s)[0] == '0') ){
             freeList(p1);
             freeList(p2);
	           return 1;
        }
      }
    }
  }
  freeList(p1);
  freeList(p2);
  return 0;
}

int isFormatAddFishValid(struct List * l, char * buffer){
  if (l->len == 6){
    if (!strcmp(l->first->next->next->s, "at")){
      int n = strlen(l->first->next->next->next->s);
      if ((l->first->next->next->next->s)[n-1] == ','){
	struct List * p1 = parse(l->first->next->next->next->s, 'x');
	if (p1->len == 2){
	  if((atoi(p1->first->s) != 0 || (p1->first->s)[0] == '0') &&
	     (atoi(p1->first->next->s) != 0 || (p1->first->next->s)[0] == '0')) {
	    n = strlen(l->first->next->next->next->next->s);
	    if ((l->first->next->next->next->next->s)[n-1] == ','){
	      struct List * p2 = parse(l->first->next->next->next->next->s, 'x');
	      if (p2->len == 2){
		if((atoi(p2->first->s) != 0 || (p2->first->s)[0] == '0') &&
		   (atoi(p2->first->next->s) != 0 || (p2->first->next->s)[0] == '0')) {
		  printf("AddFish : OK\n");
		  addFish(l,buffer);
		  freeList(p1);
		  freeList(p2);
      freeList(l);
		  return 1;
		}
	      }
	    }
	  }
	}
	freeList(p1);
      }
    }
    printf("NOK : Invalid args\n");
    freeList(l);
    return 0;
  }
  printf("NOK : AddFish expects 6 args\n");
  freeList(l);
  return 0;
}

int isCmdLoadValid(struct List * l, char * buffer){
  if(l->len == 2){
    printf("load OK\n");
    loadAquarium(l,buffer);
    freeList(l);
    return 1;
  }
  else {
    printf("load expects one arg\n");
    freeList(l);
    return 0;
  }
}

int isCmdShowValid(struct List * l,char * buffer){
  if(l->len == 2){
    printf("show : OK\n");
    showAquarium(l,buffer);
    freeList(l);
    return 1;
  }
  else{
    printf("NOK : show expects one arg\n");
    freeList(l);
    return 0;
  }
}


int isCmdAddValid(struct List * l,char * buffer){
  if(l->len == 4){
    if(!strcmp(l->first->next->s,"view")){
      if( ((l->first->next->next->s)[0] == 'N') &&
	  ( ( atoi((l->first->next->next->s)+1) != 0 ) || ( (l->first->next->next->s)[1] == '0') )  &&
	  isFormatAddViewValid(l->first->next->next->next->s)){
	printf("add : OK\n");
	addView(l,buffer);
  freeList(l);
	return 1;
      }
      else {
	printf("NOK : args invalid\n");
  freeList(l);
	return 0;
      }
    }
    else {
      printf("NOK : COMMAND INVALID\n");
      freeList(l);
      return 0;
    }
  }
  else {
    printf("NOK : add view expects 2 args\n");
    freeList(l);
    return 0;
  }
}

int isCmdDelValid(struct List * l,char * buffer){
  if(l->len == 3){
    if(!strcmp(l->first->next->s,"view")){
      if( ((l->first->next->next->s)[0] == 'N') &&
	  ( ( atoi((l->first->next->next->s)+1) != 0 ) || ( (l->first->next->next->s)[1] == '0') ) ){
	printf("del : OK\n");
	delView(l,buffer);
  freeList(l);
	return 1;
      }
      else {
	printf("NOK : args invalid\n");
  freeList(l);
	return 0;
      }
    }
    else {
      printf("NOK : COMMAND INVALID\n");
      freeList(l);
      return 0;
    }
  }
  else {
    printf("NOK : del view expects 1 arg\n");
    freeList(l);
    return 0;
  }
}

int isCmdSaveValid(struct List * l,char * buffer){
  if(l->len == 2){
    printf("save : OK\n");
    saveAquarium(l,buffer);
    freeList(l);
    return 1;
  }
  else{
    printf("NOK : save expects one arg\n");
    freeList(l);
    return 0;
  }
}


int isCmdStatusValid(struct List * l, char * buffer){
  if(l->len == 1){
    printf("status : OK\n");
    status(l,buffer);
    freeList(l);
    return 1;
  }
  else{
    printf("NOK : status has no arg\n");
    freeList(l);
    return 0;
  }
}

int isCmdDelFishValid(struct List * l, char * buffer){
  if(l->len == 2){
    printf("delFish : OK\n");
    delFish(l,buffer);
    freeList(l);
    return 1;
  }
  else{
    printf("NOK : delFish expects one arg\n");
    freeList(l);
    return 0;
  }
}

int isCmdStartFishValid(struct List * l, char * buffer){
  if(l->len == 2){
    printf("startFish : OK\n");
    startFish(l,buffer);
    freeList(l);
    return 1;
  }
  else{
    printf("NOK : startFish expects one arg\n");
    freeList(l);
    return 0;
  }
}

int isCmdGetFishesValid(struct List * l, char * buffer){
  if (l->len == 1){
    printf("getFishes : OK\n");
    getFishes(l,buffer);
    freeList(l);
    return 1;
  }
  else{
    printf("NOK : getFishes does not expect arg\n");
    freeList(l);
    return 0;
  }
}


int isCmdGetFishesContinuouslyValid(struct List * l, char * buffer){
  if (l->len == 1){
    printf("getFishesContinuously : OK\n");
    getFishesContinuously(l,buffer);
    freeList(l);
    return 1;
  }
  else{
    printf("NOK : getFishesContinuously does not expect arg\n");
    freeList(l);
    return 0;
  }
}

int isCmdLogValid(struct List * l,char * buffer){
  if (l->len == 2 && !strcmp(l->first->next->s, "out")){
    printf("log : OK\n");
    logOut(l,buffer);
    freeList(l);
    return 1;
  }
  else{
    printf("NOK : log excepts 1 arg\n");
    freeList(l);
    return 0;
  }
}

int isCmdPingValid(struct List * l, char * buffer){
  if (l->len == 2 && (atoi(l->first->next->s) != 0 || (l->first->next->s)[0]=='0')){
    printf("ping : OK\n");
    pingPong(l,buffer);
    freeList(l);
    return 1;
  }
  else{
    printf("NOK : ping expects 1 int arg\n");
    freeList(l);
    return 0;
  }
}

int isCmdHelloValid(struct List * l, char * buffer){
  if (l->len == 1){
    printf("hello : OK\n");
    hello(l,buffer);
    freeList(l);
    return 1;
  }
  else if (l->len == 4){
    if (!strcmp(l->first->next->s, "in") &&
	!strcmp(l->first->next->next->s, "as") &&
	(l->first->next->next->next->s)[0] == 'N' &&
	((atoi((l->first->next->next->next->s)+1) != 0 ) || ( (l->first->next->next->next->s)[1] == '0'))){
      printf("hello : OK\n");
      hello(l,buffer);
      freeList(l);
      return 1;
    }
    else {
      printf("NOK : invalid args\n");
      freeList(l);
      return 0;
    }
  }
  printf("NOK : hello expects 0 or 3 args\n");
  freeList(l);
  return 0;
}


int cmdIsValid(struct List * l,char * buffer){
  if(l == NULL || l->len == 0){
    printf("COMMAND INVALID : NO COMMAND \n");
    return 0;
  }
  char * cmd = l->first->s;
  struct Node * p = l->first;
  int i = 0;
  for (i = 0; i < l->len; i++) {
      printf("%s\n", p->s);
      p = p->next;
  }
  if(!strcmp(cmd,"load\n") || !strcmp(cmd,"load")){
      return isCmdLoadValid(l, buffer);
  }

  else if(!strcmp(cmd,"show\n") || !strcmp(cmd,"show")){
    return isCmdShowValid(l, buffer);
  }

  else if(!strcmp(cmd,"add\n") || !strcmp(cmd,"add")){
      printf("Here\n");
    return isCmdAddValid(l, buffer);
  }

  else if(!strcmp(cmd,"del\n") || !strcmp(cmd,"del")){
    return isCmdDelValid(l, buffer);
  }

  else if(!strcmp(cmd,"save\n") || !strcmp(cmd,"save")){
    return isCmdSaveValid(l, buffer);
  }

  else if(!strcmp(cmd,"status\n") || !strcmp(cmd,"status")){
    return isCmdStatusValid(l, buffer);
  }

  else if(!strcmp(cmd,"addFish\n") || !strcmp(cmd,"addFish") ){
     return isFormatAddFishValid(l, buffer);
  }


  else if(!strcmp(cmd,"delFish\n") || !strcmp(cmd,"delFish")){
    return isCmdDelFishValid(l, buffer);
  }

  else if(!strcmp(cmd,"startFish\n") || !strcmp(cmd,"startFish")){
    return isCmdStartFishValid(l, buffer);
  }

  else if(!strcmp(cmd,"getFishes\n") || !strcmp(cmd,"getFishes")){
    return isCmdGetFishesValid(l, buffer);
  }

  else if(!strcmp(cmd,"hello\n") || !strcmp(cmd,"hello")){
    return isCmdHelloValid(l, buffer);
  }

  else if(!strcmp(cmd,"getFishesContinuously\n") || !strcmp(cmd,"getFishesContinuously")){
    return isCmdGetFishesContinuouslyValid(l, buffer);
  }

  else if(!strcmp(cmd,"log\n") || !strcmp(cmd,"log")){
    return isCmdLogValid(l, buffer);
  }

  else if(!strcmp(cmd,"ping\n") || !strcmp(cmd,"ping")){
    return isCmdPingValid(l, buffer);
  }

  else{
    printf("NOK : COMMAND INVALID\n");
    freeList(l);
    return 0;
  }
}

/*
int main(int argc, char *argv[]){

  //affList( parse(argv[1],':') )

  char cmd[256];
  printf("Veuillez entrer une commande\n");
  printf("escape : <q>\n");
  while(1){
    fgets(cmd,256,stdin);
    if(cmd[0] == 'q' && strlen(cmd) == 2)
      return 0;
    printf("resultat du parser :\n");
    affList(parse(cmd,' '));
    cmdIsValid(parse(cmd,' '));
  }


  return 0;
}
*/
