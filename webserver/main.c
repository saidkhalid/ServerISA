
#include <stdio.h>
#include <string.h>
#include <sys/types.h> /* See NOTES */
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <signal.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "socket.h"
#include "query.h"
#include "main.h"



int main (){
    /* Arnold Robbins in the LJ of February ’95 , describing RCS */
    int socket_client;
    int socket_serveur = creer_serveur(8080);
    int SIZE=256;
    char message[SIZE];
    char messageOk[256] = "HTTP/1.1 200 OK\nConnection: close\nContent-Length: 17\n";
    char messageError[256] = "HTTP/1.1 400 Bad Request\nConnection: close\nContent-Length: 17\n400 Bad request\n" ;
    int pid;
    http_request request; //la fameuse structure qui sera modifier par parse_http_request
    request.method = 0;
    request.major_version = 0 ;
    request.minor_version = 0 ;
    request.url = 0;
    
    if(socket_serveur==-1){
        return 1;
    }
    
    while(1){
        socket_client = accept(socket_serveur,NULL,NULL);
        if (socket_client == -1){
            perror ( " accept xd " );
            continue;
            /* traitement d ’ erreur */
        }
        
        FILE * fd=fdopen(socket_client,"w+");
        if(fd ==NULL){
            printf("erreurr");
        }
        pid=fork();
        if(pid==0){
            if(parse_http_request(fgets_or_exit(message,SIZE,fd), &request)==1){
                fprintf(fd, "%s", messageOk);
                send_response(fd,200,"OK"," OK \r\n");
            }else{
                fprintf(fd, "%s", messageError);
                send_response(fd , 400 , "Bad Request" , " Bad request \r\n" );

            }
        
            
            
            fflush(fd);
            
            
            
            /* On peut maintenant dialoguer avec le client */
            /*const char * message_bienvenue = " Bonjour, bienvenue sur mon serveur.\n," ;
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
             }*/
            fclose(fd);
            exit(0);
        }else{
            fclose(fd);
        }
    }
    initialiser_signaux();
    close(socket_client);
    
    return 0;
}

