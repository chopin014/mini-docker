# Mini-Docker-C

A minimal container runtime written in C that demonstrates how Docker works under the hood using Linux namespaces.

## Features
* **Isolation:** Uses `CLONE_NEWPID`, `CLONE_NEWNS`, and `CLONE_NEWUTS` to isolate processes.
* **Filesystem:** Automatically downloads and mounts an Alpine Linux root filesystem.
* **Permissions:** Sets up UID/GID mappings for user namespaces.

## Prerequisites
* Linux Operating System (This relies on Linux-specific syscalls)
* GCC Compiler
* Make
* `wget` and `tar` (for downloading the rootfs)

## How to Run

1.  **Clone the repository:**
    ```bash
    git clone [https://github.com/chopin014/mini-docker.git](https://github.com/chopin014/mini-docker.git)
    cd mini-docker
    ```

2.  **Build and Run:**
    Use the Makefile to automatically compile the code and download the filesystem.
    ```bash
    make run
    ```
    *Note: This requires `sudo` privileges to create namespaces and mount filesystems.*

3.  **Clean up:**
    To remove the compiled binary and the downloaded rootfs:
    ```bash
    make clean
    ```

## How it Works
This project uses the `clone()` syscall to create a child process with its own namespaces. It then uses `pivot_root` to swap the OS root directory with a local Alpine Linux directory (`rootfs`), effectively creating a containerized environment.