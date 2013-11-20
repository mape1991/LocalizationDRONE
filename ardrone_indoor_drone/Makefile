## -*- Makefile -*-
##
## User: julien
## Time: Nov 16, 2013 7:54:20 AM
## Makefile created by Oracle Solaris Studio.
##
## This file is generated automatically.
##


#### Compiler and tool definitions shared by all build targets #####
CC=/opt/CodeSourcery/Sourcery_CodeBench_Lite_for_ARM_GNU_Linux/bin/arm-none-linux-gnueabi-gcc
BASICOPTS = -g
CFLAGS = $(BASICOPTS) -march=armv7-a


# Define the target directories.
TARGETDIR_ardrone_indoor_drone=ARM

all: $(TARGETDIR_ardrone_indoor_drone)/ardrone_indoor_drone

## Target: ardrone_indoor_drone
CPPFLAGS_ardrone_indoor_drone = \
	-Isrc
OBJS_ardrone_indoor_drone =  \
	$(TARGETDIR_ardrone_indoor_drone)/ardrone_indoor_drone.o \
	$(TARGETDIR_ardrone_indoor_drone)/udp_comm.o \
	$(TARGETDIR_ardrone_indoor_drone)/test_comm.o
SYSLIBS_ardrone_indoor_drone = -lm 
USERLIBS_ardrone_indoor_drone = $(SYSLIBS_ardrone_indoor_drone) 
DEPLIBS_ardrone_indoor_drone =  
LDLIBS_ardrone_indoor_drone = $(USERLIBS_ardrone_indoor_drone)


# Link or archive
$(TARGETDIR_ardrone_indoor_drone)/ardrone_indoor_drone: $(TARGETDIR_ardrone_indoor_drone) $(OBJS_ardrone_indoor_drone) $(DEPLIBS_ardrone_indoor_drone)
	$(LINK.c) $(CFLAGS_ardrone_indoor_drone) $(CPPFLAGS_ardrone_indoor_drone) -o $@ $(OBJS_ardrone_indoor_drone) $(LDLIBS_ardrone_indoor_drone)


# Compile source files into .o files
$(TARGETDIR_ardrone_indoor_drone)/ardrone_indoor_drone.o: $(TARGETDIR_ardrone_indoor_drone) src/ardrone_indoor_drone.c
	$(COMPILE.c) $(CFLAGS_ardrone_indoor_drone) $(CPPFLAGS_ardrone_indoor_drone) -o $@ src/ardrone_indoor_drone.c

$(TARGETDIR_ardrone_indoor_drone)/udp_comm.o: $(TARGETDIR_ardrone_indoor_drone) src/com/udp_comm.c
	$(COMPILE.c) $(CFLAGS_ardrone_indoor_drone) $(CPPFLAGS_ardrone_indoor_drone) -o $@ src/com/udp_comm.c

$(TARGETDIR_ardrone_indoor_drone)/test_comm.o: $(TARGETDIR_ardrone_indoor_drone) src/tests/test_comm.c
	$(COMPILE.c) $(CFLAGS_ardrone_indoor_drone) $(CPPFLAGS_ardrone_indoor_drone) -o $@ src/tests/test_comm.c


#### Clean target deletes all generated files ####
clean:
	rm -f \
		$(TARGETDIR_ardrone_indoor_drone)/ardrone_indoor_drone \
		$(TARGETDIR_ardrone_indoor_drone)/ardrone_indoor_drone.o \
		$(TARGETDIR_ardrone_indoor_drone)/udp_comm.o \
		$(TARGETDIR_ardrone_indoor_drone)/test_comm.o
	rm -f -r $(TARGETDIR_ardrone_indoor_drone)


# Create the target directory (if needed)
$(TARGETDIR_ardrone_indoor_drone):
	mkdir -p $(TARGETDIR_ardrone_indoor_drone)


# Enable dependency checking
.KEEP_STATE:
.KEEP_STATE_FILE:.make.state.ARM


