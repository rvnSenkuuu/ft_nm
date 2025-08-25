/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nm_32_64.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkara2 <tkara2@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 14:09:08 by tkara2            #+#    #+#             */
/*   Updated: 2025/08/25 18:31:36 by tkara2           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

char	get_symbol_type(Elf64_Sym *symbol, Elf64_Shdr *section_header, Elf64_Ehdr *header)
{
	unsigned char	type = ELF64_ST_TYPE(symbol->st_info);
	unsigned char	bind = ELF64_ST_BIND(symbol->st_info);
	
	if (type == STT_GNU_IFUNC) return 'i';
	if (bind == STB_GNU_UNIQUE) return 'u';
	if (symbol->st_shndx == SHN_ABS) return 'A';
	if (symbol->st_shndx == SHN_COMMON) return 'C';
	
	if (symbol->st_shndx == SHN_UNDEF) {
		if (bind == STB_WEAK) {
			if (type == STT_OBJECT) return 'v';
			else return 'w';
		}
		else return 'U';
	}
	
	if (bind == STB_WEAK) {
		if (type == STT_OBJECT) return 'V';
		else return 'W';
	}
	
	if (type == STT_FUNC) return (bind == STB_GLOBAL) ? 'T' : 't';
	
	if (symbol->st_shndx >= header->e_shnum) return '?';
	Elf64_Shdr	section = section_header[symbol->st_shndx];

	if (type == STT_OBJECT || type == STT_NOTYPE) {
		if (section.sh_type == SHT_NOBITS) return (bind == STB_GLOBAL) ? 'B' : 'b';
		if (section.sh_flags & SHF_WRITE) return (bind == STB_GLOBAL) ? 'D' : 'd';
		if (section.sh_flags & SHF_ALLOC) return (bind == STB_GLOBAL) ? 'R' : 'r';
	}

	if (type == STT_SECTION) return 's';

	return '?';
}

void	ft_nm64(t_nm *nm)
{
	Elf64_Ehdr	*header = (Elf64_Ehdr *)nm->file_map;
	Elf64_Shdr	*section_header = (Elf64_Shdr *)(nm->file_map + header->e_shoff);

	for (int i = 0; i < header->e_shnum; i++) {
		Elf64_Shdr	*current_section = &section_header[i];
		if (current_section->sh_type != SHT_SYMTAB && current_section->sh_type != SHT_DYNSYM)
			continue;
		
		Elf64_Shdr	*strtab_section = &section_header[current_section->sh_link];
		char	*symtab_data = (char *)(nm->file_map + strtab_section->sh_offset);
		int	symbol_count = current_section->sh_size / sizeof(Elf64_Sym);

		for (int j = 0; j < symbol_count; j++) {
			Elf64_Sym	*symbol = (Elf64_Sym *)(nm->file_map + current_section->sh_offset + j * sizeof(Elf64_Sym));
			if (ELF64_ST_TYPE(symbol->st_info) == STT_FILE || symbol->st_name == 0)
				continue;

			char	*symbol_name = symtab_data + symbol->st_name;
			char	symbol_type = get_symbol_type(symbol, section_header, header);
			printf("%016lu\t", symbol->st_value);
			printf("%c\t", symbol_type);
			printf("%s\n", symbol_name);
		}
	}
}

void	ft_nm32(t_nm *nm)
{
	(void)nm;
	ft_dprintf(STDOUT_FILENO, "Todo\n");
}