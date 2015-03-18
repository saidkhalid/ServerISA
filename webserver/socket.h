# ifndef __SOCKET_H__
# define __SOCKET_H__
/* * Crée une socket serveur qui écoute sur toute les interfaces IPv4
de la machine sur le port passé en paramètre . La socket retournée
doit pouvoir être utilisée directement par un appel à accept .
La fonction retourne -1 en cas d ’ erreur ou le descripteur de la
socket créée . */

<<<<<<< HEAD
int creer_serveur (int port );
=======
int creer_serveur(int port);
>>>>>>> ba8644a6eda29a3d37aaae59efd1ce3984148ec2
void initialiser_signaux();
void traitement_signal(int sig);
# endif
