CC := arm-linux-gnueabihf-gcc
LINUX_DIR= /home/waveshare/cubieboard2/kernel/linux-sunxi-sunxi-3.4.61_A20
CFLAGS := -I$(LINUX_DIR)
TARGET := test_camera

.PHONY: all clean


all: $(TARGET)

test_camera:test_camera.c
	$(CC) $(CFLAGS) -static  $^  -o  $@

clean:
	rm -rf $(TARGET)
