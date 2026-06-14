/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routines.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macerver <macerver@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/09 15:07:07 by macerver          #+#    #+#             */
/*   Updated: 2026/06/13 18:19:02 by macerver         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	take_dongle(t_dongle *dongle, t_coder *coder)
{
	pthread_mutex_lock(&dongle->mutex);
	enqueue(coder, dongle->queue, coder->master->scheduler);
	while (dongle->is_taken)
		pthread_cond_wait(&coder->cond, &dongle->mutex);
	dongle->is_taken = 1;
	pthread_mutex_unlock(&dongle->mutex);
	return (1);
}

static int	is_running(t_master *master)
{
	int	running;

	pthread_mutex_lock(&master->sim_mutex);
	running = master->simulation_running;
	pthread_mutex_unlock(&master->sim_mutex);
	return (running);
}
static void	release_dongle(t_dongle *dongle)
{
	t_coder *next;

	pthread_mutex_lock(&dongle->mutex);
	next = dequeue(dongle->queue);
	dongle->is_taken = 0;
	dongle->release_time = get_time_ms();
	pthread_mutex_unlock(&dongle->mutex);
	if (next)
	{
		usleep(next->master->dongle_cooldown * 1000);
		pthread_cond_signal(&next->cond);
	}
}

void	*coder_routine(void *coder)
{
	t_coder	*curr_coder;

	curr_coder = (t_coder *)coder;
	while (is_running(curr_coder->master)){
		if (curr_coder->l_dongle->id < curr_coder->r_dongle->id){
			take_dongle(curr_coder->l_dongle, curr_coder);
			take_dongle(curr_coder->r_dongle, curr_coder);
		}
		else{
			take_dongle(curr_coder->r_dongle, curr_coder);
			take_dongle(curr_coder->l_dongle, curr_coder);
		}
		coder_compile(curr_coder);
		curr_coder->times_compiled++;
		release_dongle(curr_coder->l_dongle);
		release_dongle(curr_coder->r_dongle);
		coder_debug(curr_coder);
		coder_refactor(curr_coder);
	}
	return (NULL);
}

void	*monitor_routine(void *monitor)
{
	t_master	*master;
	int			i;
	long		ref;
	
	master = (t_master *)monitor;
	while(is_running(master)){
		i = -1;
		while (++i < master->number_of_coders){
			ref = (master->coders[i].last_compile_start == 0)
				? master->start_time
				: master->coders[i].last_compile_start;
			if(get_time_ms() - ref > master->time_to_burnout){
				pthread_mutex_lock(&master->sim_mutex);
				master->simulation_running = 0;
				pthread_mutex_unlock(&master->sim_mutex);
			}
		}
		if (compiles_counter(master)){
			pthread_mutex_lock(&master->sim_mutex);
			master->simulation_running = 0;
			pthread_mutex_unlock(&master->sim_mutex);
		}
		usleep(10000);
	}
	return (NULL);
}
