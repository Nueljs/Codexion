/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_args.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macerver <macerver@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/04 16:56:40 by macerver          #+#    #+#             */
/*   Updated: 2026/06/04 17:44:29 by macerver         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	is_valid_int(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9' )
			return (0);
		i++;
	}
	return (1);
}

int	parse_args(t_master *master, char **argv)
{
	if (is_valid_int(argv[1]) == 0)
		return (1);
	if (is_valid_int(argv[2]) == 0)
		return (1);
	if (is_valid_int(argv[3]) == 0)
		return (1);
	if (is_valid_int(argv[4]) == 0)
		return (1);
	if (is_valid_int(argv[5]) == 0)
		return (1);
	if (is_valid_int(argv[6]) == 0)
		return (1);
	if (is_valid_int(argv[7]) == 0)
		return (1);
	if (strcmp(argv[8], "fifo") != 0 && strcmp(argv[8], "edf") != 0)
		return (1);
	master->number_of_coders = atoi(argv[1]);
	master->time_to_burnout = (long)atoi(argv[2]);
	master->time_to_compile = (long)atoi(argv[3]);
	master->time_to_debug = (long)atoi(argv[4]);
	master->time_to_refactor = (long)atoi(argv[5]);
	master->number_of_compiles_required = atoi(argv[6]);
	master->dongle_cooldown = (long)atoi(argv[7]);
	master->scheduler = argv[8];
	return (0);
}
