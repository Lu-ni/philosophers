#include "philo.h"

long get_delta_ms(struct timeval *start, struct timeval *current)
{
	return ((current->tv_sec * 1000 + current->tv_usec / 1000) - (start->tv_sec * 1000 + start->tv_usec / 1000));
}

long long current_time(void) {
    struct timeval tv;
    if(gettimeofday(&tv, NULL) == -1)
			printf("something went wrong with getting the time\n");

    return (tv.tv_sec * 1000LL + tv.tv_usec / 1000);
}
void busy_wait(int microseconds)
{
    struct timeval start;
    struct timeval current;
    gettimeofday(&start, NULL);

    long long elapsed = 0;
    while (elapsed < microseconds)
    {
        gettimeofday(&current, NULL);
        elapsed = (current.tv_sec - start.tv_sec) * 1000000LL + (current.tv_usec - start.tv_usec);
    }
}