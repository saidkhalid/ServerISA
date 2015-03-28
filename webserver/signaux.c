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
#include "main.h"
#include "query.h"
#include "signaux.h"

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