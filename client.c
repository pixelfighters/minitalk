/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kami <kami@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 10:06:50 by kami              #+#    #+#             */
/*   Updated: 2024/06/11 16:56:05 by kami             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minitalk.h"

int	check_args(int argc, char **argv)
{
	int	i;

	i = 0;
	if (argc != 3)
	{
		ft_errhandle("Invalid input.\nPlease enter <PID> and Message.");
		return (0);
	}
	while (argv[1][i])
	{
		if (!ft_isdigit(argv[1][i]))
		{
			ft_errhandle("Invalid PID.\nPlease enter valid <PID>.");
			return (0);
		}
		i++;
	}
	if (argv[2] == NULL)
	{
		ft_errhandle("Invalid Message.\nPlease enter valid Message.");
		return (0);
	}
	return (1);
}

void	send_msg(pid_t sv_pid, char *msg)
{
	unsigned char	c;
	int				nbr_bits;

	while (*msg)
	{
		c = *msg;
		nbr_bits = 8;
		while (nbr_bits--)
		{
			if (c & 0b10000000)
				kill(sv_pid, SIGUSR1);
			else
				kill(sv_pid, SIGUSR2);
			usleep(50);
			c <<= 1;
		}
		msg++;
	}
}

void	signal_actions(int signum)
{
	ssize_t	bytes_written;

	if (signum == SIGUSR2)
	{
		bytes_written = ft_printf("%sCharacter received!%s\n", KGRN, KNRM);
		if (bytes_written == -1)
		{
			ft_errhandle("Error writing to stdout");
		}
	}
}

void	config_signals(void)
{
	struct sigaction	sa_newsig;

	ft_memset(&sa_newsig, 0, sizeof(sa_newsig));
	sa_newsig.sa_handler = signal_actions;
	if (sigaction(SIGUSR1, &sa_newsig, NULL) == -1)
		ft_errhandle("Failed to change SIGUSR1's behavior");
	if (sigaction(SIGUSR2, &sa_newsig, NULL) == -1)
		ft_errhandle("Failed to change SIGUSR2's behavior");
}

int	main(int argc, char **argv)
{
	pid_t		server_pid;

	if (check_args(argc, argv))
	{
		ft_print_ascii_art("TALK CLIENT", KCYN, 1);
		ft_printf("\nSent %s%s%s to PID %s%s%s\n", KYEL, \
				argv[2], KNRM, KCYN, argv[1], KNRM);
		server_pid = ft_atoi(argv[1]);
		config_signals();
		send_msg(server_pid, argv[2]);
		while (1)
			pause();
		return (EXIT_SUCCESS);
	}
}
