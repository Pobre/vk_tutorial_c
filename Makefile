# Set default vars
CC = clang
CFLAGS = -std=c11 -O1 -g -Wall -DVK_USE_PLATFORM_WAYLAND_KHR #-fsanitize=address -fno-omit-frame-pointer
LDFLAGS = -lvulkan -lwayland-client

# steps
all:
	echo "make tut_#"
tut_1: vk_tutorial.o init_vulkan.o tut_1.o
	$(CC) $(CFLAGS) -o tut_1 vk_tutorial.o init_vulkan.o tut_1.o $(LDFLAGS)
tut_1.o: vk_tutorial.o init_vulkan.o
	$(CC) -c $(CFLAGS) -o tut_1.o tut_1.c
tut_2: vk_tutorial.o init_vulkan.o tut_2.o
	$(CC) $(CFLAGS) -o tut_2 vk_tutorial.o init_vulkan.o tut_2.o $(LDFLAGS)
tut_2.o: vk_tutorial.o init_vulkan.o
	$(CC) -c $(CFLAGS) tut_2.c
tut_3: vk_tutorial.o init_vulkan.o tut_3.o
	$(CC) $(CFLAGS) -o tut_3 vk_tutorial.o init_vulkan.o tut_3.o $(LDFLAGS)
tut_3.o: vk_tutorial.o init_vulkan.o
	$(CC) -c $(CFLAGS) tut_3.c
init_vulkan.o: vk_tutorial.o
	$(CC) -c $(CFLAGS) -o init_vulkan.o init_vulkan.c
window.o:
	$(CC) -c $(CFLAGS) -o window.o window.c
vk_tutorial.o:
	$(CC) -c $(CFLAGS) vk_tutorial.c
clean:
	rm -f *.o tut_1 tut_2
