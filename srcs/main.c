/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkara2 <tkara2@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 14:39:15 by tkara2            #+#    #+#             */
/*   Updated: 2025/08/27 17:46:09 by tkara2           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

int	ft_nm(const char *file_name)
{
	int	ret = 0;
	t_nm	nm = {0};
	Elf64_Ehdr	*elf_header;
	
	ret = init_nm_struct(&nm, file_name);
	if (ret != NO_ERR) {
		ft_dprintf(STDERR_FILENO, "nm: %s: %s\n", file_name, get_error_type(ret));
		clean_nm_struct(&nm);
		return NM_ERR;
	}

	elf_header = (Elf64_Ehdr *)nm.file_map;
	ret = check_elf_file(elf_header, &nm.file_stat);
	if (ret != NO_ERR) {
		ft_dprintf(STDERR_FILENO, "nm: %s: %s\n", file_name, get_error_type(ret));
		clean_nm_struct(&nm);
		return NM_ERR;
	}

	if (elf_header->e_ident[EI_CLASS] == ELFCLASS64) {
		ret = ft_nm64(&nm);
		if (ret != NO_ERR) {
			ft_dprintf(STDERR_FILENO, "nm: %s: %s\n", file_name, get_error_type(ret));
			clean_nm_struct(&nm);
			return NM_ERR;
		}
	} else {
		ret = ft_nm32(&nm);
		if (ret != NO_ERR) {
			ft_dprintf(STDERR_FILENO, "nm: %s: %s\n", file_name, get_error_type(ret));
			clean_nm_struct(&nm);
			return NM_ERR;
		}
	}

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
		if (ret != NO_ERR) continue;
	}

	return ret;
}