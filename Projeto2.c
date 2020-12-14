#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <time.h>

#define N 20
#define limite 1000

sem_t mutex, vazio, cheio, pombosem, usuariosem;
int cont_postit;
time_t t;

void reseta();
void fcolapostit();
void leva_mochila_ate_B_e_volta();
void *usuarios(void*);
void *pombof(void*);

//Funçao Main
int main(){

    int i, x; //Definindo variaveis auxiliares
    cont_postit = 0; //Definindo valor inicial para a quantidade de post-its
    printf("Quantos usuarios escreveram nos post-its?\nR: "); //Perguntando ao usuario quantas threads o programa ira utilizar
    scanf("%i", &i); 
    pthread_t usuario[i], pombo; //Definindo as threads

    //iniciando os semaforos
    sem_init(&usuariosem, 1, 0);
    sem_init(&mutex, 1, 1);
    sem_init(&pombosem, 1, 0);
    sem_init(&vazio, 0, N);
    sem_init(&cheio, 0, 0);

    //inicializando o pombo;
    pthread_create(&pombo, NULL, pombof, NULL);
    printf("O pombo dormiu enquanto espera os 20 post-it\n");

    //inicializando os usuarios
    for (x = 0; x < i; x++){
        pthread_create(&(usuario[x]), NULL, usuarios, NULL);
    }

    pthread_join(pombo, NULL);
    for (x = 0; x < i; x++){
        pthread_join(usuario[x], NULL);
    }
}


//Funçao que reseta os semaforos que controlam o "buffer" de post- its
void reseta(){
    for(int i = 0;i < 20; i++){
        sem_post(&vazio);
        sem_wait(&cheio);
    }
}

//Funçao para colar o postit
void fcolapostit(){
    sleep(1 + rand() % 1); //Espera um tempo para o usuario colar o post-it 
    cont_postit++;         //e logo em segui incrementa um bilhete no contador
    printf("Algum usuario colou um postit. Total de postit: %i\n", cont_postit);
    if(cont_postit == 20){ //Verifica se existem 20 post-it no pombo para que ele posso envia-los
        sem_post(&pombosem);
    }
}

//Funçao que define o momento que o pombo vai ate o lado b e o tempo q ele vai levar pra volta
void leva_mochila_ate_B_e_volta()
{
    cont_postit = 0; //Quando o pombo sai para levar os bilhetes eles sao zerados
    srand(time(NULL));
    sleep(1 + (rand() % 1));
    reseta();
    printf("O pombo retornou e irá dormir enquanto espera os 20 post-it\n");
    sem_wait(&pombosem);
    sem_post(&mutex);


}



//Funçao para o funcionamento das Threads Usuarios
void *usuarios(void *p_arg){
    while (1){
        if(cont_postit == 20){
            sem_post(&pombosem);
            sem_wait(&mutex);
        }
        sem_wait(&mutex);
        sem_wait(&vazio);
        
        fcolapostit();
        
        sem_post(&mutex);
        sem_post(&cheio);
    }
}

//Funçao para o funcionamento da Thread Pombo
void *pombof(void *p_arg){
    while (1){
        sem_wait(&pombosem);
        printf("O pombo acordou e ira levar as mensagens ate o ponto B\n");
        leva_mochila_ate_B_e_volta();
        
    }
}
