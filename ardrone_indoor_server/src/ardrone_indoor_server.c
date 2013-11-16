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

//Local project
#include "global_config.h"

#ifdef VIDEO_ON
   #include "video/video_stage.h"
#endif
   
#ifdef UDP_COMM_ON
   #include "com/udp_comm.h"

   DEFINE_THREAD_ROUTINE(server_comm, data)
   {
      // the server can listen (port 7000)
      if (is_udp_listening)
      {
         udp_listen(5);
      }
      // and the server can send at the meantime (port 7001)
      if (is_udp_sending)
      {
         //udp_send(DEST_IP, "x");
      }
   }
#endif
   
#ifdef DRONE_COMM_ON
   #include "com/drone_comm.h"

   DEFINE_THREAD_ROUTINE(drone_comm, data)
   {
      send_to_server(0, 0);
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
   #ifdef TEST_DEMO_COMM_LOCAL
      printf("demo program launched\n\n");
	
      char message[5];
      while (1) {
         button_init_callback();
         udp_listen_once(message, 5);
         if (message == "init"){
            printf("message init received\n");
            button_sync_callback();
            udp_listen_once(message, 5);
         }
         if (message == "sync"){
            printf("message sync 1 received\n");
            button_sync_callback();
            udp_listen_once(message, 5);
         }
         if (message == "sync"){
            printf("message sync 2 received\n");
            button_exit_callback();
            udp_listen_once(message, 5);
         }
         if (message == "exit"){
            printf("message exit received\n");
            return 0;
         }
      }
      return 0;
   #elif defined(TEST_GUI_STANDALONE)
      init_gui(&argc, &argv);
      gtk_main ();
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
      // if enabled server communication, initializes the callback func ptr
      #ifdef UDP_COMM_ON
         udp_listen_callback = &on_drone_message_received;
      #endif
      START_THREAD(gui, NULL); /* Starting the GUI thread */
   #endif 
   #ifdef UDP_COMM_ON
      START_THREAD(server_comm, NULL);
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
   #ifdef UDP_COMM_ON
      JOIN_THREAD(server_comm);
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
   //THREAD_TABLE_ENTRY(server_comm, 20)
END_THREAD_TABLE

