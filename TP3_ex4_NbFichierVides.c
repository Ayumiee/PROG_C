#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#include <string.h>

int nbVide = 0;

int Parcourir(const char* Rep){
  DIR* Repertoire;
  if ((Repertoire=opendir(Rep))==NULL){
    perror(Rep);
    exit(1);
  }
  const struct dirent* lect;
  char* element = malloc(sizeof(char));
  struct stat Infos;

  while((lect=readdir(Repertoire))!=NULL){
    strcpy(element, Rep); /* copie le nom du répertoire dans element */
    strcat(element, "/"); /* ajoute un slash */
    strcat(element, lect->d_name);
    if ((strcmp((lect->d_name),".")!=0) && (strcmp((lect->d_name),"..")!=0) ){
      if (lstat((lect->d_name),&Infos)<0){
        perror(lect->d_name);
        exit(2);
      }
      if (S_ISDIR(Infos.st_mode)!=0){ /* c'est un répertoire? si oui, appel récursif*/
        Parcourir(element);
      } else {
          if ((S_ISREG(Infos.st_mode)!=0) && (Infos.st_size ==0)){
            nbVide++;
            printf("%s\n",(lect->d_name));
          }
      closedir(Repertoire);

      return nbVide;
      }
    }
  }
  return nbVide;
}



int main(int argc,const char *argv[]) {
  if (argc!=2){
    fprintf(stderr,"Usage : 1 seul argument attendu\n");
    exit(3);
  }
  Parcourir(argv[1]);
  printf("Il y a %d fichiers vide\n",nbVide);
  return 0;
}
