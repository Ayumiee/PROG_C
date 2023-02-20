#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>

void ecrire_dans_stdout ( char nom_fich[] ){
    int exit_status;

    char bloc;

    int ouverture = open(nom_fich,O_RDONLY);
    if (ouverture==-1){
        perror("erreur open nom_fich");
        close(ouverture);
        exit(1);
    }
    int nbLu=0;
    int nbEcrits=0;
    while (nbLu=read(ouverture,&bloc,sizeof(bloc))){
        if (nbEcrits=write(STDOUT_FILENO,&bloc,sizeof(bloc))==-1){
            perror("erreur écriture");
            exit(3);
        };
    }
    exit_status=0;

    if(nbLu==-1){
        perror("erreur lecture");
        exit(2);
    }

    if (close(ouverture)==-1){
        perror("erreur fermeture");
        exit(4);
    }
    exit(exit_status);
}


int main(int argc, char *argv[]){
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <nom_fichier>\n", argv[0]);
        return 1;
    }
    ecrire_dans_stdout(argv[1]);
    return 0;
}
