/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekhaled <ekhaled@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 00:04:04 by ekhaled           #+#    #+#             */
/*   Updated: 2023/11/06 00:04:14 by ekhaled          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

bool	is_valid_arg_nb(int argc)
{
	if (argc < 5)
	{
		write(2, "Error : Not enough arguments given\n", 35);
		return (0);
	}
	if (argc > 6)
	{
		write(2, "Error : Too many arguments given\n", 33);
		return (0);
	}
	return (1);
}

bool	are_uints(int argc, char **argv)
{
	int	arg_index;
	int	i;

	arg_index = 1;
	while (arg_index < argc)
	{
		i = 0;
		if (!argv[arg_index][i])
		{
			write(2, "Error : Not all arguments are integers\n", 39);
			return (0);
		}
		while (argv[arg_index][i])
		{
			if (!ft_isdigit(argv[arg_index][i]))
			{
				write(2, "Error : Not all arguments are integers\n", 39);
				return (0);
			}
			i++;
		}
		arg_index++;
	}
	return (1);
}

bool	are_valid_uints(int argc, char **argv)
{
	int	arg_index;
	int	arg_len;

	arg_index = 1;
	arg_len = ft_strlen(argv[arg_index]);
	if (argv[arg_index][0] == '0' || arg_len > 10
		|| (arg_len == 10 && ft_strcmp(argv[arg_index], "4294967295") > 0))
	{
		write(2, "Error : Not all arguments are valid integers\n", 45);
		return (0);
	}
	arg_index++;
	while (arg_index < argc)
	{
		arg_len = ft_strlen(argv[arg_index]);
		if ((arg_len > 1 && argv[arg_index][0] == '0') || arg_len > 10
			|| (arg_len == 10 && ft_strcmp(argv[arg_index], "4294967295") > 0))
		{
			write(2, "Error : Not all arguments are valid integers\n", 45);
			return (0);
		}
		arg_index++;
	}
	return (1);
}

bool	are_valid_params(int argc, char **argv)
{
	if (!is_valid_arg_nb(argc))
		return (0);
	if (!are_uints(argc, argv))
		return (0);
	if (!are_valid_uints(argc, argv))
		return (0);
	return (1);
}
