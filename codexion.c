/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macerver <macerver@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/03 13:45:02 by macerver          #+#    #+#             */
/*   Updated: 2026/06/05 16:55:33 by macerver         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	init_master(t_master *master)
{
	master->simulation_running = 1;
	master->start_time = get_time_ms();
	pthread_mutex_init(&master->log_mutex, NULL);
	pthread_mutex_init(&master->sim_mutex, NULL);
	master->coders = malloc(master->number_of_coders * sizeof(t_coder));
	master->dongles = malloc(master->number_of_coders * sizeof(t_dongle));
}

int	main(int argc, char **argv)
{
	t_master	master;

	if (argc != 9)
	{
		printf("Error\n");
	}
	if (parse_args(&master, argv) != 0)
	{
		printf("Error\n");
		return (1);
	}
	init_master(&master);
	return (0);
}
