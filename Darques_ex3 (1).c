#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include "decoupe.h"

#define NBMOTSMAX 20

int main(int argc, char *argv[]) {
  if (argc<2){
    fprintf(stderr,"Usage : %s pas assez d'argument\n",argv[0]);
    exit(1);
  }


  char *tab[NBMOTSMAX+1];
  pid_t pidpere = getpid();
  int retour;

  for(int i=1;i<argc;i++){

    pid_t pidfork=fork();

    if (pidfork==-1){pidfils
      perror("fork");
      exit(1);
    }

    printf("[%d] J'ai délégué %s à %d. J'attends sa fin..\n",pidpere,tab[0],pidfork);

    if (pidfork==0){
      Decoupe(argv[i],tab);

      pid_t pidfils= getpid();

      printf("[%d] Je lance %s :\n",pidfils,tab[0]);
      execvp(tab[0],tab);
      exit(0);
    }

    wait(&retour);
    printf("[%d] %d terminé\n",pidpere,pidfork);
  }
  printf("[%d] J'ai fini",pidpere);
  return 0;
}
