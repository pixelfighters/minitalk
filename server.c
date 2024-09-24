/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kami <kami@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 10:06:50 by kami              #+#    #+#             */
/*   Updated: 2024/09/24 12:10:38 by kami             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minitalk.h"
#include "libft.h"

static int handler_configured = 0;

void	handle_sigusr(int sig, siginfo_t *info, void *msg)
{
	static int				i = -1;
	static int				pid_printed;
	static unsigned char	c;

	(void)msg;
	if (pid_printed != 1 && info != NULL && handler_configured <= 1)
	{
		ft_printf("Signal from PID %s%d%s\n\n", KCYN, info->si_pid, KNRM);
		pid_printed = 1;
	}
	if (i < 0)
		i = 7;
	if (sig == SIGUSR1)
		c |= (1 << i);
	i--;
    if (i < 0)
    {
        if (c == '\0')
        {
            ft_printf("\nMessage complete.\n\n");
            if (kill(info->si_pid, SIGUSR2) == -1)
                ft_errhandle("cannot send SIGUSR2");
            c = 0;
            return;
        }
        ft_putchar_fd(c, STDOUT_FILENO);
        c = 0;
        if (kill(info->si_pid, SIGUSR1) == -1)
            ft_errhandle("cannot send SIGUSR1");
        i = 7; 
    }
}

void	config_signals(void)
{
	struct sigaction	sa_newsig;

	if (handler_configured)
        return;
	sa_newsig.sa_sigaction = &handle_sigusr;
	sa_newsig.sa_flags = SA_SIGINFO;
	sigemptyset(&sa_newsig.sa_mask);
	if (sigaction(SIGUSR1, &sa_newsig, NULL) == -1)
		ft_errhandle("Cannot change SIGUSR1");
	if (sigaction(SIGUSR2, &sa_newsig, NULL) == -1)
		ft_errhandle("Cannot change SIGUSR2");
	handler_configured = 1;
}

int	main(void)
{
	pid_t	pid;

	pid = getpid();
	ft_print_ascii_art("TALK SERVER", KCYN, 1);
	ft_printf("SERVER PID = %s%d%s\n\n", KCYN, pid, KNRM);
    config_signals();
    while (1)
    {
        pause();
    }
	return (0);
}
