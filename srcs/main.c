/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkara2 <tkara2@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 14:39:15 by tkara2            #+#    #+#             */
/*   Updated: 2025/08/29 13:34:35 by tkara2           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

int	ft_nm(const char *file_name, t_opt *options)
{
	int	ret = 0;
	t_nm	nm = {0};
	Elf64_Ehdr	*elf_header;
	
	ret = init_nm_struct(&nm, file_name, options);
	if (ret != NO_ERR) goto err_exit;

	elf_header = (Elf64_Ehdr *)nm.file_map;
	ret = check_elf_file(elf_header, &nm.file_stat);
	if (ret != NO_ERR) goto err_exit;

	if (elf_header->e_ident[EI_CLASS] == ELFCLASS64) {
		ret = ft_nm64(&nm);
		if (ret != NO_ERR) goto err_exit;
	} else {
		ret = ft_nm32(&nm);
		if (ret != NO_ERR) goto err_exit;
	}

	clean_nm_struct(&nm);
	return ret;

err_exit:
	ft_dprintf(STDERR_FILENO, "nm: %s: %s\n", file_name, get_error_type(ret));
	clean_nm_struct(&nm);
	return NM_ERR;
}

int	main(int argc, char **argv)
{
	int	starting_index = 1;
	int	ret = 0;
	t_opt	options = {0};
	
	parse_opt(&options, &starting_index, argv + 1);
	if (argc == starting_index) {
		ret = ft_nm(DEFAULT_FILE, &options);
		return ret;
	}

	for (int i = starting_index; i < argc; i++) {
		ret = ft_nm(argv[i], &options);
		if (ret != NO_ERR) {
			write(STDOUT_FILENO, "\n", sizeof(char));		
			continue; 
		}
	}

	return ret;
}