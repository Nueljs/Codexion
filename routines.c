/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routines.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manuel <manuel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/09 15:07:07 by macerver          #+#    #+#             */
/*   Updated: 2026/06/18 06:03:05 by manuel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	take_dongle(t_dongle *dongle, t_coder *coder)
{
	long	wait;

	wait = dongle->release_time + coder->master->dongle_cooldown - get_time_ms();
	pthread_mutex_lock(&dongle->mutex);
	if (dongle->is_taken || dongle->queue->size > 0){
		enqueue(coder, dongle->queue, coder->master->scheduler);
		while ((dongle->is_taken || 
			dongle->queue->data[0] != coder) && is_running(coder->master)){
			pthread_cond_wait(&coder->cond, &dongle->mutex);
			if (!is_running(coder->master)){
				pthread_mutex_unlock(&dongle->mutex);
				return (0);
			}
			dequeue(dongle->queue);
		}
	}
	if (!is_running(coder->master)){
		pthread_mutex_unlock(&dongle->mutex);
		return (0);
	}
	if (wait > 0)
		usleep(wait * 1000);
	dongle->is_taken = 1;
	pthread_mutex_unlock(&dongle->mutex);
	pthread_mutex_lock(&coder->master->log_mutex);
	if (is_running(coder->master))
		printf("%ld %d has taken a dongle\n", (get_time_ms() - coder->master->start_time), coder->id);
	pthread_mutex_unlock(&coder->master->log_mutex);
	return (1);
}

int	is_running(t_master *master)
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
	dongle->release_time = get_time_ms();
	dongle->is_taken = 0;
	next = (dongle->queue->size > 0) ? dongle->queue->data[0] : NULL;
	pthread_mutex_unlock(&dongle->mutex);
	if (next)
		pthread_cond_signal(&next->cond);
}

void	*coder_routine(void *coder)
{
	t_coder	*curr_coder;

	curr_coder = (t_coder *)coder;
	while (is_running(curr_coder->master)){
		if (curr_coder->times_compiled >= curr_coder->master->number_of_compiles_required)
			break ;
		if (curr_coder->l_dongle->id < curr_coder->r_dongle->id){
			if (!take_dongle(curr_coder->l_dongle, curr_coder))
				break ;
			if (!take_dongle(curr_coder->r_dongle, curr_coder)){
				release_dongle(curr_coder->l_dongle);
				break ;
			}
		}
		else{
			if (!take_dongle(curr_coder->r_dongle, curr_coder))
				break ;
			if (!take_dongle(curr_coder->l_dongle, curr_coder)){
				release_dongle(curr_coder->r_dongle);
				break ;
			}
		}
		coder_compile(curr_coder);
		curr_coder->times_compiled++;
		release_dongle(curr_coder->r_dongle);
		release_dongle(curr_coder->l_dongle);
		if (!is_running(curr_coder->master))
			break ;
		coder_debug(curr_coder);
		if (!is_running(curr_coder->master))
			break ;
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
				burnout(&master->coders[i]);
				return (NULL);
			}
		}
		if (compiles_counter(master)){
			pthread_mutex_lock(&master->sim_mutex);
			stop_simulation(master);
			pthread_mutex_unlock(&master->sim_mutex);
			return (NULL);
		}
		usleep(2000);
	}
	return (NULL);
}
