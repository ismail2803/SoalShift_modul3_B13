#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <pthread.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#define PORT 8000

int *stok;

void *print_stok(void *args){
    while(1){
        printf("Stok saat ini %d\n", *stok);
        sleep(5);
    }
}

int main(int argc, char const *argv[]) {
    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
      
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
      
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );
      
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 1) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0) {
        perror("accept");
        exit(EXIT_FAILURE);
    }
    //Shared Memory
    key_t key = 1234;
    
    int shmid = shmget(key, sizeof(int), IPC_CREAT | 0666);
    stok = shmat(shmid, NULL, 0);
    *stok = 0;

    //Buat thread
    int err;
    pthread_t thread1;
    err=pthread_create(&thread1, NULL, &print_stok, NULL);

    while(1){
        char buffer[1024] = {0};
        valread = read( new_socket , buffer, 1024);
        if(strcmp(buffer, "tambah")==0)
            *stok = *stok + 1;
    }

    shmdt(stok);
    shmctl(shmid, IPC_RMID, NULL);

    return 0;
}