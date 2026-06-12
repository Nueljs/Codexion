/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macerver <macerver@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/10 12:26:26 by macerver          #+#    #+#             */
/*   Updated: 2026/06/10 12:55:12 by macerver         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	coder_compile(t_coder *coder)
{
	pthread_mutex_lock(&coder->master->log_mutex);
	printf("%ld %d is compiling\n", (get_time_ms() - coder->master->start_time), coder->id);
	pthread_mutex_unlock(&coder->master->log_mutex);
	usleep(coder->master->time_to_compile * 1000);
}

void	coder_debug(t_coder *coder)
{
	pthread_mutex_lock(&coder->master->log_mutex);
	printf("%ld %d is debugging\n", (get_time_ms() - coder->master->start_time), coder->id);
	pthread_mutex_unlock(&coder->master->log_mutex);
	usleep(coder->master->time_to_debug * 1000);
}

void	coder_refactor(t_coder *coder)
{
	pthread_mutex_lock(&coder->master->log_mutex);
	printf("%ld %d is refactoring\n", (get_time_ms() - coder->master->start_time), coder->id);
	pthread_mutex_unlock(&coder->master->log_mutex);
	usleep(coder->master->time_to_refactor * 1000);
}