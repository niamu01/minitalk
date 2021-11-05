/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils1.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yeju <yeju@student.42seoul.kr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/02 00:41:21 by yeju              #+#    #+#             */
/*   Updated: 2021/11/05 23:07:26 by yeju             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>

int	ft_strlen(char *s)
{
	int	i;

	i = 0;
	while (s[i] != '\0')
		++i;
	return (i);
}

void	ft_putstr(char *s)
{
	if (s == NULL)
		return ;
	write(1, s, ft_strlen(s));
}

void	ft_putchar(char c)
{
	write(1, &c, 1);
}

void	ft_putnbr(int n)
{
	unsigned int	i;

	if (n < 0)
	{
		ft_putchar('-');
		i = -(unsigned int)n;
	}
	else
		i = (unsigned int)n;
	if (n >= 10)
	{
		ft_putnbr((int)(i / 10));
		ft_putnbr((int)(i % 10));
	}
	else
		ft_putchar((char)(i % 10 + '0'));
}
