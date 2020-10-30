#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <time.h>

#define N 20

sem_t mutex, colapostit, limpostit;
int cont_postit;
pthread_mutex_t mutexsum;
time_t t;

//Funçao para color o postit
void fcolapostit(int id){
    sleep(1 + rand() % 1);
    cont_postit++;
    printf("O usuario %i colou um postit. Total de postit: %i\n", id, cont_postit);
}

//Funçao que define o momento que o pombo vai ate o lado b e o tempo q ele vai levar pra volta
void leva_mochila_ate_B_e_volta(){
    printf("teste 1\n");
    sem_post(&mutex);
    sem_wait(&mutex);
    cont_postit = 0;
    for(int i = 0; i < N; i++){
        sem_post(&limpostit);
        printf("%i\n", i);
    }

    srand(time(NULL));
    sleep(1 + (rand() % 1));
    
}

//Funçao que define quando o pombo dorme e quando ele eh liberado 
void dorme_aleatorio(){
    if(cont_postit != 20){
        printf("O pombo dormiu\n");
        sem_post(&mutex);
        sem_wait(&colapostit);
    }
    printf("O pombo acordou e ira levas as mensagens ate o ponto B\n");
}    

//Funçao para o funcionamento das Threads Usuarios
void* usuarios(void* threadid){
    int uid = *(int*)threadid;
    while(1){
        sem_wait(&mutex);
        sem_wait(&limpostit);
        fcolapostit(uid);
        if(cont_postit == 20){
            sem_post(&colapostit);
        }
        sem_post(&mutex);
    }
    pthread_exit(NULL);
}

//Funçao para o funcionamento da Thread Pombo
void* pombof(){
    while(1){
        if(cont_postit == 20){
            leva_mochila_ate_B_e_volta();
            printf("teste 3");
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
    sem_init(&mutex,0,1);
    sem_init(&limpostit, 0, 20);
    sem_init(&colapostit,0,0);

    //inicializando e testando o pombo;
    pombostatus = pthread_create(&pombo, NULL, pombof, NULL);
    if(pombostatus != 0){
        printf("Erro ao iniciar o pombo\n");
        exit(EXIT_FAILURE);
    }
    
    //inicializando os usuarios
    for(x = 0; x < i; x++){
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

