/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minitalk.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yeju <yeju@student.42seoul.kr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/02 00:40:58 by yeju              #+#    #+#             */
/*   Updated: 2021/11/05 23:24:30 by yeju             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINITALK_H
# define MINITALK_H

# include <unistd.h>
# include <signal.h>

int					ft_strlen(char *s);
int					ft_atoi(char *str);
void				ft_putstr(char *s);
void				ft_putchar(char c);
void				ft_putnbr(int n);

void				server_sa_sigaction(int sig, siginfo_t *act, void *oact);
struct sigaction	client_action(unsigned long int *len, char **argv);
void				client_move_bit(int n);

typedef struct s_signal
{
	unsigned int	word_bit;
	char			word;
	int				pid;
}					t_signal;

t_signal			g_signal;

#endif