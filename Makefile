# Variables
CC = gcc
CFLAGS = -Wall -Wextra
TARGET = mini_docker
ROOTFS_DIR = rootfs
ALPINE_URL = https://dl-cdn.alpinelinux.org/alpine/v3.23/releases/x86_64/alpine-minirootfs-3.23.2-x86_64.tar.gz

# The default rule (runs when you type 'make')
all: $(TARGET) $(ROOTFS_DIR)

# 1. Compile the C program
$(TARGET): main.c
	$(CC) $(CFLAGS) -o $(TARGET) main.c

# 2. Download and extract the RootFS (only if it doesn't exist)
$(ROOTFS_DIR):
	@echo "Creating rootfs directory..."
	mkdir -p $(ROOTFS_DIR)
	@echo "Downloading Alpine Linux..."
	wget $(ALPINE_URL) -O alpine.tar.gz
	@echo "Extracting filesystem..."
	tar -xvf alpine.tar.gz -C $(ROOTFS_DIR)
	rm alpine.tar.gz
	@echo "Done! Rootfs is ready."

# 3. Clean up the project
clean:
	rm -f $(TARGET)
	rm -rf $(ROOTFS_DIR)

# 4. Helper to run it with sudo (since you use namespaces/mounts)
run: all
	sudo ./$(TARGET)

.PHONY: all clean run