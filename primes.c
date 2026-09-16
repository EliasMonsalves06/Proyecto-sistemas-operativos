#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"


#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Winfinite-recursion"

void recursive(int *fd){
    close(fd[1]);

    int numero_recibido, numero_siguiente;

    if (read(fd[0], &numero_recibido, sizeof(int)) <= 0) {
        close(fd[0]);
        exit(0);
    }

    printf("prime %d\n", numero_recibido);

    int fd_nuevo[2];
    (void)pipe(fd_nuevo);

    if(fork() != 0){
        close(fd_nuevo[0]);

        while(read(fd[0], &numero_siguiente, sizeof(int)) > 0){
            if(numero_siguiente % numero_recibido != 0){
                write(fd_nuevo[1], &numero_siguiente, sizeof(int));
            }
        }
        close(fd[0]);
        close(fd_nuevo[1]);
        wait(0);
        exit(0);
    }
    else{
        close(fd[0]);
        recursive(fd_nuevo);
        exit(0);
    }
    exit(0);
}

#pragma GCC diagnostic pop


int main(int argc, char *argv[]){

    int fd[2];
    (void)pipe(fd);

    if (fork() != 0){
        close(fd[0]);
        for (int i = 2; i <= 35; i++){
            write(fd[1], &i, sizeof(int));
        }
        close(fd[1]);
        wait(0);
        exit(0);
    }
    else{
        recursive(fd);
    }
    
    exit(0);
}