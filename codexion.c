/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macerver <macerver@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/03 13:45:02 by macerver          #+#    #+#             */
/*   Updated: 2026/06/04 17:44:34 by macerver         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int main (int argc, char **argv)
{
    t_master    master;
    if (argc != 9)
    {
        printf("Error\n");
    }
    if (parse_args(&master, argv) != 0)
    {
        printf("Error\n");
        return (1);
    }
    return (0);
}