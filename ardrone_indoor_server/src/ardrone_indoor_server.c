/**
 * @file main.c
 * @author sylvain.gaeremynck@parrot.com
 * @date 2009/07/01
 * 
 * Modifications:
 *    - use of a global config with definitions to enable/disable functionalities of the program (gui,...)
 */
#include "ardrone_indoor_server.h"

//ARDroneLib
#include <utils/ardrone_time.h>
#include <ardrone_tool/Navdata/ardrone_navdata_client.h>
#include <ardrone_tool/Control/ardrone_control.h>
#include <ardrone_tool/UI/ardrone_input.h>

//Common
#include <config.h>
#include <ardrone_api.h>

//VP_SDK
#include <ATcodec/ATcodec_api.h>
#include <VP_Os/vp_os_print.h>
#include <VP_Api/vp_api_thread_helper.h>
#include <VP_Os/vp_os_signal.h>


#ifdef VIDEO_ON
   #include "video/video_stage.h"
#endif

#ifdef USB_ON
   #include "usb/usb.h"
#endif
 
#ifdef UDP_ON
   #include "../../ardrone_indoor_commons/com/udp_comm.h"

   int is_usb_reading = USB_READING_OFF;

   DEFINE_THREAD_ROUTINE(udp_listen_comm, data)
   {
      // the server can listen (port 7000)
      if (is_udp_listening)
      {
         #ifdef TEST_COMM
            test_comm_thread_udp_read();
         #elif defined TEST_WIFI_DELAY
            test_wifi_delay_udp_read();
         #elif defined TEST_GUI
            test_gui_thread_udp_read();
         #endif
      }
   }
   
   DEFINE_THREAD_ROUTINE(udp_send_comm, data)
   {   
      // and the server can send at the meantime (port 7001)
      if (is_udp_sending)
      {
         #ifdef TEST_COMM
            test_comm_thread_send();
         #elif defined TEST_WIFI_DELAY
            test_wifi_delay_udp_send();
		 #elif defined TEST_GUI
            test_gui_thread_send();
         #endif
      }
   }
#endif
   
#ifdef USB_ON
   DEFINE_THREAD_ROUTINE(usb_listen_comm, data)
   {
	   if (is_usb_reading)
	   {
      	  #ifdef TEST_COMM
         	 test_comm_thread_usb_read();
		  #elif defined TEST_GUI
             test_gui_thread_usb_read();
          #endif
	   }
   }
#endif
   
#ifdef GUI_ON
   #include "gui/gui.h"

   DEFINE_THREAD_ROUTINE(gui, data) /* gui is the routine's name */
   {
     gdk_threads_enter();
     gtk_main();
     gdk_threads_leave();
   }
#endif
   
// global variable linking the entry point to the user
static int32_t exit_ihm_program = 1;

/* Implementing Custom methods for the main function of an ARDrone application */
int main(int argc, char** argv)
{
   // execute the main of the selected test
   #ifdef TEST_COMM
      test_comm_main();
      return ardrone_tool_main(argc, argv);
   #elif defined(TEST_WIFI_DELAY)
      test_wifi_delay_main();
      return ardrone_tool_main(argc, argv);
   #elif defined(TEST_GUI)
      init_gui(&argc, &argv);
      gtk_main ();
      // test_gui_main();
      // FIXME: test gui only and then with the ardrone for the real communication tests
      return(0);
   #else
      return ardrone_tool_main(argc, argv);
   #endif
}

/* The delegate object calls this method during initialization of an ARDrone application */
C_RESULT ardrone_tool_init_custom(void)
{
   /* Registering for a new device of game controller */
   // ardrone_tool_input_add( &gamepad );

   /* Start all threads of your application */
   #ifdef VIDEO_ON
      START_THREAD( video_stage, NULL );
   #endif
   // init with arguments
   #ifdef GUI_ON
      init_gui(0, 0); /* Creating the GUI */
      START_THREAD(gui, NULL); /* Starting the GUI thread */
   #endif 

   #ifdef UDP_ON
      START_THREAD(udp_listen_comm, NULL);
      START_THREAD(udp_send_comm, NULL);
   #endif

   #ifdef USB_ON
      START_THREAD(usb_listen_comm, NULL);
   #endif

   return C_OK;
}

/* The delegate object calls this method when the event loop exit */
C_RESULT ardrone_tool_shutdown_custom(void)
{
   /* Relinquish all threads of your application */
   #ifdef VIDEO_ON
      JOIN_THREAD( video_stage );
   #endif

   /* Unregistering for the current device */
   // ardrone_tool_input_remove( &gamepad );
  
   /* user interface thread */
   #ifdef GUI_ON
     // JOIN_THREAD(gui);
   #endif
  
   /* server communication */
   #ifdef UDP_ON
      JOIN_THREAD(udp_listen_comm);
      JOIN_THREAD(udp_send_comm);
   #endif 

   #ifdef USB_ON
      JOIN_THREAD(usb_listen_comm);
   #endif
      
   return C_OK;
}

/* The event loop calls this method for the exit condition */
bool_t ardrone_tool_exit()
{
  return exit_ihm_program == 0;
}

C_RESULT signal_exit()
{
  exit_ihm_program = 0;

  return C_OK;
}

/* Implementing thread table in which you add routines of your application and those provided by the SDK */
BEGIN_THREAD_TABLE
   
   THREAD_TABLE_ENTRY(ardrone_control, 20)
   #ifdef NAV_ON
      THREAD_TABLE_ENTRY(navdata_update, 20)
   #endif
   #ifdef VIDEO_ON
      THREAD_TABLE_ENTRY(video_stage, 20)
   #endif
   #ifdef GUI_ON
      THREAD_TABLE_ENTRY(gui, 20)
   #endif

   #ifdef UDP_ON
      THREAD_TABLE_ENTRY(udp_listen_comm, 20)
      THREAD_TABLE_ENTRY(udp_send_comm, 20)
   #endif

   #ifdef USB_ON
      THREAD_TABLE_ENTRY(usb_listen_comm, 20)
   #endif

END_THREAD_TABLE

