include $(shell pwd)/user.properties
PC_TARGET=yes
USE_LINUX=yes

ifdef MYKONOS
   include $(ARDRONE_CUSTOM_CONFIG)
   include $(ARDRONE_BUILD_CONFIG)
else
   include $(SDK_PATH)/Soft/Build/custom.makefile
   include $(SDK_PATH)/Soft/Build/config.makefile
endif

ifeq "$(RELEASE_BUILD)" "yes"
   ARDRONE_TARGET_DIR=$(shell pwd)/Build/Release
else
   ARDRONE_TARGET_DIR=$(shell pwd)/Build/Debug
endif

TARGET=ardrone_indoor_server

SRC_DIR:=$(shell pwd)/src

# Define application source files
GENERIC_BINARIES_SOURCE_DIR:=$(SRC_DIR)

GENERIC_BINARIES_COMMON_SOURCE_FILES+=			\
   nav/navdata.c    \
   ../../ardrone_indoor_commons/com/udp_comm.c \
   ../../ardrone_indoor_commons/usb/usb.c \
   tests/test_comm.c \
   tests/test_wifi_delay.c \
   tests/test_gui.c \
   tests/test_usb.c \
   tests/test_full.c \
   gui/gui.c \
   gui/gtk_utils.c \
   gui/scene.c \
   video/video_stage.c 
   
# Including headers means also adding the lib folder to the generic_lib_paths
# i.e. for the gtkextra, we have added the path to the headers (defined in the custom user properties)
# of the gtkextra and we have added the static shared library libgtkextra compiled from the folder of gtkextra (containing a
# configure command and a makefile)
GENERIC_INCLUDES+=					\
	$(SRC_DIR) \
	$(LIB_DIR) \
	$(SDK_PATH)/Soft/Common \
	$(SDK_PATH)/Soft/Lib \
	$(GTK_EXTRA_PATH_INCLUDE)

GENERIC_TARGET_BINARIES_PREFIX=

GENERIC_TARGET_BINARIES_DIR=$(ARDRONE_TARGET_DIR)

GENERIC_BINARIES_SOURCE_ENTRYPOINTS+=			\
   ardrone_indoor_server.c

GENERIC_INCLUDES:=$(addprefix -I,$(GENERIC_INCLUDES))

GENERIC_LIB_PATHS=-L$(GENERIC_TARGET_BINARIES_DIR) -L$(GTK_EXTRA_PATH)
#warning: when specifying the name of the library, do not put the prefix lib in front of it
GENERIC_LIBS=-lpc_ardrone -lgtk-x11-2.0 -lrt -lgtkextra-x11-3.0

SDK_FLAGS+="USE_APP=yes"
SDK_FLAGS+="APP_ID=ardrone_indoor_server"

export GENERIC_CFLAGS
export GENERIC_LIBS
export GENERIC_LIB_PATHS
export GENERIC_INCLUDES
export GENERIC_BINARIES_SOURCE_DIR
export GENERIC_BINARIES_COMMON_SOURCE_FILES
export GENERIC_TARGET_BINARIES_PREFIX
export GENERIC_TARGET_BINARIES_DIR
export GENERIC_BINARIES_SOURCE_ENTRYPOINTS

# Bug fix ...
export GENERIC_LIBRARY_SOURCE_DIR=$(GENERIC_BINARIES_SOURCE_DIR)


.PHONY: $(TARGET) build_libs

all: build_libs $(TARGET)

$(TARGET):
	@$(MAKE) -C $(SDK_PATH)/VP_SDK/Build $(TMP_SDK_FLAGS) $(SDK_FLAGS) $(MAKECMDGOALS) USE_LINUX=yes
	mv $(ARDRONE_TARGET_DIR)/ardrone_indoor_server $(TARGET)
	mv $(TARGET) $(ARDRONE_TARGET_DIR)/

$(MAKECMDGOALS): build_libs
	@$(MAKE) -C $(SDK_PATH)/VP_SDK/Build $(TMP_SDK_FLAGS) $(SDK_FLAGS) $(MAKECMDGOALS) USE_LINUX=yes

build_libs:
	@$(MAKE) -C $(SDK_PATH)/Soft/Build $(TMP_SDK_FLAGS) $(SDK_FLAGS) $(MAKECMDGOALS) USE_LINUX=yes

