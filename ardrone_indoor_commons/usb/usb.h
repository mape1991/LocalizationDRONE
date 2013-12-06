/* 
 * File:   usb.h
 * Author: julien
 *
 * Created on November 19, 2013, 10:54 AM
 */

#ifndef USB_H
#define	USB_H

// included here because the speed has definitions such as B9600
#include <termios.h>

/**
 * parity: 0 for 8n1
 * speed : B9600 (by instance..), unit in bps
 * should_block : if equals 1 then timeout of 0.5 s
 */
int usb_init(char *portname, int speed, int parity, int should_block);
void usb_write(char *message, int message_size);
void usb_write_char(char message);
int usb_read(char *message, int message_size);
void usb_close();

#endif	/* USB_H */

