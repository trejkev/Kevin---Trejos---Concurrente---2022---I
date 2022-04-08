#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

void* run1(void*);
void* run2(void*);

int main(){

    pthread_t thread1;
    pthread_t thread2;

    size_t value = sysconf(_SC_NPROCESSORS_ONLN);

    if(pthread_create(&thread1, NULL /*attr*/, run1, (void*)value) == EXIT_SUCCESS && pthread_create(&thread2, NULL /*attr*/, run2, (void*)value) == EXIT_SUCCESS){
        printf("Hello and goodbye world! %zu\n", value);
        pthread_join(thread1, NULL);
        pthread_join(thread2, NULL);
    }else{
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

void* run1(void* param){
    size_t value = (size_t)param;
    printf("Hello world! %zu\n", value);
    return NULL;
}

void* run2(void* param){
    size_t value = (size_t)param;
    printf("Goodbye world! %zu\n", value);
    return NULL;
} 

// ¿Cuántos hilos diferentes se ejecutan en el programa?
//      R/ 3 hilos
// ¿Existe concurrencia en su programa? Si es así, ¿cuáles líneas de código se ejecutan de manera concurrente?
//      R/ Si existe, dentro del if de los pthread_create, de las lineas 16 a la 20
// ¿El valor obtenido con el comando sysconf es igual para todos?¿Qué representa dicho valor?
//      R/ Igual para todos los hilos, mas no todos los companeros, representa la cantidad de nucleos del procesador