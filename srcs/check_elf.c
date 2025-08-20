/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_elf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkara2 <tkara2@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 18:09:52 by tkara2            #+#    #+#             */
/*   Updated: 2025/08/20 13:49:04 by tkara2           ###   ########.fr       */
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

int	check_elf_file(Elf64_Ehdr *elf_header, struct stat *file_stat)
{
	if (file_stat->st_size < EI_NIDENT)
		return 1;

	if (check_elf_magic(elf_header->e_ident) != 0)
		return 1;

	if (elf_header->e_ident[EI_CLASS] == ELFCLASS64 && file_stat->st_size < (off_t)sizeof(Elf64_Ehdr))
			return 1;
	else if (elf_header->e_ident[EI_CLASS] == ELFCLASS32 && file_stat->st_size < (off_t)sizeof(Elf32_Ehdr))
		return 1;

	return 0;
}