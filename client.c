/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fschuh <fschuh@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 10:06:50 by kami              #+#    #+#             */
/*   Updated: 2025/02/26 12:51:56 by fschuh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minitalk.h"

void	signal_error(void)
{
	ft_print_ascii_art("ERROR", KRED, 1);
	ft_errhandle("Unexpected SIG Error.\n");
	exit(EXIT_FAILURE);
}

void	convert(unsigned char c, int pid)
{
	int	bit;

	bit = 0;
	while (bit < 8)
	{
		if (c & 128)
		{
			if (kill(pid, SIGUSR2) == -1)
				signal_error();
		}
		else
		{
			if (kill(pid, SIGUSR1) == -1)
				signal_error();
		}
		c <<= 1;
		bit++;
		pause();
		usleep(10);
	}
}

void	sent_text(char *str, int pid)
{
	int	i;

	i = 0;
	while (str[i])
		convert(str[i++], pid);
	convert('\0', pid);
}

void	success(int sig)
{
	static int	sent;

	if (sig == SIGUSR1)
	{
		ft_printf("%s%d signals%s sent successfully!%s\n",
			KGRN, ++sent, KNRM, KNRM);
		exit(EXIT_SUCCESS);
	}
	if (sig == SIGUSR2)
		++sent;
}

int	main(int argc, char **argv)
{
	pid_t	server_pid;

	if (argc == 3 && (ft_atoi(argv[1]) > 0))
	{
		ft_print_ascii_art("TALK CLIENT", KCYN, 1);
		ft_printf("Sendig %s%s%s to PID: %s%s%s\n",
			KYEL, argv[2], KNRM, KCYN, argv[1], KNRM);
		signal(SIGUSR1, success);
		signal(SIGUSR2, success);
		server_pid = ft_atoi(argv[1]);
		ft_printf("%sSending to Server%s\n", KYEL, KNRM);
		sent_text(argv[2], server_pid);
		ft_printf("%s", KNRM);
		return (EXIT_SUCCESS);
	}
	else
	{
		ft_print_ascii_art("Failure", KRED, 1);
		return (EXIT_FAILURE);
	}
}
