/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macerver <macerver@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/02 11:53:17 by macerver          #+#    #+#             */
/*   Updated: 2026/06/05 17:11:11 by macerver         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H
# include <pthread.h>
# include <stdio.h>
# include <sys/time.h>

//////////////
//STRUCTS
//////////////

typedef struct s_master	t_master;
typedef struct s_coder	t_coder;
typedef struct s_dongle	t_dongle;
typedef struct s_heap	t_heap;

//////////////
//CODER-STRUCT
//////////////

typedef struct s_coder
{
	int				id;
	long			last_compile_start;
	int				times_compiled;
	struct s_dongle	*l_dongle;
	struct s_dongle	*r_dongle;
	struct s_master	*master;
}	t_coder;

//////////////
//MASTER-STRUCT
//////////////

typedef struct s_master
{
	int				number_of_coders;
	long			time_to_burnout;
	long			time_to_compile;
	long			time_to_debug;
	long			time_to_refactor;
	int				number_of_compiles_required;
	long			dongle_cooldown;
	char			*scheduler;
	int				simulation_running;
	long			start_time;
	pthread_mutex_t	log_mutex;
	pthread_mutex_t	sim_mutex;
	t_coder			*coders;
	t_dongle		*dongles;
	pthread_t		monitor_thread;
}	t_master;

//////////////
//DONGLE-STRUCT
//////////////

typedef struct s_dongle
{
	int				id;
	long			release_time;
	int				is_taken;
	pthread_mutex_t	mutex;
	pthread_cond_t	cond;
	t_heap			*queue;
}	t_dongle;

//////////////
//HEAP-STRUCT
//////////////

typedef struct s_heap
{
	int		capacity;
	int		size;
	t_coder	**coders;
}	t_heap;

//////////////
//FUNCTIONS
//////////////

int	parse_args(t_master *master, char **argv);

#endif