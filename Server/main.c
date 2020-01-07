#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>

#include "parser.h"

#define SIZE_LINE 256

int socket_fd;
struct sockaddr_in address;
int addrlen;

void getConfig(FILE * configFile,int res[]){
    int cmpt = 0;
    char s[SIZE_LINE];
    struct List * l;
    while(cmpt < 3){
        if (fgets(s,SIZE_LINE,configFile) == NULL) {
	    printf("An error was encountered with the configuration file\n");
	    exit(1);
	}
	if(s[0] != '#' && s[0] != ' ' && s[0] != '\n'){
	    l = parse(s,' ');
	    res[cmpt] = atoi(l->first->next->next->s);
	    cmpt++;
	    freeList(l);
	}
    }
}

int main() {
    pid_t pid = fork();
    if (pid == 0) {
	char * argv[] = {"server", "1",  NULL};
        execv("./server", argv);
    } else {
	int value[3];
	FILE * configuration = fopen("controller.cfg", "r");
	if (configuration == NULL) {
	    printf("Error : couldn't find <controller.cfg>\n");
	    exit(1);
	}
	getConfig(configuration, value);

	int port = value[0];

	if ((socket_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
	    perror("Socket ");
	    exit(1);
	}

	address.sin_family = AF_INET;   
	address.sin_addr.s_addr = htonl(INADDR_ANY);
	address.sin_port = htons(port);

	addrlen = sizeof(address);

	while (connect(socket_fd, (struct sockaddr *)&address, (socklen_t) addrlen) < 0) {
	    sched_yield();
	}
	sched_yield();
	
	int status;
	int len = 0;
	char buf[1024] = {1, 2, 3, 4, 5, 6, 7, 8, 9, '\0'};
        send(socket_fd, buf, 10, 0);
	while (waitpid(pid, &status, WNOHANG) == 0) {
	    printf("=>$ ");
	    fgets(buf, 1500, stdin);
	    buf[strlen(buf)] = '\0';
	    send(socket_fd, buf, strlen(buf), 0);
	    len = read(socket_fd, buf, 1024);
	    buf[len] = '\0';
	    printf("%s\n", buf);
	}
    }
    return 0;
}
