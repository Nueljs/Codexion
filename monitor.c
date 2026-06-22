/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macerver <macerver@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/21 10:59:42 by macerver          #+#    #+#             */
/*   Updated: 2026/06/21 13:44:54 by macerver         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	compiles_counter(t_master *master)
{
	int	compiles;
	int	i;
	int	local_times;
	
	compiles = 0;
	i = -1;
	while (++i < master->number_of_coders)
	{
		pthread_mutex_lock(&master->sim_mutex);
		local_times = master->coders[i].times_compiled;
		pthread_mutex_unlock(&master->sim_mutex);
		if (local_times >= master->number_of_compiles_required)
			compiles++;
	}
	if (compiles == master->number_of_coders)
		return (1);
	return (0);
}

static int	kill_coder(t_master *master, int id, long time)
{
	pthread_mutex_lock(&master->log_mutex);
	if (is_running(master))
	{
		printf("%ld %d burned out\n", time - master->start_time, id);
		pthread_mutex_lock(&master->sim_mutex);
		stop_simulation(master);
		pthread_mutex_unlock(&master->sim_mutex);
	}
	pthread_mutex_unlock(&master->log_mutex);
	return (1);
}

static int	check_deaths(t_master *master)
{
	int		i;
	long	time;
	long	local_deadline;
	
	i = -1;
	while(++i < master->number_of_coders)
	{
		time = get_time_ms();
		pthread_mutex_lock(&master->sim_mutex);
		local_deadline = master->coders[i].deadline;
		pthread_mutex_unlock(&master->sim_mutex);
		if (time >= local_deadline)
			return (kill_coder(master, master->coders[i].id, time));
	}
	return (0);
}

void	*monitor_routine(void *monitor)
{
	t_master	*master;
	
	master = (t_master *)monitor;
	while (is_running(master))
	{
		if (check_deaths(master))
			break ;
		if (compiles_counter(master))
		{
			pthread_mutex_lock(&master->sim_mutex);
			stop_simulation(master);
			pthread_mutex_unlock(&master->sim_mutex);
			break ;
		}
		usleep(1000);
	}
	return (NULL);
}