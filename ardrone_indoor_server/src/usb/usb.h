/* 
 * File:   usb.h
 * Author: julien
 *
 * Created on November 19, 2013, 10:54 AM
 */

#ifndef USB_H
#define	USB_H

#include "../global_config.h"

#ifdef USB_ON

#define USB_PORT_NAME "/dev/ttyUSB0"
//#define USB_PORT_NAME "/dev/bus/usb/001"
#define USB_BUFFER_MAX_SIZE 100

int usb_init(char *portname);
void usb_write(char *message, int message_size);
void usb_write_char(char message);
int usb_read(char *message, int message_size);
void usb_close();

#define USB_READING_OFF 0
#define USB_READING_ON 1

extern int is_usb_reading;
#endif

#endif	/* USB_H */

