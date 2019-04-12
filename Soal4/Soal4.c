#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

int status1 = 0; 
int status2 = 0;

void *ps(void *args){
    int *proses = (int *) args;
    if(*proses == 1){
        system("ps -aux | head -11 | tail -10 > /home/ismail/Documents/FolderProses1/SimpanProses1.txt");
        status1 = 1;
    }
    if(*proses == 2){
        system("ps -aux | head -11 | tail -10 > /home/ismail/Documents/FolderProses2/SimpanProses2.txt");
        status2 = 1;
    }
}

void *zip(void *args){
    int *proses = (int *) args;

    while(status1 != 1 || status2 != 1){

    }
    
    if(*proses == 1 && status1 == 1){
        system("zip -jm /home/ismail/Documents/FolderProses1/KompresProses1.zip /home/ismail/Documents/FolderProses1/SimpanProses1.txt");
        printf("Menunggu 15 detik untuk mengekstrak kembali\n");
        sleep(15);
        status1 = 2;
    }
    if(*proses == 2 && status2 == 1){
        system("zip -jm /home/ismail/Documents/FolderProses2/KompresProses2.zip /home/ismail/Documents/FolderProses2/SimpanProses2.txt");
        sleep(15);
        status2 = 2;
    }
    
}

void *unzip(void *args){
    int *proses = (int *) args;

    while(status1 != 2 || status2 != 2){
        
    }

    if(*proses == 1 && status1 == 2){
        system("unzip /home/ismail/Documents/FolderProses1/KompresProses1.zip -d /home/ismail/Documents/FolderProses1/");
    }
    if(*proses == 2 && status2 == 2){
        system("unzip /home/ismail/Documents/FolderProses2/KompresProses2.zip -d /home/ismail/Documents/FolderProses2/");
    }
}

int main(){
    pthread_t thread1, thread2, thread3, thread4, thread5, thread6;
    int  iret1, iret2, iret3, iret4, iret5, iret6;
    int proses1 = 1;
    int proses2 = 2;

    iret1 = pthread_create( &thread1, NULL, &ps, (void*) &proses1);
    iret2 = pthread_create( &thread2, NULL, &ps, (void*) &proses2);
    iret3 = pthread_create( &thread3, NULL, &zip, (void*) &proses1);
    iret4 = pthread_create( &thread4, NULL, &zip, (void*) &proses2);
    iret5 = pthread_create( &thread5, NULL, &unzip, (void*) &proses1);
    iret6 = pthread_create( &thread6, NULL, &unzip, (void*) &proses2);

    pthread_join(thread3,NULL);
    pthread_join(thread4,NULL);
    pthread_join(thread1,NULL);
    pthread_join(thread2,NULL);
    pthread_join(thread5,NULL);
    pthread_join(thread6,NULL);
}

