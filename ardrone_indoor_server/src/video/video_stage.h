#ifndef _IHM_STAGES_O_GTK_H
#define _IHM_STAGES_O_GTK_H

#include "../global_config.h"

#ifdef VIDEO_ON
#include <config.h>
#include <VP_Api/vp_api_thread_helper.h>


PROTO_THREAD_ROUTINE(video_stage, data);
#endif

#endif // _IHM_STAGES_O_GTK_H
