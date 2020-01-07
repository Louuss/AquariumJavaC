#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "parser.h"

#define SIZE_LINE 256

void getConfig(FILE * configFile,int res[]){
  int cmpt = 0;
  char s[SIZE_LINE];
  struct List * l;
  while(cmpt < 3){
    fgets(s,SIZE_LINE,configFile);
    if(s[0] != '#' && s[0] != ' ' && s[0] != '\n'){
      l = parse(s,' ');
      res[cmpt] = atoi(l->first->next->next->s);
      cmpt++;
    }
  }
}


int main(int argc, char *argv[]){
  int res[3];
  FILE * fd = fopen(argv[1],"r");
  getConfig(fd,res);
  printf("%d %d %d\n",res[0],res[1],res[2]);
  return 0;
}

