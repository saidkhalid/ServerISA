#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <netinet/ip.h>
 #include <signal.h>

#include "socket.h"

void traitement_signal(int sig){
	if(waitpid(-1,0,WNOHANG)>-1){
	printf ( " Signal%dreçu \n ",sig );
	}
}
void initialiser_signaux(){
	struct sigaction sa ;
	sa.sa_handler = traitement_signal ;
	sigemptyset (& sa . sa_mask );
	sa.sa_flags = SA_RESTART ;
	if (signal(SIGPIPE ,SIG_IGN ) == SIG_ERR ){
		perror ("signal");
	}
	if (sigaction ( SIGCHLD , & sa , NULL ) == -1){
	perror ( " sigaction ( SIGCHLD ) " );
	exit(1);
	}
}

int creer_serveur(int port){
	int socket_serveur ;
	int optval = 1;
	socket_serveur = socket(AF_INET, SOCK_STREAM, 0);

	if(socket_serveur == -1){
		perror (" socket_serveur ");
		return -1;
		/* traitement de l ’ erreur */
	}

	/* Utilisation de la socket serveur */
	struct sockaddr_in saddr ;
	saddr.sin_family = AF_INET; /* Socket ipv4 */
	saddr.sin_port = htons(port); /* Port d ’ écoute */
	saddr.sin_addr.s_addr = INADDR_ANY; /* écoute sur toutes les interfaces */

	if (bind(socket_serveur, (struct sockaddr *)&saddr, sizeof(saddr)) == -1){
		perror ( " bind socker_serveur " );
		return -1;
		/* traitement de l ’ erreur */
	}
	
	if (listen(socket_serveur, 10) == -1){
		perror ( " listen socket_serveur " );
		return -1;
		/* traitement d ’ erreur */
	}
	
	if ( setsockopt ( socket_serveur, SOL_SOCKET , SO_REUSEADDR , & optval , sizeof ( int )) == -1){
		perror ( " Can not set SO_REUSEADDR option " );
	}
	
	

	
	return socket_serveur;	
}
