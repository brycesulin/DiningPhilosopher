/*
 * This Program implement the classical 'Dining-Philosophers' problem
 * by using Pthreads mutex locks and condition variables.
 *
 * Compile: gcc Dining-Philosophers.c
 *
 * Run:     ./a.out
 */

#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <zconf.h>

#define N 5
#define THINKING 2
#define HUNGRY 1
#define EATING 0
#define LEFT (Philosopher_ID + 4) % N
#define RIGHT (Philosopher_ID + 1) % N

int state[N];
int phil[N] = {0, 1, 2, 3, 4};

sem_t mutex;
sem_t S[N];

void Test_Dining(int Philosopher_ID) {
    if (state[Philosopher_ID] == HUNGRY
        && state[LEFT] != EATING
        && state[RIGHT] != EATING) {

        // Eating State
        state[Philosopher_ID] = EATING;

        sleep(2);

        printf("Philosopher %d takes chopsticks %d and %d\n",
               Philosopher_ID + 1, LEFT + 1, Philosopher_ID + 1);

        printf("Philosopher %d is eating\n", Philosopher_ID + 1);

        sem_post(&S[Philosopher_ID]);
    }
}

// Pick up the chopsticks
void Pickup_Forks(int Philosopher_ID) {

    sem_wait(&mutex);

    // Hungry State
    state[Philosopher_ID] = HUNGRY;

    printf("Philosopher %d is hungry\n", Philosopher_ID + 1);

    // Eat if adjacent neighbours are not eating
    Test_Dining(Philosopher_ID);

    sem_post(&mutex);

    // Wait to be signalled if unable to eat
    sem_wait(&S[Philosopher_ID]);

    sleep(1);
}

// Put down the chopsticks
void Return_Forks(int Philosopher_ID) {

    sem_wait(&mutex);

    // Thinking State
    state[Philosopher_ID] = THINKING;

    printf("Philosopher %d putting chopsticks %d and %d down\n",
           Philosopher_ID + 1, LEFT + 1, Philosopher_ID + 1);
    printf("Philosopher %d is thinking\n", Philosopher_ID + 1);

    Test_Dining(LEFT);
    Test_Dining(RIGHT);

    sem_post(&mutex);
}

void *philospher(void *num) {

    while (1) {

        int *i = num;

        sleep(1);

        Pickup_Forks(*i);

        sleep(1);

        Return_Forks(*i);
    }
}

int main() {

    int i;
    pthread_t thread_id[N];

    // Initialize semaphores
    sem_init(&mutex, 0, 1);

    for (i = 0; i < N; i++)

        sem_init(&S[i], 0, 0);

    for (i = 0; i < N; i++) {

        // Create philosopher threads
        pthread_create(&thread_id[i], NULL,
                       philospher, &phil[i]);

        printf("Philosopher %d is thinking\n", i + 1);
    }

    for (i = 0; i < N; i++)

        pthread_join(thread_id[i], NULL);
}