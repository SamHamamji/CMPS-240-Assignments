#include "../include/Constants.h"

pthread_mutex_t total_lock;
pthread_mutex_t mutex;

int array[length];
State current_state = writing;
