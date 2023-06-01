#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

void my_cat_destination(int nbArguments, char *argv[])
{
  printf("%s\n", argv[nbArguments-1]);
  int destination = open(argv[nbArguments-1], O_WRONLY);
  if(destination < 0) {
    perror(argv[nbArguments-1]);
    exit(1);
  }

  dup2(destination, STDOUT_FILENO);

  for (int i = 1; i < nbArguments-2; i++) {
    pid_t pid = fork();
    if (pid == -1) {
      perror("fork");
      exit(2);
    }
    if (pid == 0) {
      execlp("cat", "cat", argv[i], NULL);
      perror("execlp");
      exit(3);
    }
  }
  close(destination);
  exit(0);
}

void my_cat(int nbArguments, char *argv[]) {
  for (int i = 1; i < nbArguments; i++) {
    pid_t pid = fork();
    if(pid == -1) {
      perror("fork");
      exit(2);
    }
    if(pid == 0) {
      execlp("cat", "cat", argv[i], NULL);
      perror("execlp");
      exit(3);
    }
  }
  exit(0);
}

int main(int argc, char *argv[]) {
  if(argc < 2) {
    fprintf(stderr, "Usage : Erreur parametre");
    exit(1);
  }

  if(strcmp(argv[argc-2], ">") == 0) {
    my_cat_destination(argc, argv);
  } else {
    my_cat(argc, argv);
  }
  exit(0);
}
