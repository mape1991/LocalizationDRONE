#ifndef USB_H
#define USB_H

#include <errno.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/signal.h>
#include <sys/types.h>

int set_interface_attribs (int fd, int speed, int parity);
void set_blocking (int fd, int should_block);

#define PORT_NAME "/dev/usbdev1.2"

extern int fd;

#endif
