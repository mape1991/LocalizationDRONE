/**
 * @file main.c
 * @author sylvain.gaeremynck@parrot.com
 * @date 2009/07/01
 */
#include "ardrone_indoor.h"

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
#include "video/video_stage.h"
#include "gui.h"

static int32_t exit_ihm_program = 1;

DEFINE_THREAD_ROUTINE(gui, data) /* gui is the routine's name */
{
  gdk_threads_enter();
  gtk_main();
  gdk_threads_leave();
}

/* Implementing Custom methods for the main function of an ARDrone application */
int main(int argc, char** argv)
{
	return ardrone_tool_main(argc, argv);
}

/* The delegate object calls this method during initialization of an ARDrone application */
C_RESULT ardrone_tool_init_custom(void)
{
  /* Registering for a new device of game controller */
  // ardrone_tool_input_add( &gamepad );

  /* Start all threads of your application */
  START_THREAD( video_stage, NULL );
  
  // init with arguments
  init_gui(0, 0); /* Creating the GUI */
  START_THREAD(gui, NULL); /* Starting the GUI thread */
  
  return C_OK;
}

/* The delegate object calls this method when the event loop exit */
C_RESULT ardrone_tool_shutdown_custom(void)
{
  /* Relinquish all threads of your application */
  JOIN_THREAD( video_stage );

  /* Unregistering for the current device */
  // ardrone_tool_input_remove( &gamepad );
  
  /* user interface thread */
  JOIN_THREAD(gui);
  
  /* server communication */
  JOIN_THREAD(server_comm);

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
  THREAD_TABLE_ENTRY(navdata_update, 20)
  THREAD_TABLE_ENTRY(video_stage, 20)
  THREAD_TABLE_ENTRY(gui, 20)
  //THREAD_TABLE_ENTRY(server_comm, 20)
END_THREAD_TABLE

