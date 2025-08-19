/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_elf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkara2 <tkara2@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 18:09:52 by tkara2            #+#    #+#             */
/*   Updated: 2025/08/19 19:14:58 by tkara2           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

static int check_elf_magic(unsigned char *e_ident)
{
	if (e_ident[EI_MAG0] == 0x7F 
		&& e_ident[EI_MAG1] == 'e'
		&& e_ident[EI_MAG2] == 'l'
		&& e_ident[EI_MAG3] == 'f')
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

	if (nm->file_stat.st_size < ELF_HEADER_MAGIC_SIZE) {
		ft_dprintf(STDERR_FILENO, "%s: %s: file format not recognized\n", nm->program_name, nm->file_name);
		return 1;
	}

	if (check_elf_magic(elf_header->e_ident) != 0) {
		ft_dprintf(STDERR_FILENO, "%s: %s: file format not recognized\n", nm->program_name, nm->file_name);
		return 1;
	}

	if (elf_header->e_ident[EI_CLASS] == ELFCLASS64) {
		if (nm->file_stat.st_size < (off_t)sizeof(Elf64_Ehdr)) {
			ft_dprintf(STDERR_FILENO, "%s: %s: file format not recognized\n", nm->program_name, nm->file_name);
			return 1;
		}
	} else if (elf_header->e_ident[EI_CLASS] == ELFCLASS32) {
    	if (nm->file_stat.st_size < (off_t)sizeof(Elf32_Ehdr)) {
			ft_dprintf(STDERR_FILENO, "%s: %s: file format not recognized\n", nm->program_name, nm->file_name);
        	return 1;
    	}
	}

	return 0;
}