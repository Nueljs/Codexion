/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macerver <macerver@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/19 17:57:24 by macerver          #+#    #+#             */
/*   Updated: 2026/06/21 11:16:06 by macerver         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	assign_ticket(t_coder *coder)
{
	pthread_mutex_lock(&coder->master->ticket_mutex);
	coder->ticket_number = coder->master->ticket_counter;
	coder->master->ticket_counter++;
	pthread_mutex_unlock(&coder->master->ticket_mutex);
}


long	get_priority(t_coder *coder, char *scheduler)
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

void	stop_simulation(t_master *master)
{
	int	i;

	i = -1;
	master->simulation_running = 0;
	while (++i < master->number_of_coders)
		pthread_cond_broadcast(&master->coders[i].cond);
}

int	is_running(t_master *master)
{
	int	running;

	pthread_mutex_lock(&master->sim_mutex);
	running = master->simulation_running;
	pthread_mutex_unlock(&master->sim_mutex);
	return (running);
}
