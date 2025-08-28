/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   options.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkara2 <tkara2@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 19:14:38 by tkara2            #+#    #+#             */
/*   Updated: 2025/08/28 19:15:04 by tkara2           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

static void	print_usage(void)
{
	printf("NM OPTIONS: \n");
	printf("\t-a\n\t--debug-syms\n\t\tDisplay all symbols, even debugger-only symbols; normally these are not listed.\n");
	printf("\t-g\n\t--extern-only\n\t\tDisplay only external symbols.\n");
	printf("\t-u\n\t--undefined-only\n\t\tDisplay only undefined symbols (those external to each object file).\n");
	printf("\t-r\n\t--reverse-sort\n\t\tReverse the order of the sort (whether numeric or alphabetic); let the last come first.\n");
	printf("\t-p\n\t--no-sort\n\t\tDo not bother to sort the symbols in any order; print them in the order encountered.\n");
	printf("\t-h\n\t--help\n\t\tShow a summary of the options to nm and exit.\n");
	exit(0);
}

void	parse_opt(t_opt *options, int *starting_index, char **argv)
{
	int	i = 0;
	for (; argv[i]; i++) {
		if (ft_strncmp(argv[i], "-h", ft_strlen("-h")) == 0 
			|| ft_strncmp(argv[i], "--help", ft_strlen("--help")) == 0)
			print_usage();
		else if (ft_strncmp(argv[i], "-a", ft_strlen("-a")) == 0 
			|| ft_strncmp(argv[i], "--debug-syms", ft_strlen("--debug-syms")) == 0)
			options->opt_a = true;
		else if (ft_strncmp(argv[i], "-g", ft_strlen("-g")) == 0 
			|| ft_strncmp(argv[i], "--extern-only", ft_strlen("--extern-only")) == 0)
			options->opt_g = true;
		else if (ft_strncmp(argv[i], "-u", ft_strlen("-u")) == 0 
			|| ft_strncmp(argv[i], "--undefined-only", ft_strlen("--undefined-only")) == 0)
			options->opt_u = true;
		else if (ft_strncmp(argv[i], "-r", ft_strlen("-r")) == 0 
			|| ft_strncmp(argv[i], "--reverse-sort", ft_strlen("--reverse-sort")) == 0)
			options->opt_r = true;
		else if (ft_strncmp(argv[i], "-p", ft_strlen("-p")) == 0 
			|| ft_strncmp(argv[i], "--no-sort", ft_strlen("--no-sort")) == 0)
			options->opt_p = true;
	}
	*starting_index = i;
}