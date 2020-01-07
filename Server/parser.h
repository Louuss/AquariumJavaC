#ifndef PARSER_H
#define PARSER_H


struct Node {
  char * s;
  struct Node * next;
};

struct List{
  int len;
  struct Node *first;
  struct Node *last;
};

void add(char * src,struct List*l);
void affList(struct List * l);
char *get_sub_part(char * main_part, char sep);
struct List * parse(char * src,char sep);
int cmdIsValid(struct List * l,char * buffer);
void freeList(struct List * l);



#endif
