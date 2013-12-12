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

#ifdef UDP_ON
   int is_udp_listening = 0;
   int is_udp_sending   = 0;

   DEFINE_THREAD_ROUTINE(udp_listen_comm, data)
   {
	  #ifdef TEST_COMM
		 test_comm_thread_udp_read();
	  #elif defined TEST_WIFI_DELAY
		 test_wifi_delay_udp_read();
	  #elif defined TEST_GUI
		 test_gui_thread_udp_read(COMM_MESSAGE_SIZE);
	  #elif defined TEST_FULL
		 test_gui_thread_udp_read(COMM_MESSAGE_DTS_SIZE);
	  #endif
   }
   
   DEFINE_THREAD_ROUTINE(udp_send_comm, data)
   {   
	  #ifdef TEST_COMM
		 test_comm_thread_send();
	  #elif defined TEST_WIFI_DELAY
	 	 test_wifi_delay_udp_send();
	 	 	 // no changes for udp sending behavior between test_gui and test_full
	  #elif defined(TEST_GUI) || defined (TEST_FULL)
	     test_gui_thread_send();
	  #endif
   }
#endif
   
#ifdef USB_ON
   int is_usb_reading = 0;

   DEFINE_THREAD_ROUTINE(usb_listen_comm, data)
   {
	  #ifdef TEST_COMM
		 test_comm_thread_usb_read();
	  #elif defined(TEST_GUI) || defined(TEST_FULL)
		 test_gui_thread_usb_read();
	  #endif
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

// TODO: call tests with respect to the test, udp and usb parameters
// (without using defines but a switch (cases) from the user inputs)
/* Implementing Custom methods for the main function of an ARDrone application */
int main(int argc, char** argv)
{
	// template for changing the tests according to input user arguments
	/*
	if (argc > 1){
		if (strcmp(argv[1], "test_comm") == -1){
		}else if (strcmp(argv[1], "test_wifi_delay") == -1){
			#undef TEST_COMM
		}
	}*/

   // test of communication protocols (udp and usb) (equal to the demo of sprint 1)
   #ifdef TEST_COMM
      test_comm_main();
      return ardrone_tool_main(argc, argv);
   // test of wifi delay (roundtrip tranmission time of one message)
   #elif defined(TEST_WIFI_DELAY)
      test_wifi_delay_main();
      return ardrone_tool_main(argc, argv);
   // test of the gui including communication protocols
   // the user can click on buttons and obtain direct message transmissions
   #elif defined(TEST_GUI)
      test_gui_main(argc, argv);
      return ardrone_tool_main(argc, argv);
	//
   #elif defined (TEST_FULL)
      test_full_main();
      return ardrone_tool_main(argc, argv);
   // test of the gui only (to check the resulting interface display)
   #elif defined(TEST_GUI_ONLY)
      init_gui(argc, argv);
      gtk_main ();
      while(1);
      return 0;
	#elif defined(TEST_USB)
      test_usb_main();
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
     // init_gui(0, 0); /* Creating the GUI */
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
      JOIN_THREAD(gui);
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

