/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkara2 <tkara2@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 14:39:15 by tkara2            #+#    #+#             */
/*   Updated: 2025/08/19 16:39:46 by tkara2           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

int	ft_nm(const char *file_name)
{
	int	ret = 0;
	t_nm	nm = {0};
	
	ret = init_nm_struct(&nm, file_name);
	if (ret == 1) {
		clean_nm_struct(&nm);
		return 1;
	}

	//Check elf file

	clean_nm_struct(&nm);
	return ret;
}

int	main(int argc, char **argv)
{
	int	ret = 0;

	if (argc == 1) {
		ret = ft_nm(DEFAULT_FILE);
		return ret;
	}

	for (int i = 1; i < argc; i++) {
		ret = ft_nm(argv[i]);
		if (ret != 0) continue;
	}

	return ret;
}