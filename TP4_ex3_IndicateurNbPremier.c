#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <errno.h>
#include "est_premier.c"

#define N 4

void erreur(int code,char* msg){
  perror(msg);
  exit(code);
}

int main(int argc, char const *argv[]) {
  if (argc<2){
      fprintf(stderr,"il faut au moins un argument\n");
      exit(2);
    }
  int tube[2]; /*creation tube*/
  if (pipe(tube)==-1){
    erreur(3,"tube");
  }
  for (int i=0;i<N;i++){

    pid_t fils=fork(); /*-----------------------creation fils*/
    if (fils==-1){
        close(tube[0]);
        close(tube[1]);
        erreur(4,"fork");
    }

    if (fils==0){

      if (close(tube[1])==-1){
        erreur(5,"fermeture tube 1");
      }
      long long unsigned int elementLu;
      int nbPremier=0;
      while(read(tube[0],&elementLu,sizeof(elementLu))>0){
        if (est_premier(elementLu)){
          printf("[FILS %d] %10llu est premier\n",i,elementLu);
          nbPremier++;
        } else {
          printf("[FILS %d] %10llu\n",i,elementLu);
        }
      }
      exit(nbPremier);
    } /*---------------------------------fin processus fils*/
  }/*fin première boucle for*/
  if (close(tube[0])==-1){
    erreur(5,"fermeture tube 2");
  }
  for (int j=1;j<argc;j++){ /* ---------------------------------------------- PERE */
    char *endptr;
    long long unsigned int chiffreConvertis=strtoull(argv[j],&endptr,10);
    if (argv[j]==endptr){
      fprintf(stderr,"Aucune conversion effectuée");
      exit(7);
    }
    if (write(tube[1],&chiffreConvertis,sizeof(chiffreConvertis))==-1){
      erreur(6,"write");
    }
  }/*fin deuxieme boucle for*/

  if (close(tube[1])==-1){
    erreur(5,"close");
  }
  
  int retour;
  int nbPremierTotaux=0;
  while((wait(&retour))!=-1){
    nbPremierTotaux+=+WEXITSTATUS(retour);
  }
  printf("[PERE] Il y a %d nombres premiers\n", nbPremierTotaux);
  return 0;

}
