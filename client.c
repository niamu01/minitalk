/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yeju <yeju@student.42seoul.kr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/02 00:40:44 by yeju              #+#    #+#             */
/*   Updated: 2021/11/06 00:51:48 by yeju             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minitalk.h"

t_signal			g_signal;

void	client_move_bit(int n)
{
	(void)n;
	g_signal.word_bit >>= 1;
}

struct sigaction	client_action(char **argv)
{
	struct sigaction	action;

	action.sa_flags = 0;
	action.sa_handler = &client_move_bit;
	sigemptyset(&action.sa_mask);
	g_signal.pid = ft_atoi(argv[1]);
	return (action);
}

int	main(int argc, char **argv)
{
	struct sigaction	action;
	unsigned long int	len;

	if (argc < 3)
		return (-1);
	len = ft_strlen(argv[2]) + 1;
	action = client_action(argv);
	sigaction(SIGUSR1, &action, 0);
	while (len--)
	{
		g_signal.word_bit = 0b10000000;
		while (g_signal.word_bit)
		{
			if (*argv[2] & g_signal.word_bit)
				kill(g_signal.pid, SIGUSR1);
			else
				kill(g_signal.pid, SIGUSR2);
			pause();
			usleep(100);
		}
		++argv[2];
	}
	return (0);
}
