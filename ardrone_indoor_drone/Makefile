CC=/opt/CodeSourcery/Sourcery_CodeBench_Lite_for_ARM_GNU_Linux/bin/arm-none-linux-gnueabi-gcc
SRC=ardrone_indoor_drone.c com/udp.c
CFLAGS=-Wall -W -march=armv7-a
LFLAGS=-lm
OBJ=(SRC *.c:*.o)
TARGET=drone

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $^ $(CFLAGS) -o $@ $(LFLAGS) 

%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@

#$(MAKECMDGOALS): build_libs
#	/opt/CodeSourcery/Sourcery_CodeBench_Lite_for_ARM_GNU_Linux/bin/arm-none-linux-gnueabi-gcc -march=armv7-a -C $(SDK_PATH)/VP_SDK/Build $(TMP_SDK_FLAGS) $(SDK_FLAGS) $(MAKECMDGOALS) USE_LINUX=yes

#build_libs:
#	/opt/CodeSourcery/Sourcery_CodeBench_Lite_for_ARM_GNU_Linux/bin/arm-none-linux-gnueabi-gcc -march=armv7-a -C $(SDK_PATH)/Soft/Build $(TMP_SDK_FLAGS) $(SDK_FLAGS) $(MAKECMDGOALS) USE_LINUX=yes

