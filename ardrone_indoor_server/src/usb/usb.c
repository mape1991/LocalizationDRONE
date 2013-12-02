#include "usb.h"

#include <errno.h>
#include <termios.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/signal.h>
#include <sys/types.h>

int fd;

int usb_set_interface_attribs (int fd, int speed, int parity)
{
        struct termios tty;
        memset (&tty, 0, sizeof tty);
        if (tcgetattr (fd, &tty) != 0)
        {
                printf ("error %d from tcgetattr \n", fd);
                return -1;
        }

        cfsetospeed (&tty, speed);
        cfsetispeed (&tty, speed);

        tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS8;     // 8-bit chars
        // disable IGNBRK for mismatched speed tests; otherwise receive break
        // as \000 chars
        tty.c_iflag &= ~IGNBRK;         // ignore break signal
        tty.c_lflag = 0;                // no signaling chars, no echo,
                                        // no canonical processing
        tty.c_oflag = 0;                // no remapping, no delays
        tty.c_cc[VMIN]  = 0;            // read doesn't block
        tty.c_cc[VTIME] = 5;            // 0.5 seconds read timeout

        tty.c_iflag &= ~(IXON | IXOFF | IXANY); // shut off xon/xoff ctrl

        tty.c_cflag |= (CLOCAL | CREAD);// ignore modem controls,
                                        // enable reading
        tty.c_cflag &= ~(PARENB | PARODD);      // shut off parity
        tty.c_cflag |= parity;
        tty.c_cflag &= ~CSTOPB;
        tty.c_cflag &= ~CRTSCTS;

        if (tcsetattr (fd, TCSANOW, &tty) != 0)
        {
                printf ("error %d from tcsetattr \n", fd);
                return -1;
        }
        return 0;
}

void usb_set_blocking (int fd, int should_block)
{
        struct termios tty;
        memset (&tty, 0, sizeof tty);
        if (tcgetattr (fd, &tty) != 0)
        {
                printf ("error %d from tggetattr \n", fd);
                return;
        }

        tty.c_cc[VMIN]  = should_block ? 1 : 0;
        tty.c_cc[VTIME] = 5;            // 0.5 seconds read timeout

        if (tcsetattr (fd, TCSANOW, &tty) != 0)
                printf ("error %d setting term attributes \n", fd);
}

int usb_init(char *portname)
{
   fd  = open (portname, O_RDWR | O_NOCTTY | O_SYNC);

   if (fd < 0)
   {
        printf ("usb error %d opening %s \n", fd, portname);
        return 0;
   }
   else
   {
      printf("usb open port %s \n", portname);
   }

   usb_set_interface_attribs (fd, B9600, 0);  // set speed to 115,200 bps, 8n1 (no parity)
   //set_interface_attribs (fd2, B115200, 0);  // set speed to 115,200 bps, 8n1 (no parity)
   printf("usb set port\n");
   
   usb_set_blocking (fd, 0);                // set no blocking
   //set_blocking (fd2, 0);                // set no blocking
   printf("usb set blocking\n");
   
   return 0;
}

void usb_write(char *message, int message_size) 
{
   write (fd, message, message_size);
}

void usb_write_char(char message)
{
   char message_arr[1];
   message_arr[0] = message;
   usb_write(message_arr, 1);
}

/**
 * 
 * @return size of buffer 
 */
int usb_read(char *message, int message_size)
{
   return read (fd, message, message_size);
}

void usb_close()
{
   close(fd);
}
