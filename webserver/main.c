#include <stdio.h>
#include <string.h>
#include "socket.h"
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <signal.h>

enum http_method {
    HTTP_GET ,
    HTTP_UNSUPPORTED ,
};
typedef struct
{
    enum http_method method ;
    int major_version ;
    int minor_version ;
    char * url ;
} http_request ;

int parse_http_request ( const char * request_line , http_request * request ){
    char slash[256];
    char get[3]="GET";
    
    char url[256],http[256];
    int M,m;
    
    if((request_line[0]==get[0]) && (request_line[1]==get[1]) && (request_line[2]==get[2])){
        //perror("ligne");
        request->method = HTTP_GET;
        
        sscanf(request_line, "%s %s HTTP/%d.%d %s",url,http,&M,&m,slash);
        if((M==1) || (m==0 && m==1)){
            // printf("erreur de saisie \n");
            request->minor_version = m;
            request->major_version = M;
            
            if (slash[0]=='/') {
                //if (message[0] == '\n' || (message[0] == '\r' && message[1] == '\n')) {
                //fprintf(fd, "%s", "message is vide");
                return 1;
            }
        }
    }
    return 0;
        //fprintf(fd,"%s",message);
}


char * fgets_or_exit ( char * message , int size , FILE * fd ){
    
    if(fd==NULL){
        perror("fgets");
        exit(1);
    }
    return fgets(message,size,fd);
    //printf("%s",url);
    fclose(fd);
    exit(1);
}


int main (){
    
    
    /* Arnold Robbins in the LJ of February ’95 , describing RCS */
    
    int socket_client;
    int socket_serveur = creer_serveur(8080);
    int SIZE=256;
    char message[SIZE];
    char messageOk[256] = "HTTP/1.1 200 OK\nConnection: close\nContent-Length: 17\n";
    char messageError[256] = "HTTP/1.1 400 Bad Request\nConnection: close\nContent-Length: 17\n400 Bad request\n" ;
    
    int pid;
    
    http_request * request = NULL ;
    
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
        pid=fork();
        if(pid==0){
            
            if(parse_http_request(fgets_or_exit(message,SIZE,fd), request)==1){
                fprintf(fd, "%s", messageOk);
            }else{
                fprintf(fd, "%s", messageError);
            }
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
             }
             
             exit(0);*/
            
            
            
            
        }
    }
    
    initialiser_signaux();
    close(socket_client);
}

