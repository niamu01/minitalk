/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yeju <yeju@student.42seoul.kr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/02 00:41:10 by yeju              #+#    #+#             */
/*   Updated: 2021/11/06 00:51:57 by yeju             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minitalk.h"

t_signal			g_signal;

void	server_sa_sigaction(int sig, siginfo_t *act, void *oact)
{
	(void)oact;
	if (sig == SIGUSR1)
		g_signal.word += g_signal.word_bit;
	g_signal.word_bit >>= 1;
	if (!g_signal.word_bit)
	{
		if (!g_signal.word)
			ft_putchar('\n');
		else
			ft_putchar(g_signal.word);
		g_signal.word_bit = 0b10000000;
		g_signal.word = 0;
	}
	usleep(100);
	kill(act->si_pid, SIGUSR1);
}

int	main(void)
{
	struct sigaction	action;

	g_signal.word = 0;
	g_signal.word_bit = 0b10000000;
	action.sa_flags = SA_SIGINFO;
	action.sa_sigaction = &server_sa_sigaction;
	sigemptyset(&action.sa_mask);
	sigaction(SIGUSR1, &action, 0);
	sigaction(SIGUSR2, &action, 0);
	ft_putstr("SERVER PID IS: < ");
	ft_putnbr(getpid());
	ft_putstr(" >\n");
	while (1)
		pause();
}
