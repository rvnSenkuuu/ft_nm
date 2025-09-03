/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_elf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkara2 <tkara2@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 18:09:52 by tkara2            #+#    #+#             */
/*   Updated: 2025/09/03 11:58:30 by tkara2           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

static int check_elf_magic(unsigned char *e_ident)
{
	if (ft_memcmp(e_ident, ELFMAG, SELFMAG) != 0)
		return ELF_FILE_FORMAT_ERR;
	
	if (e_ident[EI_CLASS] != ELFCLASS32 && e_ident[EI_CLASS] != ELFCLASS64)
		return ELF_FILE_FORMAT_ERR;
		
	if (e_ident[EI_DATA] != ELFDATA2LSB && e_ident[EI_DATA] != ELFDATA2MSB)
		return ELF_FILE_FORMAT_ERR;
	
	if (e_ident[EI_VERSION] != EV_CURRENT)
		return ELF_FILE_FORMAT_ERR;

	return NO_ERR;
}

int	check_elf_file(Elf64_Ehdr *elf_header, struct stat *file_stat)
{
	if (file_stat->st_size < EI_NIDENT)
		return STAT_FILE_ERR;

	if (check_elf_magic(elf_header->e_ident) != 0)
		return ELF_FILE_FORMAT_ERR;

	if (elf_header->e_ident[EI_CLASS] == ELFCLASS64 && file_stat->st_size < (off_t)sizeof(Elf64_Ehdr))
		return ELF_FILE_FORMAT_ERR;
	else if (elf_header->e_ident[EI_CLASS] == ELFCLASS32 && file_stat->st_size < (off_t)sizeof(Elf32_Ehdr))
		return ELF_FILE_FORMAT_ERR;

	return NO_ERR;
}