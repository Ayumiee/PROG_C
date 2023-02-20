#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int main(int argc, char const *argv[])
{
    pid_t pidfork=fork();
    if (pidfork==-1){
        perror("erreur fork");
        exit(1);
    }
    if (pidfork==0) {
        execlp("ls","ls","-l",NULL);
        perror("ls");
        exit(2);
    }
    int retour;
    wait(&retour);
    execlp("date","date",NULL);
    perror("date");
    exit(3);
    return 0;
}
