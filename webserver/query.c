
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

void send_status(FILE *client , int code , const char *reason_phrase){
    //code=404;
    char http[256];
    //reason_phrase="NOT FOUND \n";
    int m=1,M=1;
    
    fprintf(client,"%s HTTP/%d.%d %d %s",http,M,m,code,reason_phrase);
}

void send_response(FILE *client , int code , const char *reason_phrase ,const char *message_body){
    
    send_status(client,code,reason_phrase);
    if(fprintf(client, "Connection: close\r\nContent-Length: %d\r\nContent-Type: text/plain\r\n\r\n%s",(int)strlen(message_body),message_body)<0){
        perror("error send_response");
    }
    
}
char * rewrite_url(char * url){
    char * nv_url;
    nv_url=strchr(url,'?');
    if(nv_url!=NULL){
        *url='\0';
    }
    if(strcmp(url,"/")==0){
        strcat(url,"monsite.html");
    }
    
    return url;
}

int check_and_open ( const char * url , const char * document_root ){
    int fd;
    char buf[1024];
    strcpy(buf,document_root);
    if((fd=open(strcat(buf,url),O_RDONLY))==-1){
        perror("error open");
        return -1;
        exit(0);
    }
    return fd;
}

int get_file_size(int fd){
    struct stat buf;
    if(fstat(fd, &buf) != 0){
        perror("error get_file_size");
        return 0;
    }
    return buf.st_size;
}

int copy(int in, int out) {
    int size;
    char buf[1024];
    
    if ((size = read(in, buf, 1024))==-1){
        perror("error copy : read");
        return -1;
    }
    while (size > 0) {
        if ((write(out, buf, size)== 0)){
            perror("error copy : write");
            return -1;
        }
        if ((size = read(in, buf, size))==-1) {
            perror("error copy : read");
            return -1;
        }
    }
    return out;
}

int parse_http_request ( const char * request_line , http_request * request ){
    char slash[256];
    char get[3]="GET";
    char url[256],http[256];
    int M,m;
    /*
     * Vérifie si le premier mot de la requete est bien "GET"
     */
    printf("%s",request_line);
    if((request_line[0]==get[0]) && (request_line[1]==get[1]) && (request_line[2]==get[2])){
        //perror("ligne");
        printf("GET ok \n");
        request->method = HTTP_GET;
        /*
         * Stocke la requete dans les variables définies plus haut et vérifie que le contenu de M et m est le bon.
         */
        sscanf(request_line, "%s %s HTTP/%d.%d %s",url,http,&M,&m,slash);
        if((M==1) || (m==0 && m==1)){
            // printf("erreur de saisie \n");
            request->minor_version = m;
            request->major_version = M;
            /*
             * La requete est valide que si la forme de l'entete est vérifié (conditions précédentes) et le premier caractère qui le suive est '/'
             */
            
            if (slash[0]=='/') {
                //if (message[0] == '\n' || (message[0] == '\r' && message[1] == '\n')) {
                //fprintf(fd, "%s", "message is vide");
                return 1;
            }
        }
    }
    
    return 0;
}

char * fgets_or_exit ( char * message , int size , FILE * fd ){
    /*
     * si le client est deconnecté on interromp le processus
     */
    if(fd==NULL){
        perror("fgets");
        exit(1);
    }
    
    /*
     * si le client est connecté on fait un appel à fgets
     */
    return fgets(message,size,fd);
    /*printf("%s",url);
     fclose(fd);
     exit(1);*/
}

/*void skip_headers(FILE *client){
	fgets_or_exit(
 }*/
