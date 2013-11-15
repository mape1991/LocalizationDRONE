int receptionudp(int port, int lg_mesg_emis, char * message, struct sockaddr_in * sender);
void envoiudp(int port, struct sockaddr_in adr_distant, char * message);
