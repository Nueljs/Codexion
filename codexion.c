/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macerver <macerver@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/03 13:45:02 by macerver          #+#    #+#             */
/*   Updated: 2026/06/11 11:15:00 by macerver         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	init_coders_and_dongles(t_master *master)
{
	int	i;
	
	i = -1;
	while (++i < master->number_of_coders){
		master->coders[i].last_compile_start = 0;
		master->coders[i].master = master;
		master->coders[i].times_compiled = 0;
		master->coders[i].id = i + 1;
		pthread_cond_init(&master->coders[i].cond, NULL);
		master->dongles[i].id = i + 1;
		master->dongles[i].release_time = 0;
		master->dongles[i].is_taken = 0;
		pthread_mutex_init(&master->dongles[i].mutex, NULL);
		master->dongles[i].queue = malloc(sizeof(t_heap));
		master->dongles[i].queue->size = 0;
		master->dongles[i].queue->capacity = master -> number_of_coders;
		master->dongles[i].queue->data = malloc(master->number_of_coders * sizeof(t_coder *));
		master->coders[i].l_dongle = &master->dongles[(i - 1 + master->number_of_coders) % master->number_of_coders];
		master->coders[i].r_dongle = &master->dongles[(i + 1) % master->number_of_coders];
	}
}

static void	init_master(t_master *master)
{

	master->simulation_running = 1;
	master->start_time = get_time_ms();
	pthread_mutex_init(&master->log_mutex, NULL);
	pthread_mutex_init(&master->sim_mutex, NULL);
	master->coders = malloc(master->number_of_coders * sizeof(t_coder));
	master->dongles = malloc(master->number_of_coders * sizeof(t_dongle));
	init_coders_and_dongles(master);
}

static void	init_threads(t_master *master)
{
	int	i;

	i = -1;
	while (++i < master->number_of_coders)
		pthread_create(&master->coders[i].thread, NULL, coder_routine, &master->coders[i]);
	pthread_create(&master->monitor_thread, NULL, monitor_routine, master);
}

static void	join_threads(t_master *master)
{
	int	i;

	i = -1;
	while (++i < master->number_of_coders)
		pthread_join(master->coders[i].thread, NULL);
	pthread_join(master->monitor_thread, NULL);
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
	init_threads(&master);
	join_threads(&master);
	return (0);
}
