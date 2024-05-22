#include "philo.h"

void init_philos(t_params *params, t_philo *philos) {
    for (int i = 0; i < params->number_of_philosophers; i++) {
        philos[i].id = i;
        philos[i].left_fork = &params->forks[i];
        philos[i].right_fork = &params->forks[(i + 1) % params->number_of_philosophers];
        philos[i].params = params;
        philos[i].last_meal_time = current_time();
        philos[i].meals_eaten = 0;
        pthread_mutex_init(&philos[i].last_meal_lock, NULL);
    }
    params->start_time = current_time();
}

void print_status(t_philo *philo, const char *status) {
    t_params *params = philo->params;
    pthread_mutex_lock(&params->print_lock);
    printf("%lld %d %s\n", current_time() - params->start_time, philo->id + 1, status);
    pthread_mutex_unlock(&params->print_lock);
}

void cleanup(t_params *params, t_philo *philos) {
    for (int i = 0; i < params->number_of_philosophers; i++)
        pthread_mutex_destroy(&params->forks[i]);
    pthread_mutex_destroy(&params->print_lock);
    free(params->forks);
}
/*
The philosophers alternatively eat, think, or sleep.
While they are eating, they are not thinking nor sleeping;
while thinking, they are not eating nor sleeping;
and, of course, while sleeping, they are not eating nor thinking.
*/
void *philosopher_thread(void *args)
{
	t_philo *philo = (t_philo *) args;
	while(1)
	{
		pthread_mutex_lock(philo->left_fork);
		print_status(philo, "as taken the left fork");
		pthread_mutex_lock(philo->right_fork);
		print_status(philo, "as taken the right fork");
		print_status(philo, "is eating");
		usleep(philo->params->time_to_eat * 1000);
		pthread_mutex_lock(&philo->last_meal_lock);
		philo->last_meal_time = (long long) current_time;
		pthread_mutex_unlock(&philo->last_meal_lock);
		pthread_mutex_unlock(philo->right_fork);
		pthread_mutex_unlock(philo->left_fork);
		print_status(philo, "is thinking");
		usleep(philo->params->time_to_think * 1000);
		print_status(philo, "is sleeping");
		usleep(philo->params->time_to_sleep * 1000);
	}
	return NULL;
}
