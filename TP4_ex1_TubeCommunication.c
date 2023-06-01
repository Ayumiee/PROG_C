#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>


int main(int argc, char const *argv[])
{
    if (argc!=2){
        fprintf(stderr,"il faut un seul argument\n");
        exit(7);
    }
    int tube[2]; /*creation tube*/
    if (pipe(tube)==-1){
        perror("pipe");
        exit(2);
    }
    int fils=fork(); /*creation fils*/
    if (fils==-1){
        close(tube[0]);
        close(tube[1]);
        perror("fork");
        exit(3);
    }
    if (fils==0){ /* ------------------------------------- Processus fils*/
        int entierRecup;
        if (close(tube[1])==-1){
            perror("close(tube[1])");
            exit(4);
        }
        while (read(tube[0],&entierRecup,sizeof(entierRecup))>0){
            printf("[FILS] %d\n",entierRecup);    
        }
        if (close(tube[0])==-1){
            perror("close(tube[0])");
            exit(5);
        }
        exit(1);
    }
    if (close(tube[0]==-1)){ /*-------------------------- Processus pere*/
        perror("close(tube[0])");
        exit(5);
    } 
    for (int i=1;i<=atoi(argv[1]);i++){
        if (write(tube[1],&i,sizeof(i))==-1){
            perror("write");
            exit(6);
        }
        sleep(1);
    }
    if (close(tube[1])==-1){
        perror("close(tube[1])");
        exit(4);
    }
    int retour;
    wait(&retour);
    printf("[PERE] Mon fils s'est terminé avec le code de retour %d\n",WEXITSTATUS(retour));
}
