#ifndef PHILO_H
#define PHILO_H

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>

typedef struct s_params {
    int number_of_philosophers;
    int time_to_die;
    int time_to_eat;
    int time_to_sleep;
    int number_of_times_each_philosopher_must_eat;
	int dead;
    pthread_mutex_t *forks;
    pthread_mutex_t print_lock;
    long long start_time;
} t_params;

typedef struct s_philo {
    int id;
    pthread_t thread;
    pthread_mutex_t *left_fork;
    pthread_mutex_t *right_fork;
    pthread_mutex_t last_meal_lock;
    struct s_params *params;
    long long last_meal_time;
    int meals_eaten;
} t_philo;

long get_delta_ms(struct timeval *start, struct timeval *current);
long long current_time(void);
void cleanup(t_params *params, t_philo *philos);
void print_status(t_philo *philo, const char *status);
void init_philos(t_params *params, t_philo *philos);
void *philosopher_thread(void *args);
#endif
