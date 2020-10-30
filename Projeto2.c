#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <time.h>

sem_t sempombo, colapostit;
int cont_postit;
pthread_mutex_t mutexsum;
time_t t;

//Funçao para color o postit
void fcolapostit(int id){
    sleep(1 + (rand() % 1));
    cont_postit++;
    printf("O usuario %i colou um postit. Total de postit: %i\n", id, cont_postit);
}

//Funçao que define o momento que o pombo vai ate o lado b e o tempo q ele vai levar pra volta
void leva_mochila_ate_B_e_volta(){
    sem_wait(&sempombo);
    sem_wait(&colapostit);
    srand(time(NULL));
    sleep(1 + (rand() % 4));
    cont_postit = 0;
    sem_post(&sempombo);
    sem_post(&colapostit);
}

//Funçao que define quando o pombo dorme e quando ele eh liberado 
void dorme_aleatorio(){
    if(cont_postit != 20){
        printf("O pombo dormiu\n");
    }
    sem_wait(&sempombo);
    while(cont_postit != 20){
        sleep(1);
    }
    printf("O pombo acordou\n");
    sem_post(&sempombo);
}    

//Funçao para o funcionamento das Threads Usuarios
void* usuarios(void* threadid){
    int uid = *(int*)threadid;
    while(1){
        fcolapostit(uid);
        sched_yield();
    }
    pthread_exit(NULL);
}

//Funçao para o funcionamento da Thread Pombo
void* pombof(){
    while(1){
        if(cont_postit == 20){
            leva_mochila_ate_B_e_volta();
        }else{
            dorme_aleatorio();
        }
    }
    pthread_exit(NULL);
}

//Funçao Main        
int main(){      

    int i, pstatus, x, pombostatus;
    cont_postit = 0;
    scanf("%i", &i);
    pthread_t usuario[i], pombo;

    //iniciando os semaforos
    sem_init(&sempombo,0,1);
    sem_init(&colapostit,0,0);

    //inicializando e testando o pombo;
    pombostatus = pthread_create(&pombo, NULL, pombof, NULL);
    if(pombostatus != 0){
        printf("Erro ao iniciar o pombo\n");
        exit(EXIT_FAILURE);
    }
    
    //inicializando os usuarios
    for(x = 0; x < i; x++){
        printf("Criando thread do cliente %i\n", x);
        pstatus = pthread_create(&(usuario[x]), NULL, usuarios, &x);
        //Verificaçao para ver se a thread foi executada com exito
        if(pstatus != 0){
            printf("Erro na criaçao da thread %i\n", x);
            exit (EXIT_FAILURE);
        }
    }
    pstatus = pthread_join(pombo, NULL);
    for(x = 0; x < i; x++){
        pstatus = pthread_join(usuario[x], NULL);
        if(pstatus != 0){
            printf("Erro ao aguardar finalização do thread %i.\n", i);
            exit (EXIT_FAILURE);
        }
    }


}

