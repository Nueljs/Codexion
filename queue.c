/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   queue.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manuel <manuel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/13 17:59:16 by macerver          #+#    #+#             */
/*   Updated: 2026/06/17 06:10:36 by manuel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	fifo(t_coder *coder, t_heap *queue)
{
	int	i;
	
	i = queue->size;
	queue->data[i] = coder;
	queue->size++;
}

static int	calc_deadline(t_coder *coder)
{
	int	deadline;

	deadline = ((coder->last_compile_start == 0)
		? coder->master->start_time
		: coder->last_compile_start) +
		coder->master->time_to_burnout;
	return (deadline);
}

static void	edf(t_coder *coder, t_heap *queue)
{
	int	i;
	int	deadline_curr;
	int deadline;
	int	size;

	size = queue->size;
	deadline_curr = calc_deadline(coder);
	i = -1;
	while (++i < queue->size){
		deadline = calc_deadline(queue->data[i]);
		if (deadline > deadline_curr){
			while (size > i){
				queue->data[size] = queue->data[size - 1];
				size--;
			}
			queue->data[i] = coder;
			queue->size++;
			return ;
		}
	}
	queue->data[size] = coder;
	queue->size++;
}

void	enqueue(t_coder *coder, t_heap *queue, char	*scheduler)
{
	if (strcmp(scheduler, "fifo") == 0)
		fifo(coder, queue);
	else if (strcmp(scheduler, "edf") == 0)
		edf(coder, queue);
}

t_coder    *dequeue(t_heap *queue)
{
    int i;
	t_coder	*coder;

	if (queue->size == 0)
		return (NULL);
    i = 0;
	coder = queue->data[i];
    while (++i < queue->size)
		queue->data[i - 1] = queue->data[i];
	queue->size--;
	return (coder);
}
