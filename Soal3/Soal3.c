#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>

int fitur, status;
int wakeup_status=0;
int spirit_status=100;
int Agmal3=0;
int Iraj3=0;

void *Agmal(void *args){
    while(1)
    {
        if(fitur == 2)
        {
            wakeup_status = wakeup_status + 15;
            Agmal3 = Agmal3 + 1;
            fitur = 0;
            status = 1;
        }
        if(Iraj3 == 3)
        {
            Iraj3 = 0;
            printf("\nAgmal Ayo Bangun disabled 10 s\n");
            status = 1;
            sleep(10);
        }
    }
}

void *Iraj(void *args){
    while(1)
    {
        if(fitur == 3)
        {
            spirit_status = spirit_status - 20;
            // printf("\nspirit_status Iraj %d\n", spirit_status);
            Iraj3 = Iraj3 + 1;
            // Agmal3 = 0;
            fitur = 0;
            status = 1;
        }
        if(Agmal3 == 3)
        {
            Agmal3 = 0;
            printf("\nFitur Iraj Ayo Tidur disabled 10 s\n");
            status = 1;
            sleep(10);
        }
    }
}

int main(){
    pthread_t thread1, thread2;
    int  iret1, iret2;

    iret1 = pthread_create( &thread1, NULL, &Agmal, NULL);
    iret2 = pthread_create( &thread2, NULL, &Iraj, NULL);

    while(1){
        status=0;

        if(wakeup_status >= 100)
        {
            printf("\nAgmal Terbangun,mereka bangun pagi dan berolahraga\n");
            break;
        }
        if(spirit_status <= 0)
        {
            printf("\nIraj ikut tidur, dan bangun kesiangan bersama Agmal\n");
            break;
        }

        printf("\nFitur :\n");
        printf("1. All Status\n");
        printf("2. Agmal Ayo bangun\n");
        printf("3. Iraj Ayo Tidur\n");
        scanf("%d", &fitur);

        if(fitur == 1)
        {
            printf("\nAgmal WakeUp_Status = %d\n", wakeup_status);
            printf("Iraj Spirit_Status = %d\n", spirit_status);
            status = 1;
        }
        while(status == 0)
        {

        }
    }

    return 0;
}