/* 
 * File:   usb.h
 * Author: julien
 *
 * Created on November 19, 2013, 10:54 AM
 */

#ifndef USB_H
#define	USB_H

#define USB_PORT_NAME "/dev/ttyUSB0"
#define USB_BUFFER_MAX_SIZE 100

int usb_init(char *portname);
void usb_write(char *message, int message_size);
void usb_write_char(char message);
int usb_read(char *message, int message_size);
void usb_close();

#endif	/* USB_H */

