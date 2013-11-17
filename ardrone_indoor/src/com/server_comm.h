/* 
 * File:   server_thread.h
 * Author: julien
 *
 * Created on November 12, 2013, 2:06 AM
 */

#ifndef SERVER_THREAD_H
#define	SERVER_THREAD_H

#include "../global_config.h"
#include "commons_comm.h"
#include <config.h>


#ifdef SERVER_COMM_ON

int listen_drone(); /* listen to drone, return a type of errors */

#endif

#endif	/* SERVER_THREAD_H */

