#include <stdio.h>
#include <string.h>
#include "socket.h" 
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>

#include <signal.h>



int main (){


/* Arnold Robbins in the LJ of February ’95 , describing RCS */

	int socket_client ;
	int socket_serveur = creer_serveur(8080);
	int ret;
	char message[256];
	int pid;
	
	while(1){
		socket_client = accept(socket_serveur, NULL, NULL);
		pid=fork();
		if(pid==0){
			
			if (socket_client == -1){
				perror ( " accept " );
				/* traitement d ’ erreur */
			}
			/* On peut maintenant dialoguer avec le client */
	
			const char * message_bienvenue = " Bonjour, bienvenue sur mon serveur.\n" ;
			int i;
			
			for (i=1 ; i<11; i++){
				if (write(socket_client, message_bienvenue, strlen(message_bienvenue))==-1){
					perror("write");
				}
			}

			while ((ret = read(socket_client, message, 256))>0){ 		
				if (write(socket_client, message, ret) == -1){
					perror("write");
				}
			}
			
			exit(0);
		}
		initialiser_signaux();
		close(socket_client);
	}
}
