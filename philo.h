#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <unistd.h>

typedef struct s_params
{
	int				num_philo;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				max_serving;
	int				philos_done;
	int				dead;
	pthread_mutex_t	lock_dead;
	pthread_mutex_t	*forks;
	pthread_mutex_t	print_lock;
	pthread_mutex_t	philos_done_lock;
	long long		start_time;
}					t_params;

typedef struct s_philo
{
	int				id;
	pthread_t		thread;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	pthread_mutex_t	last_meal_lock;
	struct s_params	*params;
	long long		last_meal_time;
	int				meals_eaten;
}					t_philo;

long long			current_time(void);
void				cleanup(t_params *params, t_philo *philos);
void				print_status(t_philo *philo, const char *status);
void				init_philos(t_params *params, t_philo *philos);
void				*philosopher_thread(void *args);
void				busy_wait(int microseconds);
int					ft_atoi(const char *str);
void				*supervising(void *arr);

#endif
