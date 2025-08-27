/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nm_32bits.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkara2 <tkara2@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 14:09:08 by tkara2            #+#    #+#             */
/*   Updated: 2025/08/27 17:53:00 by tkara2           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

char	get_symbol_type_32bits(Elf32_Sym *symbol, Elf32_Shdr *section_header, Elf32_Ehdr *header)
{
	unsigned char	type = ELF32_ST_TYPE(symbol->st_info);
	unsigned char	bind = ELF32_ST_BIND(symbol->st_info);
	
	if (type == STT_GNU_IFUNC) return 'i';
	if (bind == STB_GNU_UNIQUE) return 'u';
	if (symbol->st_shndx == SHN_ABS) return (bind == STB_GLOBAL) ? 'A' : 'a';
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
	Elf32_Shdr	section = section_header[symbol->st_shndx];

	if (type == STT_OBJECT || type == STT_NOTYPE) {
		if (section.sh_type == SHT_NOBITS) return (bind == STB_GLOBAL) ? 'B' : 'b';
		if (section.sh_flags & SHF_EXECINSTR) return (bind == STB_GLOBAL) ? 'T' : 't';
		if (section.sh_flags & SHF_WRITE) return (bind == STB_GLOBAL) ? 'D' : 'd';
		if (section.sh_flags & SHF_ALLOC) return (bind == STB_GLOBAL) ? 'R' : 'r';
	}

	if (type == STT_SECTION) return 's';

	return '?';
}

void	handle_symbols_32bits(t_sym_arr *sym_arr, size_t sym_count)
{
	for (size_t i = 0; i < sym_count - 1; i++) {
		for (size_t j = 0; j < sym_count - i - 1; j++) {
			if (ft_strncmp(sym_arr[j].name_cpy, sym_arr[j + 1].name_cpy, ft_strlen(sym_arr[j].name_cpy)) > 0)
				swap_symbols(&sym_arr[j], &sym_arr[j + 1]);
		}
	}

	for (size_t i = 0; i < sym_count; i++) {
		if (sym_arr[i].value == 0)
			printf("          ");
		else
			printf("%08x ", (unsigned int)sym_arr[i].value);
		printf("%c %s\n", sym_arr[i].type, sym_arr[i].name);
	}
}

t_err	ft_nm32(t_nm *nm)
{
	bool	has_sym = false;
	size_t	sym_arr_counter = 0;
	t_sym_arr	*sym_arr = NULL;
	Elf32_Ehdr	*header = (Elf32_Ehdr *)nm->file_map;
	Elf32_Shdr	*section_header = (Elf32_Shdr *)(nm->file_map + header->e_shoff);

	for (int i = 0; i < header->e_shnum; i++) {
		Elf32_Shdr	*current_section = &section_header[i];

		if (current_section->sh_type == SHT_SYMTAB) {
			has_sym = true;	
			Elf32_Shdr	*strtab_section = &section_header[current_section->sh_link];
			Elf32_Sym	*symbols = (Elf32_Sym *)(nm->file_map + current_section->sh_offset);
			char	*symtab_data = (char *)(nm->file_map + strtab_section->sh_offset);
			int	symbol_count = current_section->sh_size / sizeof(Elf32_Sym);
			
			sym_arr = malloc(symbol_count * sizeof(*sym_arr));
			if (!sym_arr) return MALLOC_ERR;

			for (int j = 0; j < symbol_count; j++) {
				Elf32_Sym	*symbol = &symbols[j];
				if (ELF32_ST_TYPE(symbol->st_info) == STT_FILE || symbol->st_name == 0)
					continue;

				sym_arr[sym_arr_counter].type = get_symbol_type_32bits(symbol, section_header, header);
				sym_arr[sym_arr_counter].value = symbol->st_value;
				sym_arr[sym_arr_counter].name = symtab_data + symbol->st_name;
				sym_arr[sym_arr_counter].name_cpy = ft_strtrim(symtab_data + symbol->st_name, "_");
				str_to_lower(sym_arr[sym_arr_counter].name_cpy);
				sym_arr_counter++;
			}
		}
	}
	if (has_sym == false) return NO_SYMBOL_ERR;
	
	handle_symbols_32bits(sym_arr, sym_arr_counter);
	clean_sym_struct(sym_arr, sym_arr_counter);
	return NO_ERR;
}