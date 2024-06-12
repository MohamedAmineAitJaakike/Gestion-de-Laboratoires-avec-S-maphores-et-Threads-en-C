#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <time.h>

#define NB_CHERCHEURS 8

#define NB_UTILISATEURS_SIMULTANES_MAX 3

sem_t semaphore;

void *thread_chercheur(void *id_chercheur) {

    int id = *((int *)id_chercheur);

    printf("\nLe chercheur %d attend pour acceder a l'equipement de recherche.\n", id);

    sem_wait(&semaphore);

    printf("\nLe chercheur %d a obtenu l'acces a l'equipement de recherche.\n", id);

    srand(time(NULL));
    
    sleep(rand() % 5 + 1);

    printf("\nLe chercheur %d a libere l'equipement de recherche.\n\n", id);

    sem_post(&semaphore);

    pthread_exit(NULL);
}

int main() {

    sem_init(&semaphore, 0, NB_UTILISATEURS_SIMULTANES_MAX);

    pthread_t chercheurs[NB_CHERCHEURS];

    int ids_chercheurs[NB_CHERCHEURS];

    for (int i = 0; i < NB_CHERCHEURS; i++) {

        ids_chercheurs[i] = i + 1;

        if (pthread_create(&chercheurs[i], NULL, thread_chercheur, (void *)&ids_chercheurs[i]) != 0) {

            perror("Erreur lors de la creation du thread chercheur");

            exit(EXIT_FAILURE);

        }
    }

    for (int i = 0; i < NB_CHERCHEURS; i++) {

        pthread_join(chercheurs[i], NULL);

    }

    sem_destroy(&semaphore);

    return 0;
}
