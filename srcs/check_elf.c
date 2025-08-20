/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_elf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkara2 <tkara2@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 18:09:52 by tkara2            #+#    #+#             */
/*   Updated: 2025/08/20 11:38:57 by tkara2           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

static int check_elf_magic(unsigned char *e_ident)
{
	if (ft_memcmp(e_ident, ELFMAG, SELFMAG) != 0)
		return 1;
	
	if (e_ident[EI_CLASS] != ELFCLASS32 && e_ident[EI_CLASS] != ELFCLASS64)
		return 1;
		
	if (e_ident[EI_DATA] != ELFDATA2LSB && e_ident[EI_DATA] != ELFDATA2MSB)
		return 1;
	
	if (e_ident[EI_VERSION] != EV_CURRENT)
		return 1;

	return 0;
}

int	check_elf_file(t_nm *nm)
{
	Elf64_Ehdr	*elf_header = (Elf64_Ehdr *)nm->file_map;

	if (nm->file_stat.st_size < EI_NIDENT)
		goto file_format_error;

	if (check_elf_magic(elf_header->e_ident) != 0)
		goto file_format_error;

	if (elf_header->e_ident[EI_CLASS] == ELFCLASS64) {
		if (nm->file_stat.st_size < (off_t)sizeof(Elf64_Ehdr))
			goto file_format_error;
	} else if (elf_header->e_ident[EI_CLASS] == ELFCLASS32) {
    	if (nm->file_stat.st_size < (off_t)sizeof(Elf32_Ehdr))
			goto file_format_error;
	}

	return 0;

file_format_error:
	ft_dprintf(STDERR_FILENO, "%s: %s: file format not recognized\n", nm->program_name, nm->file_name);
	return 1;
}