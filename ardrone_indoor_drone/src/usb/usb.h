#ifndef USB_H
#define USB_H

int set_interface_attribs (int fd, int speed, int parity);
void set_blocking (int fd, int should_block);

#define PORT_NAME "/dev/ttyO3"

extern int fd;

#endif
