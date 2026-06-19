#include "codexion.h"

void	assign_ticket(t_coder *coder)
{
	pthread_mutex_lock(&coder->master->ticket_mutex);
	coder->ticket_number = coder->master->ticket_counter;
	coder->master->ticket_counter++;
	pthread_mutex_unlock(&coder->master->ticket_mutex);
}


long    get_priority(t_coder *coder, char *scheduler)
{
	if (strcmp(scheduler, "edf") == 0)
		return (coder->deadline);
	else if (strcmp(scheduler, "fifo") == 0)
		return (coder->ticket_number);
	return (0);
}

long	get_time_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}
