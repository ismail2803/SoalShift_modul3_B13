#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>

int action = 0;

int kbhit(void)
{
    struct termios oldt, newt;
    int ch;
    int oldf;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

    ch = getchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);

    if(ch != EOF)
    {
        ungetc(ch, stdin);
        return 1;
    }

    return 0;
}

void *input(void *args)
{
    while(1){
        while(!kbhit());
        action = getchar() - '0';
    }
}

int main(){
    //buat thread
    pthread_t thread1;
    int iret1;
    iret1 = pthread_create( &thread1, NULL, &input, NULL);
    
    //Shared Memory
    int *stok;
    key_t key = 1234;
    
    int shmid = shmget(key, sizeof(int), IPC_CREAT | 0666);
    stok = shmat(shmid, NULL, 0);
    *stok = 10;

    while(1){
        printf("Shop\n");
        printf("Food stock : [ %d ]\n", *stok);
        printf("Choices\n");
        printf("1. Restock\n");
        printf("2. Exit\n");

        if(action == 1){
            *stok = *stok + 1;
            action = 0;
        }
        else if(action == 2)
        {
            break;
        }
        sleep(1);
        system("clear");
    }

    shmdt(stok);
    shmctl(shmid, IPC_RMID, NULL);

    return 0;
}