#ifndef _ARDRONE_INDOOR_SERVER_H_
#define _ARDRONE_INDOOR_SERVER_H_

#include <stdio.h>
#include <VP_Os/vp_os_types.h>

#include "global_config.h"

#ifdef TEST_COMM
	#include "tests/test_comm.h"
#elif defined TEST_WIFI_DELAY
	#include "tests/test_wifi_delay.h"
#elif defined TEST_GUI
	#include "tests/test_gui.h"
#elif defined TEST_GUI_ONLY
#elif defined TEST_USB
	#include "tests/test_usb.h"
#endif

C_RESULT signal_exit();

#endif // _MYKONOS_TESTING_TOOL_H_
