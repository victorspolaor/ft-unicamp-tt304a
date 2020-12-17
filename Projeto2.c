//Blibliotecas utilizadas
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <time.h>

#define N 20                         //Numero definido para a quantidade de post-its que o pombo suporta


sem_t mutex, vazio, cheio, pombosem; //Definindo semaforos
int cont_postit;                     //Definindo variavel globais que contará os post-its
time_t t;                            //Definindo variavel de tempo para utilizar o srand com mais variedade

//Chamadas de funçoes
void *usuarios(void*);
void *pombof(void*);

//Funçao Main
int main(){

    int i, x, *id;                                                 //Definindo variaveis auxiliares
    cont_postit = 0;                                          //Definindo valor inicial para a quantidade de post-its
    printf("Quantos usuarios escreveram nos post-its?\nR: "); //Perguntando ao usuario quantas threads o programa ira utilizar
    scanf("%i", &i); 
    pthread_t usuario[i], pombo;                              //Definindo as threads

    //iniciando os semaforos
    sem_init(&mutex, 0, 1);
    sem_init(&pombosem, 0, 0);

    //inicializando o pombo;
    pthread_create(&pombo, NULL, pombof, NULL);
    printf("O pombo dormiu enquanto espera os 20 post-it\n");

    //inicializando os usuarios
    for (x = 0; x < i; x++){
        id = (int *)malloc(sizeof(int));
        *id = x + 1;
        pthread_create(&(usuario[x]), NULL, usuarios, id);
    }

    pthread_join(pombo, NULL);
    for (x = 0; x < i; x++){
        pthread_join(usuario[x], NULL);
    }
}

//Funçao para colar o postit
void colapostit(void *arg){
    int id = *((int *) arg);

    sleep(1 + rand() % 1);   //Espera um tempo para o usuario colar o post-it 
    printf("O usuario %d colou um postit. Total de postit: %i\n", id, cont_postit);
    
}

//Funçao que define quanto tempo o pombo demorará na viagem
void leva_mochila_ate_B_e_volta(){
    srand(time(NULL));
    sleep(1 + (rand() % 3));

}

//Funçao que define o tempo q o usuario demorará pra escrever o post-it
void dormealeatorio(){
    srand(time(NULL));
    sleep(1 + (rand() % 3));
}

//Funçao para o funcionamento das Threads Usuarios
void *usuarios(void *arg){
    while (1){
        sem_wait(&mutex);
        dormealeatorio();
        cont_postit++; 
        colapostit(arg);
        if(cont_postit == N){
            printf("ATENÇAO! A MOCHILA ESTA CHEIA!!!\n");
            sem_post(&pombosem);
            sem_wait(&mutex);
            
        }
        sem_post(&mutex);
    }
}

//Funçao para o funcionamento da Thread Pombo
void *pombof(void *p_arg){
    while (1){        
        sem_wait(&pombosem);
        printf("**O pombo acordou e ira levar as mensagens ate o ponto B**\n");
        leva_mochila_ate_B_e_volta();
        printf("**O pombo retornou e irá dormir enquanto espera os 20 post-it**\n");
        cont_postit = 0;
        sem_post(&mutex);
        sem_wait(&pombosem);
    }
}

