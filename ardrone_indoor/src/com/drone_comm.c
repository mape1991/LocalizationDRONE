#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>

#define PORT_ENVOI_DRONE 7000
#define NB_BEACON 4
struct timeval tbalise[NB_BEACON];

//récupère le temps pour la balise indiquée
void retrieve_timevals(int balise){
    gettimeofday(tbalise+balise,NULL);
    printf("Beacon %d : %d,%d s\n", balise, (int)tbalise[balise].tv_sec, (int)tbalise[balise].tv_usec);
}

//envoie un message
void send_time(char * dest){
      int sock;
      //struct hostent *hp;
      struct sockaddr_in adr_distant;
      int lg_adr_distant = sizeof(adr_distant);
      //struct in_addr adresse;

      // création du socket	
      if ((sock = socket(AF_INET, SOCK_DGRAM, 0))==-1)
      {
              printf("Echec de création du socket\n");
              exit(1);
      }

      //affectation domaine et n° de port
      memset((char*) &adr_distant, 0, sizeof(adr_distant));
      adr_distant.sin_family = AF_INET;
      adr_distant.sin_port = PORT_ENVOI_DRONE;

      //affectation @IP
      inet_aton(dest, &adr_distant.sin_addr);

      // construction du message puis envoi
      printf("SOURCE : Envoi de %d octets.\n", NB_BEACON*sizeof(struct timeval));
      sendto(sock, (void *) tbalise, NB_BEACON*sizeof(struct timeval), 0, (struct sockaddr*) &adr_distant, lg_adr_distant);

      // fermeture du socket
      close(sock);
      printf("SOURCE : fin émission\n");
}


int communicate_with_drone(int argc, char ** argv){
      int i;
      for (i=0;i<NB_BEACON;i++){
              retrieve_timevals(i);
      }
      send_time(argv[1]);

      return(0);
}
