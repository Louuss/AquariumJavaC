#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>

#include <ifaddrs.h>
#include <pthread.h>
#include <signal.h>

#include "parser.h"
#include "command.h"

#define MAX_CLIENT 50//Temporaire, à définir? :3
#define TRUE 1
#define FALSE 0
#define PENDING -1
#define SIZE_LINE 256
#define MAX_SIZE 1500

struct client {
    int socket;
    int last_request;
    int valid;
    char * data_to_send;
    int fish_continuum_requested;
    pthread_mutex_t lock;
};

int display_timeout_value;

struct client clients[MAX_CLIENT];
int fishnet;//Socket d'écoute des clients
struct client netball = {.socket = 0, .last_request = -1, .valid = TRUE, .data_to_send = NULL, .fish_continuum_requested = TRUE};

struct sockaddr_in address;
int addrlen;

sigset_t mask;

pthread_t listen_th;
pthread_t timer_th;

int over = FALSE;

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

void closing() {
    //pthread_cancel(timer_th);
    pthread_exit(NULL);
}

/*TODO :
  - Lire fonction char par char au cas où deux fonctions reçues en même temps
 */

void * listening(void * param) {
    (void) param;
    pthread_sigmask(SIG_BLOCK, &mask, NULL);
    fd_set readfds; //File descriptor de tout les sockets, en mode réception
    int max_sd;// Descripteur le plus élevé
    int sd;//Descripteur en cours de manipulation
    int new_socket; //Descripteur associé à un nouveau socket.
    int activity; //Retour de la fonction vérifiant si un événement a eu lieu au sein du réseau.

    char buffer[1024];//Buffer de lecture des données transmises par les clients
    int valread; //Longueur du message recu

    char password[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, '\0'}; //"Mot de passe" connu uniquement de l'invite de commande pour bénificier d'un status spécial, non tapable par un client classique

    int i = 0;

    while(TRUE) {//Boucle principale => devra être placée dans un thread à l'avenir.
	FD_ZERO(&readfds);//D'abord, on initialise les structures.

	// On ajoute le socket principal à la liste des sockets à observer
	FD_SET(fishnet, &readfds);
	max_sd = fishnet;

	if (netball.socket != 0) {
	    FD_SET(netball.socket, &readfds);
	    max_sd = netball.socket;
	}

	//On regarde ensuite quels sockets sont associés à un client.

	for (i = 0; i < MAX_CLIENT; i++) {
	    sd = clients[i].socket;
	    //si le socket est utilisé
	    if (sd > 0) {
		FD_SET(sd, &readfds);
	    }
	    if (sd > max_sd) {
		max_sd = sd;
	    }
	}

	//Select est un appel bloquant : se débloque si un événement a lieu, ou le timer s'écoule.
	activity = select(max_sd + 1 , &readfds , NULL , NULL , NULL);

	if (activity < 0) {
	    perror("Error on select ");
	}

	//Tout d'abord, on vérifie s'il y a une demande de connection
	if (FD_ISSET(fishnet, &readfds)) {
	    if ((new_socket = accept(fishnet, (struct sockaddr*)&address, (socklen_t*)&addrlen)) <0) {
		perror("Failed accept ");
		exit(1);
	    }
	    //Debug... Evidemment.
	    printf("Come here, fishy fishy~\n");
	    //send(new_socket, greetings, strlen(greetings), 0);

	    for (i = 0; i < MAX_CLIENT; i++) {
                //On chercher le premier élément de notre liste non associé à un socket.
                if( clients[i].socket == -1 ) {
                    clients[i].socket = new_socket;
		    clients[i].data_to_send = malloc(MAX_SIZE * sizeof(char));
                    printf("Adding to list of sockets as %d\n" , new_socket);
                    break;
                }
            }
	}

	if (FD_ISSET(netball.socket, &readfds)) { //Received command from user
	    sd = netball.socket;
	    valread = read(sd, buffer, 1024);

	    if (valread == 0) {
		printf("User closed connection, quitting...\n");
		for (i = 0; i < MAX_CLIENT; i++) {
		    if (clients[i].socket != -1) {
			free(clients[i].data_to_send);
			close(clients[i].socket);
		    }
		}
		close(fishnet);
		free(netball.data_to_send);
		close(sd);
		closing();
	    }

	    if (buffer[valread-1] == '\n') {
		buffer[valread-1] = '\0';//Remplacer le dernier caractère (Newline) par EOF
	    }

	    if (strcmp(buffer, "quit") == 0) {
		send(sd, "Closing server... Bye", 22, 0);
		for (i = 0; i < MAX_CLIENT; i++) {
		    if (clients[i].socket != -1) {
			free(clients[i].data_to_send);
			close(clients[i].socket);
		    }
		}
		close(fishnet);
		free(netball.data_to_send);
		close(sd);
		closing();
	    }

	    sprintf(netball.data_to_send, "%d", sd);

	    printf("Received command : %s (%d)\n", buffer, valread);

	    cmdIsValid(parse(buffer,' '), netball.data_to_send);
	    printf("Result : %s\n", netball.data_to_send);

	    if (atoi(netball.data_to_send) != sd) {
		send(sd, netball.data_to_send, strlen(netball.data_to_send)+1, 0);
	    } else {
		send(sd, "Invalid command", 16, 0);
	    }

	    netball.data_to_send[0] = '\0';//'Cleaner' le buffer après traitement.
	}

	// Puis on regarde si on a reçu des données d'un client.
	for (i = 0; i < MAX_CLIENT; i++) {
	    sd = clients[i].socket;
	    if (FD_ISSET(sd, &readfds)) {
		pthread_mutex_lock(&clients[i].lock);

		if (clients[i].socket == -1) {//Securité au cas où le timer a expire trop tot
		    pthread_mutex_unlock(&clients[i].lock);
		    continue;
		}

		//Si le socket a envoyé un message vide, il s'est déconnecté : fermer la connection de notre côté aussi.
		if ((valread = read(sd, buffer, 1024)) == 0) {
		    char * temp_buf = malloc(sizeof(char) * 20);//Sécurite si utilisateur se déco en force
		    sprintf(temp_buf, "log out");
		    sprintf(clients[i].data_to_send, "%d", sd); //Transmettre l'identifiant socket à l'interpréteur

		    cmdIsValid(parse(temp_buf,' '), clients[i].data_to_send);

                    printf("Host disconnected , socket %d , port %d \n" ,
                          sd, ntohs(address.sin_port));

                    //Réinitialiser la structure du socket : reset les variables, et libérer le buffer de transmission s'il contient quelque chose.
                    close(sd);
                    clients[i].socket = -1;
		    clients[i].valid = FALSE;
		    clients[i].last_request = 0;
		    clients[i].fish_continuum_requested = FALSE;
		    free(clients[i].data_to_send);
		    free(temp_buf);
		    pthread_mutex_unlock(&clients[i].lock);
                }

		else {
		    //On a reçu un message a traiter
		    clients[i].last_request = 0;

                    if (buffer[valread-1] == '\n') {
			buffer[valread-1] = '\0';//Remplacer le dernier caractère (Newline) par EOF
		    }

		    printf("Received something : %s (%d)\n", buffer, valread);
		    printf("%d\n", strcmp(buffer, password));
		    if (strcmp(buffer, password) == 0) { //Message spécial indiquant que c'est le cmd utilisateur : a des droits Admins.
			printf("Greeting Admin\n");
			if (netball.socket != 0) {//Securite si l'admin fait des betises
			    close(netball.socket);
			}
			netball.socket = sd;
			netball.data_to_send = clients[i].data_to_send;
			//Libère le socket client
			clients[i].socket = -1;
			clients[i].valid = FALSE;
			clients[i].last_request = 0;
			clients[i].fish_continuum_requested = FALSE;
			pthread_mutex_unlock(&clients[i].lock);
		        clients[i].data_to_send = NULL;//Ne PAS le libérer, il est maintenant pointé par netball
			printf("Went okay\n");
			continue;
		    }

		    if (!clients[i].valid && strncmp(buffer, "hello", 5) != 0) {
			printf("Client not valid, command refused\n");
			pthread_mutex_unlock(&clients[i].lock);
			continue;
		    }

		    if (strncmp(buffer, "getFishesContinuously", 22) == 0) {
			printf("Client requested fishes continuum\n");
			clients[i].fish_continuum_requested = PENDING;
		    }

		    sprintf(clients[i].data_to_send, "%d", sd); //Transmettre l'identifiant socket à l'interpréteur

		    cmdIsValid(parse(buffer,' '), clients[i].data_to_send);

		    //Traitements spéciaux pour certaines commandes
		    if (atoi(clients[i].data_to_send) != sd) {
			send(sd, clients[i].data_to_send, strlen(clients[i].data_to_send)+1, 0);
		    } else {
			send(sd, "Invalid command", 16, 0);
		    }
		    if (!clients[i].valid && strncmp(clients[i].data_to_send, "greeting", 8) == 0) {
			clients[i].valid = TRUE;
		    }
		    if (clients[i].fish_continuum_requested == PENDING) {
			if (strncmp(clients[i].data_to_send, "NOK", 2) == 0) {
			    clients[i].fish_continuum_requested = FALSE;
			}else {
			    clients[i].fish_continuum_requested = TRUE;
			}
		    }
		    if (strncmp(clients[i].data_to_send, "bye", 4) == 0) { //if buffer == "bye" => déclencher procédure fermeture connexion
			printf("Client quitted\n");
			close(sd);
			clients[i].socket = -1;
			clients[i].valid = FALSE;
			clients[i].last_request = 0;
			clients[i].fish_continuum_requested = FALSE;
			free(clients[i].data_to_send);
			pthread_mutex_unlock(&clients[i].lock);
			continue;
		    } else {
			clients[i].data_to_send[0] = '\0';//'Cleaner' le buffer après émission.
			pthread_mutex_unlock(&clients[i].lock);
		    }
		}
	    }
	}
    }
    pthread_exit(NULL);
}

void * regular_activity(void * param) {
    (void) param;
    
    alarm(1);
    int sig;
    int sd;
    int i = 0;

    while(!over) {

	sigwait(&mask, &sig);
	alarm(1); //On réarme au tout début pour maintenir l'horloge la plus régulière possible

	updateFish();

	for (i = 0; i < MAX_CLIENT; i++) {
	    pthread_mutex_lock(&clients[i].lock);
	    sd = clients[i].socket;

	    if (sd != -1) {
		if (clients[i].last_request == display_timeout_value) {//Le temps de connexion sans message est expiré : on ferme la connexion de notre côté.
		    //ENVOYER UN MESSAGE DE LOG OUT POUR LIBERER LA VIEW
		    char * temp_buf = malloc(sizeof(char) * 20);
		    sprintf(temp_buf, "log out");
		    sprintf(clients[i].data_to_send, "%d", sd); //Transmettre l'identifiant socket à l'interpréteur
		    //parseClientCmd();
		    cmdIsValid(parse(temp_buf, ' '), clients[i].data_to_send);

		    send(sd, clients[i].data_to_send, strlen(clients[i].data_to_send)+1, 0);

		    printf("Ejected host , socket %d , port %d \n" ,
			   sd , ntohs(address.sin_port));

		    //Réinitialiser la structure du socket : reset les variables, et libérer le buffer de transmission s'il contient quelque chose.
		    close(sd);
		    clients[i].socket = -1;
		    clients[i].valid = FALSE;
		    clients[i].last_request = 0;
		    clients[i].fish_continuum_requested = FALSE;
		    free(clients[i].data_to_send);
		    free(temp_buf);
		}

		else {
		    //Incrémenter le temps sans requêtes du client.
		    clients[i].last_request++;

		    //Générer les messages à envoyer si nécessaire (GetFishList)
		    if (clients[i].fish_continuum_requested == TRUE) {
			char * temp_buf = malloc(sizeof(char) * 20);
			sprintf(temp_buf, "getFishes");
			sprintf(clients[i].data_to_send, "%d", sd); //Transmettre l'identifiant socket à l'interpréteur
			cmdIsValid(parse(temp_buf, ' '), clients[i].data_to_send);

			send(sd, clients[i].data_to_send, strlen(clients[i].data_to_send)+1, 0);
			clients[i].data_to_send[0] = '\0';//'Cleaner' buffer.
			free(temp_buf);
		    }
		}
	    }
	    pthread_mutex_unlock(&clients[i].lock);
	}
    }
    pthread_exit(NULL);
}

int main(int argc, char ** argv) {
    (void) argv;
    //Parsing du fichier de configuration
    //printf("%d\n", argc);

    if (argc > 1) {//Disable all logs on production
	freopen ("/dev/null", "w", stdout);
    }

    int value[3];
    int i = 0;
    FILE * configuration = fopen("controller.cfg", "r");
    if (configuration == NULL) {
	printf("Error : couldn't find <controller.cfg>\n");
	exit(1);
    }
    getConfig(configuration, value);

    fclose(configuration);

    printf("%d %d %d\n", value[0], value[1], value[2]);

    srand(time(NULL));

    int port = value[0];
    display_timeout_value = value[1];
    int fish_update_interval = value[2];

    //Génération des sockets clients

    for (i = 0; i < MAX_CLIENT; i++) {
	pthread_mutex_init(&clients[i].lock, NULL);
	clients[i].socket = -1;
	clients[i].last_request = 0;
	clients[i].valid = FALSE;
	clients[i].fish_continuum_requested = FALSE;
	clients[i].data_to_send = NULL;
    }

    //Génération du socket principal

    //netball = {.socket = 0, .last_request = -1, .valid = TRUE, .data_to_send = NULL};//Socket de communication avec le contrôleur.

    if ((fishnet = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
	perror("Socket ");
	exit(1);
    }

    //Expliquer ce à quoi ça sert, mais TRES important!!
    int opt = TRUE;
    if(setsockopt(fishnet, SOL_SOCKET, SO_REUSEADDR, (char *)&opt,
		   sizeof(opt)) < 0 )
	{
	    perror("setsockopt ");
	    exit(EXIT_FAILURE);
	}

    //Preparation au binding...
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = htonl(INADDR_ANY);
    address.sin_port = htons(port);

    //Binding

    if (bind(fishnet, (struct sockaddr *)&address, sizeof(address)) < 0) {
	perror("bind failed ");
	exit(1);
    }

    printf("%d\n", ((struct sockaddr_in*)&address)->sin_addr.s_addr);

    if (listen(fishnet, 5) < 0) {
	perror("listen ");
	exit(1);
    }

    addrlen = sizeof(address);

    printf("Go Fish!\n");

    sigemptyset(&mask);
    sigaddset(&mask, SIGALRM);
    pthread_sigmask(SIG_BLOCK, &mask, NULL);

    pthread_create(&listen_th, NULL, listening, NULL);
    pthread_create(&timer_th, NULL, regular_activity, NULL);

    pthread_join(listen_th, NULL);

    over = TRUE;

    pthread_join(timer_th, NULL);

    endGame();
    exit(0);
}
