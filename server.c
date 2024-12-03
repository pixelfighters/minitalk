/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kami <kami@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 10:06:50 by kami              #+#    #+#             */
/*   Updated: 2024/12/03 20:41:19 by kami             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minitalk.h"
#include "libft.h"

void	signal_error(void)
{
	ft_print_ascii_art("ERROR", KRED, 1);
	ft_errhandle("Unexpected SIG Error.\n");
	exit(EXIT_FAILURE);
}

void	extended_action(char *c, int *received, int *client_pid, int *bit)
{
	ft_printf("%c", *c);
	if (*c == '\0')
	{
		ft_printf("\n%s%d signals%s recieved from client PID: %d%s\n",
			KGRN, *received, KNRM, *client_pid, KNRM);
		*received = 0;
		*c = 0;
		if (kill(*client_pid, SIGUSR1) == -1)
			signal_error();
		return ;
	}
	*bit = 0;
}

void	action(int sig, siginfo_t *info, void *context)
{
	static pid_t	client_pid;
	static int		bit;
	static char		c;
	static int		received;
	static pid_t	current_pid;

	(void)context;
	if (!client_pid)
		client_pid = info->si_pid;
	current_pid = info->si_pid;
	if (client_pid != current_pid)
	{
		client_pid = current_pid;
		bit = 0;
		c = 0;
		received = 0;
	}
	c |= (sig == SIGUSR2);
	received++;
	bit++;
	if (bit == 8)
		extended_action(&c, &received, &client_pid, &bit);
	c <<= 1;
	usleep(100);
	kill(client_pid, SIGUSR2);
}

int	main(void)
{
	pid_t				pid;
	struct sigaction	act;

	pid = getpid();
	act.sa_sigaction = action;
	sigemptyset(&act.sa_mask);
	act.sa_flags = SA_SIGINFO;
	ft_print_ascii_art("TALK SERVER", KCYN, 1);
	ft_printf("SERVER PID = %s%d%s\n\n", KCYN, pid, KNRM);
	while (1)
	{
		sigaction(SIGUSR1, &act, 0);
		sigaction(SIGUSR2, &act, 0);
		pause();
	}
	return (EXIT_FAILURE);
}
