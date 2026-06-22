/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routines.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macerver <macerver@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/19 17:56:55 by macerver          #+#    #+#             */
/*   Updated: 2026/06/21 13:39:05 by macerver         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	wait_in_queue(t_dongle *dongle, t_coder *coder)
{
	assign_ticket(coder);
	heap_push(dongle->queue, coder, coder->master->scheduler);
	while (is_running(coder->master))
	{
		if (!dongle->is_taken && dongle->queue->data[0] == coder)
			break ;
		pthread_cond_wait(&coder->cond, &dongle->mutex);
	}
	if (!is_running(coder->master))
		return (0);
	heap_pop(dongle->queue, coder->master->scheduler);
	return (1);
}

static int	take_dongle(t_dongle *d, t_coder *c)
{
	long	wait;

	pthread_mutex_lock(&d->mutex);
	if ((d->is_taken || d->queue->size > 0) && !wait_in_queue(d, c))
	{
		pthread_mutex_unlock(&d->mutex);
		return (0);
	}
	if (!is_running(c->master))
	{
		pthread_mutex_unlock(&d->mutex);
		return (0);
	}
	d->is_taken = 1;
	pthread_mutex_unlock(&d->mutex);
	wait = d->available_at - get_time_ms();
	if (wait > 0)
		usleep(wait * 1000);
	pthread_mutex_lock(&c->master->log_mutex);
	if (is_running(c->master))
		printf("%ld %d has taken a dongle\n",
			get_time_ms() - c->master->start_time, c->id);
	pthread_mutex_unlock(&c->master->log_mutex);
	return (1);
}

static void	release_dongle(t_dongle *dongle, t_coder *coder)
{
	t_coder	*next;

	pthread_mutex_lock(&dongle->mutex);
	dongle->is_taken = 0;
	dongle->available_at = get_time_ms() + coder->master->dongle_cooldown;
	if (dongle->queue->size > 0)
	{
		next = dongle->queue->data[0];
		pthread_cond_signal(&next->cond);
	}
	pthread_mutex_unlock(&dongle->mutex);
}

static int	grab_dongles(t_coder *curr)
{
	t_dongle	*first;
	t_dongle	*second;

	first = curr->r_dongle;
	second = curr->l_dongle;
	if (curr->l_dongle->id < curr->r_dongle->id)
	{
		first = curr->l_dongle;
		second = curr->r_dongle;
	}
	if (!take_dongle(first, curr))
		return (0);
	if (curr->master->number_of_coders == 1)
	{
		while (is_running(curr->master))
			usleep(1000);
		return (0);
	}
	if (!take_dongle(second, curr))
	{
		release_dongle(first, curr);
		return (0);
	}
	return (1);
}

void    *coder_routine(void *coder)
{
	t_coder	*curr;

	curr = (t_coder *)coder;
	while(is_running(curr->master))
	{
		if (!grab_dongles(curr))
			break ;
		coder_compile(curr);
		pthread_mutex_lock(&curr->master->sim_mutex);
		curr->times_compiled++;
		pthread_mutex_unlock(&curr->master->sim_mutex);
		release_dongle(curr->r_dongle, curr);
		release_dongle(curr->l_dongle, curr);
		if (!is_running(curr->master))
			break ;
		coder_debug(curr);
		if (!is_running(curr->master))
			break ;
		coder_refactor(curr);
	}
	return (NULL);
}
