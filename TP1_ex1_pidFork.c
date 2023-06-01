#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

void AfficherInfo(void){
    pid_t pid = getpid();
    pid_t pidpere = getppid();
    pid_t pidgroupe= getpgrp();
    char* login = getlogin();
    uid_t uid = getuid();
    gid_t gid= getgid();
    printf("PID : %d\n",pid);
    printf("PID père : %d\n",pidpere);
    printf("PID groupe: %d\n",pidgroupe);
    printf("LOGIN : %s\n",login);
    printf("UID : %d\n",uid);
    printf("GID : %d\n",gid);
}


int main(int argc, char const *argv[])
{
    int retour;
    pid_t pid=fork();
    if (pid==-1){
        perror("erreur fork");
        exit(1);
    }
    if (pid==0){
        AfficherInfo();
        printf("[FILS] j'ai fini, Mon code de retour est 0\n");
        exit(0);
    }
    while ((pid=wait(&retour))!=-1) {
        printf("[PERE] mon fils est terminé, son code de retour : %d\n",WEXITSTATUS(retour));
        printf("[PERE] J'ai fini, Mon code de retour est 0\n");
    }
    exit(0);
}
