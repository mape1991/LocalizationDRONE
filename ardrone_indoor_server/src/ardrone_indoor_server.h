#ifndef _ARDRONE_INDOOR_SERVER_H_
#define _ARDRONE_INDOOR_SERVER_H_

#include <stdio.h>
#include <VP_Os/vp_os_types.h>

#include "global_config.h"

C_RESULT signal_exit();

#ifdef TEST_COMM
extern char message_send_enable;
extern char message_sent_id;
#endif

#endif // _MYKONOS_TESTING_TOOL_H_
