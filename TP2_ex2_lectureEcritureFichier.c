#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>


int lire(off_t n, const char* fichier){
  /*renvoie l'entier présent à l'endroit n*/
  int Entier;
  int fd= open(fichier,O_RDONLY);
  if (fd==-1){
    perror("erreur ouverture\n");
  }
  if (lseek(fd, sizeof(int)*(n-1),SEEK_SET)<0){
    perror("n'existe pas dans le fichier\n");
    exit(1);
  };
  if (read(fd,&Entier,sizeof(Entier))!=sizeof(Entier)){
    fprintf(stderr,"lecture\n");
  };
  if (close(fd)==-1){
    perror("erreur fermeture\n");
  }
  return Entier;
}

int ecrire(off_t n, const char* new,const char* fichier){
  int fd= open(fichier,O_WRONLY);
  if (fd==-1){
    perror("erreur ouverture");
  }
  if (lseek(fd, sizeof(int)*(n-1),SEEK_SET)<0){
    perror("n'existe pas dans le fichier");
    exit(1);
  } else {
    write(fd,&new,sizeof(n));
  }
  if (close(fd)==-1){
    perror("erreur fermeture");
  }
  exit(0);
}

int main(int argc, const char *argv[]) {
  if (argc<2){
    fprintf(stderr,"Usage: Pas assez d'arguments\n");
    exit(1);
  }
  if (argc>4){
    fprintf(stderr,"Usage : Trop d'argument\n");
    exit(2);
  }
  if (argc==3){
    lire(atoi(argv[1]),argv[2]);
  } else {
    if (argc==4){
      ecrire(atoi(argv[1]),argv[2],argv[3]);
    }
  }

  return 0;
}
