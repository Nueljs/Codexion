/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macerver <macerver@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/05 17:30:47 by macerver          #+#    #+#             */
/*   Updated: 2026/06/05 17:52:03 by macerver         ###   ########.fr       */
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
		if (master->coders[i].times_compiled == master->number_of_compiles_required)
			compiles++;
	}
	if (compiles == master->number_of_coders)
		return (1);
	return (0);
}

void	cleanup(t_master *master)
{
	
}