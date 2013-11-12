#include "drone_comm.h"

#ifdef DRONE_COMM_ON 

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

void retrieve_timevals(int beacon_id){
    gettimeofday(tbalise+beacon_id,NULL);
    printf("Beacon %d : %d,%d s\n", beacon_id, (int)tbalise[beacon_id].tv_sec, (int)tbalise[beacon_id].tv_usec);
}

void send_time(char * dest){
      int sock;
      //struct hostent *hp;
      struct sockaddr_in adr_distant;
      int lg_adr_distant = sizeof(adr_distant);
      //struct in_addr adresse;
      int error_type = ERROR_TYPE_NONE;

      printf("\n   Drone communication thread : initialisation\n\n");
      // socket creation		
      if ((sock = socket(AF_INET, SOCK_DGRAM, 0))==-1)
      {
              printf("\n   Drone communication thread : Error during the socket creation\n");
              error_type = ERROR_TYPE_SOCKET_CREATION;
      }

      //affectation domaine et n° de port
      memset((char*) &adr_distant, 0, sizeof(adr_distant));
      adr_distant.sin_family = AF_INET;
      adr_distant.sin_port = PORT_DRONE;

      //affectation @IP
      inet_aton(dest, &adr_distant.sin_addr);

      // construction du message puis envoi
      printf("\n     Drone communication thread : Sent %d bytes\n\n", NUM_BEACONS*sizeof(struct timeval));
      sendto(sock, (void *) tbalise, NUM_BEACONS*sizeof(struct timeval), 0, (struct sockaddr*) &adr_distant, lg_adr_distant);

      // fermeture du socket
      close(sock);
      printf("\n     Drone communication thread : end of communication\n\n");
}


int send_to_server(int argc, char ** argv){
   int i;
   for (i=0;i<NUM_BEACONS;i++){
        retrieve_timevals(i);
   }
   send_time(argv[1]);

   return(0);
}

#endif
