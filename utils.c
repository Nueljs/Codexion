/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macerver <macerver@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/05 17:30:47 by macerver          #+#    #+#             */
/*   Updated: 2026/06/16 18:14:13 by macerver         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

long	get_time_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

int	compiles_counter(t_master *master)
{
	int	compiles;
	int	i;

	i = -1;
	compiles = 0;
	while (++i < master->number_of_coders)
	{
		printf("coder %d: %d compilaciones\n", master->coders[i].id, master->coders[i].times_compiled);
		if (master->coders[i].times_compiled >= master->number_of_compiles_required)
			compiles++;
	}
	if (compiles == master->number_of_coders)
		return (1);
	return (0);
}

void	cleanup(t_master *master)
{
	int	i;

	i = -1;
	while(++i < master->number_of_coders)
	{
		free(master->dongles[i].queue->data);
		free(master->dongles[i].queue);
		pthread_mutex_destroy(&master->dongles[i].mutex);
		pthread_cond_destroy(&master->coders[i].cond);
	}
	free(master->coders);
	free(master->dongles);
	pthread_mutex_destroy(&master->log_mutex);
	pthread_mutex_destroy(&master->sim_mutex);
}

void	stop_simulation(t_master *master)
{
	int	i;

	i = -1;
	master->simulation_running = 0;
	while (++i < master->number_of_coders)
		pthread_cond_broadcast(&master->coders[i].cond);
}
