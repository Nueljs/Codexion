/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macerver <macerver@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/03 13:45:02 by macerver          #+#    #+#             */
/*   Updated: 2026/06/06 18:15:59 by macerver         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	init_master(t_master *master)
{
	int	i;
	
	master->simulation_running = 1;
	master->start_time = get_time_ms();
	pthread_mutex_init(&master->log_mutex, NULL);
	pthread_mutex_init(&master->sim_mutex, NULL);
	master->coders = malloc(master->number_of_coders * sizeof(t_coder));
	master->dongles = malloc(master->number_of_coders * sizeof(t_dongle));
	i = 0;
	while (i < master->number_of_coders)
	{
		master->coders[i].last_compile_start = 0;
		master->coders[i].master = &master;
		master->coders[i].times_compiled = 0;
		master->coders[i].id = i + 1;
		master->dongles[i].id = i + 1;
		master->dongles[i].release_time = 0;
		master->dongles[i].is_taken = 0;
		pthread_mutex_init(&master->dongles[i].mutex, NULL);
		pthread_cond_init(&master->dongles[i].cond, NULL);
		i++;
	}
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
