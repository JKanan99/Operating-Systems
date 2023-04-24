#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <pthread.h>
#include <fcntl.h>
#include <sys/types.h>
#include <semaphore.h>

#define NPHILO 5
#define MAX_SLEEP_TIME 2
#define MAX_EATEN 3

pthread_mutex_t     mutex_lock;

sem_t* semaphore[NPHILO] = {NULL};
const char* semaphore_name[NPHILO] = {"sem1", "sem2", "sem3", "sem4", "sem5"};

typedef enum {THINKING, HUNGRY, EATING} STATE;

pthread_t tid[NPHILO];
STATE state[NPHILO];
int thread_id[NPHILO];
int have_eaten[NPHILO];

const char* to_string(STATE state, int haveEaten) {
	static char buf[80];

	switch(state) {
	case THINKING: return "thinking";
	case HUNGRY:   return "*Hungry*";
	case EATING:
		memset(buf, 0, sizeof(buf));
		snprintf(buf, sizeof(buf), "EATING (%d)", haveEaten);
		return buf;
	default:	return "-------- invalid state --------";
	}
}
void show_all_philosophers(void) {
	pthread_mutex_lock(&mutex_lock);
	printf("---------------------------------------------------------------+\n");
	for (int i = 0; i < NPHILO; ++i) {
		printf("%10s | ", to_string(state[i], have_eaten[i]));
	}
	printf("\n");
	pthread_mutex_unlock(&mutex_lock);
}
int onleft(int n)  { return (n + NPHILO - 1) % NPHILO; }
int onright(int n) { return (n + 1) % NPHILO; }

int random_time(void) { return (int)((random() % MAX_SLEEP_TIME) + 1); }
void thinking(void) { sleep(random_time()); }
void eating(void)   { sleep(random_time()); }

bool pickup_left(int i) {
	bool result = false;
	if (state[onleft(i)] != EATING) {
		sem_wait(semaphore[i]); 
		result = true;
	}
	return result;
}
bool pickup_right(int i) {
	bool result = false;
	if (state[onright(i)] != EATING) {
		sem_wait(semaphore[(i + 1) % 5]);
		result = true;
	}
	return result;
}
void pickup_chopsticks(int n) {
	state[n] = HUNGRY;
	bool holding_left = false, holding_right = false;

	while (state[n] == HUNGRY) {
		if ((holding_left = pickup_left(n))) { holding_right = pickup_right(n); }

		if (n % 2 == 0) {   
			if ((holding_left = pickup_left(n))) { 
			    holding_right = pickup_right(n); 
            }
		} else {  
			if ((holding_right = pickup_right(n))) { 
			    holding_left = pickup_left(n); 
            }
		}

		if (holding_left && holding_right) {  
			++have_eaten[n];
			state[n] = EATING;
			show_all_philosophers();
		}
		else {  
			if (holding_left)  { sem_post(semaphore[onleft(n)]); }
			if (holding_right) { sem_post(semaphore[onright(n)]); }
		}
	}
}

void return_chopsticks(int n) {
    state[n] = THINKING;
    sem_post(semaphore[onleft(n)]);     // signal HUNGRY left/right neighbors to eat
    sem_post(semaphore[onright(n)]);
}

void* philosopher(void* param) {
    int number = *(int *)param;
    
    srandom((unsigned)time(NULL));
    while (have_eaten[number] < MAX_EATEN) {
        thinking();
        pickup_chopsticks(number);
        eating();
        return_chopsticks(number);
    }
    return NULL;
}

void init_philos(void) {           
    for (int i = 0; i < NPHILO; i++) {
        state[i] = THINKING;
        have_eaten[i] = 0;
        thread_id[i] = i;
    }
}

void run_simulation(void) { 
    init_philos();
    printf("==================================================================\n");
    printf("|                    ====== PHILOSOPHERS ======                  |\n");
    printf("==================================================================\n");
    printf("   Phil_0  |   Phil_1   |   Phil_2   |   Phil_3   |   Phil_4   |\n");
    show_all_philosophers();
    for (int i = 0; i < NPHILO; i++) { 
      pthread_create(&tid[i], 0, philosopher, (void *)&thread_id[i]);
    }
    for (int i = 0; i < NPHILO; i++) {
      pthread_join(tid[i], NULL);
    }
    printf("\n\t\t...done.\n");
}

int main(int argc, const char** argv) {
	run_simulation();
    return 0;
}