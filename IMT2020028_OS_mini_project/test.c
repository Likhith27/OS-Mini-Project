#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <stdbool.h>
#include <fcntl.h>
#include <netinet/in.h> 
#include <string.h> 
#include <pthread.h>
#include "constants.h"

int main() {
    char* type = "admin";
    char* username = "chakri";
    char* password = "9943";
    char file[40];
    strcpy(file, username);
    strncat(file, ".txt", sizeof(".txt"));
    int fd = open(file, O_RDWR | O_CREAT , 0744);
    if(fd == -1) {
        perror("signup");
        return -1;
    }

    struct user u;
    strcpy(u.type,type);
    strcpy(u.username,username);
    strcpy(u.password,password);
    write(fd, &u, sizeof(struct user));
    close(fd);
    return 0;
}