#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include "decoupe.c"

#define NBMOTSMAX 20

int main(int argc, char *argv[]) {
  if (argc<2){
    fprintf(stderr,"Usage : %s pas assez d'argument\n",argv[0]);
    exit(1);
  }


  char *tab[NBMOTSMAX+1];
  int retour;

  for(int i=1;i<argc;i++){

    pid_t pidfork=fork();

    if (pidfork==-1){
      perror("fork");
      exit(1);
    }

    printf("[%d] J'ai délégué %s à %d. J'attends sa fin..\n",getpid(),tab[0],pidfork);
   // !! pere et fils ?

    if (pidfork==0){
      Decoupe(argv[i],tab);
      printf("[%d] Je lance %s :\n",getpid(),tab[0]);
      execvp(tab[0],tab);
      exit(0);
    }

    wait(&retour);
    printf("[%d] %d terminé\n",getpid(),pidfork);
  }
  printf("[%d] J'ai fini\n",getpid());
  return 0;
}
