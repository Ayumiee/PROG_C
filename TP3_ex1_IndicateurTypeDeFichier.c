#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>

void Affiche_inode (struct stat * Infos){
    /*info pointe vers une struct stat */
    char* type;
    int taille = Infos->st_size;
    char* date = ctime(&Infos->st_mtime);
    if (S_ISREG(Infos->st_mode)!=0){
        type="fichier ordinaire";
    }
    if (S_ISDIR(Infos->st_mode)!=0){
        type="répertoire";
    }
    if (S_ISFIFO(Infos->st_mode)!=0){
        type="fichier spécial tube(fifo)";
    }
    if (S_ISCHR(Infos->st_mode)!=0){
        type="périphérique caractère";
    }
    if (S_ISBLK(Infos->st_mode)!=0){
        type="périphérique bloc";
    }
    if (S_ISLNK(Infos->st_mode)!=0){
        type="lien symbolique";
    }
    if (S_ISSOCK(Infos->st_mode)!=0){
        type="socket(connexion réseau)";
    }
    printf(" %10s %10d octets, modifié le: %10s\n",type,taille,date);
}


int main(int argc, char const *argv[])
{
    if(argc!=2){
        fprintf(stderr,"Usage : 1 argument seul attendu ");
        exit(1);
    }
    struct stat Infos;
    if (lstat(argv[1],&Infos)<0){
        perror(argv[1]);
        exit(2);
    }
    printf("%-30s",argv[1]);
    Affiche_inode(&Infos);
    return 0;
}
