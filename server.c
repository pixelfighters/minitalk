/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kami <kami@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 10:06:50 by kami              #+#    #+#             */
/*   Updated: 2024/09/24 09:11:18 by kami             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minitalk.h"
#include "libft.h"

void	handle_sigusr(int sig, siginfo_t *info, void *msg)
{
	static int				i = -1;
	static int				pid_printed;
	static unsigned char	c;

	(void)msg;
	if (pid_printed != 1 && info != NULL)
	{
		ft_printf("Signal from PID %s%d%s\n\n", KCYN, info->si_pid, KNRM);
		pid_printed = 1;
	}
	if (i < 0)
		i = 7;
	if (sig == SIGUSR1)
		c |= (1 << i);
	i--;
	if (i < 0 && c)
	{
		ft_putchar_fd(c, STDOUT_FILENO);
		c = 0;
		if (kill(info->si_pid, SIGUSR2) == -1)
			ft_errhandle("cannot send SIGUSR2");
		return ;
	}
	if (kill(info->si_pid, SIGUSR1) == -1)
		ft_errhandle("cannot send SIGUSR1");
}

void	config_signals(void)
{
	struct sigaction	sa_newsig;

	sa_newsig.sa_sigaction = &handle_sigusr;
	sa_newsig.sa_flags = SA_SIGINFO;
	if (sigaction(SIGUSR1, &sa_newsig, NULL) == -1)
		ft_errhandle("Cannot change SIGUSR1");
	if (sigaction(SIGUSR2, &sa_newsig, NULL) == -1)
		ft_errhandle("Cannot change SIGUSR2");
}

int	main(void)
{
	pid_t	pid;

	pid = getpid();
	ft_print_ascii_art("TALK SERVER", KCYN, 1);
	ft_printf("SERVER PID = %s%d%s\n\n", KCYN, pid, KNRM);
	while (1)
	{
		config_signals();
	}
	return (EXIT_SUCCESS);
}
