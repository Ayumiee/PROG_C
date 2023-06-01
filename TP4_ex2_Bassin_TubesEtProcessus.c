#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <errno.h>

#define MAX_BASSIN 5

void erreur(int code,char* msg){
  perror(msg);
  exit(code);
}

int main(int argc, char const *argv[]) {
  if (argc<2){
      fprintf(stderr,"il faut au moins un argument N (nombre d'étoiles)\n");
      exit(2);
    }
    int remplir[2], etat[2];
    if ((pipe(remplir)==-1) || pipe(etat)==-1){
      erreur(3,"pipe");
    }
    pid_t fils=fork();
    if (fils==-1){
      erreur(4,"fork");
    }
    if (fils==0){/* ------------------------------------- Processus fils*/

      if ((close(remplir[1])==-1) || (close(etat[0])==-1)){
        erreur(5,"close");
      }
      int element;
      int bassin=0;
      while (read(remplir[0],&element,sizeof(int))>0){
        if (element>0){
          bassin++;
          printf("*");
          fflush(stdout);

          if (bassin>=MAX_BASSIN){
            if (write(etat[1],&bassin,sizeof(bassin))==-1){
              erreur(6,"write");
            }
          }

        } else {
          bassin=0;
          printf("\n");
        }

      }
      if ((close(remplir[0])==-1)||(close(etat[1])==-1)){
        erreur(5,"close");
      }
      exit(1);
    }/*-------------------------------------------------- Processus pere*/
    if ((close(remplir[0])==-1)||(close(etat[1])==-1)){
      erreur(5,"close");
    }

    int f_flags; /* mode non bloquant pour le       printf("[PERE] ecris -1\n");tube Etat dans le P`ere */
    f_flags = fcntl(etat[0], F_GETFL); /* R´ecup´eration des flags */
    f_flags |= O_NONBLOCK; /* Positionnement du flag de non blocage */
    fcntl(etat[0], F_SETFL, f_flags); /* Mis `a jour des flags */

    int un=1;
    int element;
    int retourLect;
    int moinsUn=-1;
    for (int i=0;i<atoi(argv[1]);i++){

      if (write(remplir[1],&un,sizeof(un))==-1){
        erreur(6,"write");
      }
      sleep(1); /*père écrit toutes les secondes le chiffre 1*/
      if ((retourLect=read(etat[0],&element,sizeof(element))>0)){
        if (write(remplir[1],&moinsUn,sizeof(moinsUn))==-1){
          erreur(6,"write");
        }

      } else {
        if (errno!=EAGAIN){
          erreur(7,"lecture");
        }
      }
    }

    if ((close(remplir[1])==-1)||(close(etat[0])==-1)){
      erreur(5,"close");
    }

    int retour;
    wait(&retour);
    printf("[PERE] Mon fils s'est fini avec le code %d \n",WEXITSTATUS(retour));

  return 0;
}
