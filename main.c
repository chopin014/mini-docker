#define _GNU_SOURCE 
#include <sched.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mount.h>
#include <stdio.h>
#include <stdlib.h>

int fd[2];

int child_fn (void *arg){
    
    // listen to the pipe, waiting from the parent signal
    char buf[1];
    close(fd[1]);
    read(fd[0], buf, 1);

    sethostname("container", 9);
    mount(NULL, "/", NULL, MS_REC |MS_PRIVATE, NULL);
    
    printf("Phase 1 complete, ran the child process\n");
    return 0;
}

void write_rule (const char *path, const char *value){
    FILE *file;

    file = fopen(path, "w");

    if (file == NULL){
        perror("Error");
        exit(1);
    }

    fprintf(file, value);

    fclose(file);

}


#define STACK_SIZE (1024 * 1024)


int main (){
    
    char path[100], buf[1];

    if (pipe(fd) == -1){
        perror("Error");
        exit(1);
     
    }

    printf ("Phase 0 complete, pipe is working\n");

    char *stack = malloc(STACK_SIZE);
    char *stack_top = stack + STACK_SIZE;


    pid_t child_pid = clone (child_fn, stack_top, CLONE_NEWUTS | CLONE_NEWPID | CLONE_NEWNS | CLONE_NEWUSER | SIGCHLD, NULL);

    if (child_pid == -1){
        perror("Error");
        exit(1);
    }

    close(fd[0]);

    // write rules to the filesystem
    sprintf(path, "/proc/%d/setgroups", child_pid);
    write_rule (path, "deny");

    sprintf(path, "/proc/%d/uid_map", child_pid);
    write_rule(path, "0 1000 1");

    sprintf(path, "/proc/%d/gid_map", child_pid);
    write_rule(path, "0 1000 1");

    // wakes up the child
    write(fd[1], buf, 1);

    // wait the child wake up
    waitpid(child_pid, NULL, 0);

    printf("Phase 2 complete, parent exiting\n");   

}