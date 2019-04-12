#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>

int fac[100];
long long int res[100];
int x = 0;

void *factorial(void *args){
    int *n = (int *) args;
    int c;
    long long int result=1;

    for (c=1; c<=*n; c++)
        result = result*c;

    fac[x] = *n;
    res[x] = result;
    x++;
}

int main(int argc, char *argv[]){
    int arc = argc-1;
    pthread_t tid[arc];
    int n[100];
    for (int i=1; i<argc; i++){
        n[i-1] = atoi(argv[i]);
    }

    int t = 0;
    while(t<arc){
        pthread_create(&(tid[t]),NULL,&factorial,(void*) &n[t]);

		t++;
    }
    int tj = 0;
    while(tj<arc){
        pthread_join(tid[tj],NULL);

        tj++;
    }
    
    for (int i=0; i<arc-1; i++){
        for (int j=0; j<arc-i-1; j++){
            if(fac[j] > fac[j+1]){
                int temp = fac[j];
                fac[j] = fac[j+1];
                fac[j+1] = temp;
                
                long long int temps = res[j];
                res[j] = res[j+1];
                res[j+1] = temps; 
            }
        }
    }
    int pr = 0;
    while(pr<arc){
        printf("%d! = %lld\n", fac[pr], res[pr]);

        pr++;
    }

    return 0;
}