#include "../include/Constants.h"

pthread_mutex_t m;
pthread_mutex_t wrt;
pthread_mutex_t order;
sem_t reading;
int read_counter = 0;

pthread_mutex_t total_lock;
int total_reading_executions = 0;
int total_writing_executions = 0;

int array[length];
