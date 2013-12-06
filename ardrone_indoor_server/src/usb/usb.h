/* 
 * File:   usb.h
 * Author: julien
 *
 * Created on November 19, 2013, 10:54 AM
 */

#ifndef USB_H
#define	USB_H

#include "../global_config.h"

#define USB_PORT_NAME "/dev/ttyUSB0"
#define USB_BUFFER_MAX_SIZE 100

/**
 * parity: 0 for 8n1
 *l
 */
int usb_init(char *portname, int speed, int parity, int should_block);
void usb_write(char *message, int message_size);
void usb_write_char(char message);
int usb_read(char *message, int message_size);
void usb_close();

#endif	/* USB_H */

