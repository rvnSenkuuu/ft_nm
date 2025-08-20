/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkara2 <tkara2@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 14:39:15 by tkara2            #+#    #+#             */
/*   Updated: 2025/08/20 14:15:00 by tkara2           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

int	ft_nm(const char *file_name)
{
	int	ret = 0;
	t_nm	nm = {0};
	Elf64_Ehdr	*elf_header;
	
	ret = init_nm_struct(&nm, file_name);
	if (ret != 0) {
		clean_nm_struct(&nm);
		return 1;
	}

	elf_header = (Elf64_Ehdr *)nm.file_map;
	ret = check_elf_file(elf_header, &nm.file_stat);
	if (ret != 0) {
		ft_dprintf(STDERR_FILENO, "%s: %s: file format not recognized\n", nm.program_name, nm.file_name);
		clean_nm_struct(&nm);
		return 1;
	}

	if (elf_header->e_ident[EI_CLASS] == ELFCLASS64)
		ft_nm64(&nm);
	else
		ft_nm32(&nm);

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