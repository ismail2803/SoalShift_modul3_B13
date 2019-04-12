#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <termios.h>
#include <fcntl.h>

int hunger_status = 200;
int hygiene_status = 100;
int healt_status = 300;
int healt_NPC = 100;
int fitur = 0;
int *stok;
int mystok = 0;
int act_standby = 0;
int mandi;
int act_food = 0;
int act_battle = 0;
int mati = 0;

void *hgr_status(void *args){
    while(1){
        if(fitur != 2)
        {
            sleep(10);
            hunger_status = hunger_status - 5;

            if(hunger_status <= 0)
            {
                printf("Hunger Anda dibawah 0\n");
                mati = 1;
            }
        }
    }
}

void *hgn_status(void *args){
    while(1){
        if(fitur != 2)
        {
            sleep(30);
            hygiene_status = hygiene_status - 10;

            if(hygiene_status <= 0)
            {
                printf("Hygiene Anda dibawah 0\n");
                mati = 1;
            }
        }
    }
}


void *hlt_status(void *args){
    while(1){
        if(fitur != 2)
        {
            sleep(10);
            healt_status = healt_status + 5;
        }
    }
}

void *bath(void *args){
    while(1){
        if(act_standby == 2 && mandi == 1)
        {
            hygiene_status = hygiene_status + 30;
            mandi = 0;
            act_standby = 0;
            sleep(30);
        }
        mandi = 1;
    }
}

void *battle(void *args){
    while(1){
        if(fitur == 2){
            printf("Battle Mode\n");
            printf("Monter's Health : [ %d ]\n", healt_status);
            printf("Enemy's Health : [ %d ]\n", healt_NPC);
            printf("Choices\n");
            printf("1. Attack\n");
            printf("2. Run\n");

            if(act_battle == 1)
            {
                healt_status = healt_status - 20;
                healt_NPC = healt_NPC - 20;
                act_battle = 0;
            }
            else if(act_battle == 2)
            {
                fitur = 1;
            }
            if(healt_status <= 0)
            {
                printf("You Kalah\n");
                mati = 1;
            }
            if(healt_NPC <= 0){
                printf("You menang\n");
                healt_NPC = 100;
                fitur = 1;
            }
            sleep(1);
            system("clear");
        }
    }
}

void *standby(void *args){
    while(1){
        if(fitur ==  1)
        {
            printf("Standby Mode\n");
            printf("Health : [ %d ]\n", healt_status);
            printf("Hunger : [ %d ]\n", hunger_status);
            printf("Hygiene : [ %d ]\n", hygiene_status);
            printf("Food left : [ %d ]\n", mystok);
            if(mandi == 1)
                printf("Bath is ready\n");
            else if(mandi == 0)
                printf("Bath is not ready\n");
            printf("Choices\n");
            printf("1. Eat\n");
            printf("2. Bath\n");
            printf("3. Battle\n");
            printf("4. Shop\n");
            printf("5. Exit\n");

            if(mystok > 0 && act_standby == 1)
            {
                hunger_status = hunger_status + 15;
                act_standby = 0;
                mystok = mystok - 1;
                if(hunger_status > 200){
                    hunger_status = 200;
                }
            }
            if(act_standby == 3)
            {
                fitur = 2;
            }
            if(act_standby == 4)
            {
                fitur = 3;
            }
            if(act_standby == 5)
            {
                mati = 1;
            }
            sleep(1);
            system("clear");
        }
    }
}

void *shop(void *args){
    while(1){
        if(fitur == 3)
        {
            printf("Shop Mode\n");
            printf("Shop food stock : [ %d ]\n", *stok);
            printf("Your food stock : [ %d ]\n", mystok);
            printf("Choices\n");
            printf("1. Buy\n");
            printf("2. Back\n");

            if(act_food == 1 && *stok > 0)
            {
                mystok = mystok + 1;
                *stok = *stok -1;
                act_food = 0;
            }
            if(act_food == 2)
            {
                fitur = 1;
            }
            sleep(1);
            system("clear");
        }
    }
}

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

void *input(void *args){
    while(1){
        if(fitur == 1){
            while(!kbhit());
            act_standby = getchar() - '0';
        }
        else if(fitur == 2){
            while(!kbhit());
            act_battle = getchar() - '0';
        }
        else if(fitur == 3){
            while(!kbhit());
            act_food = getchar() - '0';
        }
    }
}


int main(){
    char nama[20];
    pthread_t thread1, thread2, thread3, thread4, thread5, thread6, thread7, thread8;
    int iret1, iret2, iret3, iret4, iret5, iret6, iret7, iret8;

    //Shared Memory
    key_t key = 1234;

    int shmid = shmget(key, sizeof(int), IPC_CREAT | 0666);
    stok = shmat(shmid, NULL, 0);

    printf("Berikan nama pada monstermu : \n");
    scanf("%s", nama);

    iret1 = pthread_create( &thread1, NULL, &hgr_status, NULL);
    iret2 = pthread_create( &thread2, NULL, &hgn_status, NULL);
    iret3 = pthread_create( &thread3, NULL, &hlt_status, NULL);
    iret4 = pthread_create( &thread4, NULL, &battle, NULL);
    iret5 = pthread_create( &thread5, NULL, &bath, NULL);
    iret6 = pthread_create( &thread6, NULL, &standby, NULL);
    iret7 = pthread_create( &thread7, NULL, &shop, NULL);

    fitur = 1;

    iret8 = pthread_create( &thread8, NULL, &input, NULL);

    while(mati == 0){

    }

    shmdt(stok);
    shmctl(shmid, IPC_RMID, NULL);

    return 0;
}