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

static int  is_valid_int(char *str)
{
    int i;

    i = 0;
    while (str[i])
    {
        if (str[i] < '0' || str[i] > '9' )
        return (0);
        i++;
    }
    return (1);
}

int parse_args(t_master *master, char **argv)
{
    if (is_valid_int(argv[1]) == 0)
        return (1);
    else
        master->number_of_coders = atoi(argv[1]);
}