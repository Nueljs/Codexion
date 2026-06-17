/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: manuel <manuel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/10 12:26:26 by macerver          #+#    #+#             */
/*   Updated: 2026/06/17 06:11:19 by manuel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	coder_compile(t_coder *coder)
{
	if (!is_running(coder->master))
		return ;
	pthread_mutex_lock(&coder->master->log_mutex);
	printf("%ld %d is compiling\n", (get_time_ms() - coder->master->start_time), coder->id);
	pthread_mutex_unlock(&coder->master->log_mutex);
	coder->last_compile_start = get_time_ms();
	usleep(coder->master->time_to_compile * 1000);
}

void	coder_debug(t_coder *coder)
{
	if (!is_running(coder->master))
		return ;
	pthread_mutex_lock(&coder->master->log_mutex);
	printf("%ld %d is debugging\n", (get_time_ms() - coder->master->start_time), coder->id);
	pthread_mutex_unlock(&coder->master->log_mutex);
	usleep(coder->master->time_to_debug * 1000);
}

void	coder_refactor(t_coder *coder)
{
	if (!is_running(coder->master))
		return ;
	pthread_mutex_lock(&coder->master->log_mutex);
	printf("%ld %d is refactoring\n", (get_time_ms() - coder->master->start_time), coder->id);
	pthread_mutex_unlock(&coder->master->log_mutex);
	usleep(coder->master->time_to_refactor * 1000);
}