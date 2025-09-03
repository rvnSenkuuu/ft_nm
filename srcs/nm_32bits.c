/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nm_32bits.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkara2 <tkara2@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 14:09:08 by tkara2            #+#    #+#             */
/*   Updated: 2025/09/03 18:22:54 by tkara2           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

char	get_symbol_type(Elf32_Sym *symbol, Elf32_Shdr *section_header, Elf32_Ehdr *header)
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

static bool	filter_symbols(Elf32_Sym *symbol, t_opt *options)
{
	Elf32_Sym	empty_symbol = {0};

	if (ft_memcmp(symbol, &empty_symbol, sizeof(*symbol)) == 0)
		return false;

	if (options->opt_a == false && ((ELF32_ST_TYPE(symbol->st_info) == STT_SECTION)
		|| (ELF32_ST_TYPE(symbol->st_info) == STT_FILE)))
		return false;

	if (options->opt_g == true && ELF32_ST_BIND(symbol->st_info) == STB_LOCAL)
		return false;

	if (options->opt_u == true && symbol->st_shndx != SHN_UNDEF) return false;

	return true;
}

static t_err	get_symbols(t_nm *nm, Elf32_Ehdr *header, Elf32_Shdr *section_header, Elf32_Shdr *current, t_symbols_info **symbols_output, size_t *symbol_count)
{
	Elf32_Shdr	*symtab_section = &section_header[current->sh_link];
	Elf32_Sym	*symbols = (Elf32_Sym *)(nm->file_map + current->sh_offset);
	
	int	count = 0;
	size_t	total_symbol_count = current->sh_size / sizeof(Elf32_Sym);
	char	*symtab_data = (char *)(nm->file_map + symtab_section->sh_offset);
			
	t_symbols_info	*symbol_arr = malloc(total_symbol_count * sizeof(**symbols_output));
	if (!symbol_arr) return MALLOC_ERR;

	for (size_t j = 0; j < total_symbol_count; j++) {
		Elf32_Sym	*symbol = &symbols[j];

		if (symbol->st_name >= symtab_section->sh_size) continue;
		if (filter_symbols(symbol, &(nm->options)) == false) continue;

		symbol_arr[count].type = get_symbol_type(symbol, section_header, header);
		symbol_arr[count].value = symbol->st_value;
		symbol_arr[count].name = symtab_data + symbol->st_name;
		symbol_arr[count].name_cpy = ft_strtrim(symtab_data + symbol->st_name, "_");
		if (!symbol_arr[count].name_cpy) {
			clean_sym_struct(symbol_arr, count + 1);
			return MALLOC_ERR;
		}
		str_to_lower(symbol_arr[count].name_cpy);
		count++;
	}
	*symbol_count = count;
	*symbols_output = symbol_arr;
	return NO_ERR;
}

t_err	ft_nm32(t_nm *nm)
{
	Elf32_Ehdr	*header = (Elf32_Ehdr *)nm->file_map;
	if (header->e_shoff >= (size_t)nm->file_stat.st_size)
		return ELF_FILE_OFFSET_ERR;

	Elf32_Shdr	*section_header = (Elf32_Shdr *)(nm->file_map + header->e_shoff);
	if (header->e_shstrndx >= header->e_shnum || section_header[header->e_shstrndx].sh_type != SHT_STRTAB)
		return ELF_FILE_SECTION_ERR;

	bool	has_symbol = false;
	t_err	ret = 0;
	size_t	symbol_count = 0;
	t_symbols_info	*symbols = NULL;

	for (int i = 0; i < header->e_shnum; i++) {
		Elf32_Shdr	*current_section = &section_header[i];

		if (current_section->sh_type == SHT_SYMTAB) {
			has_symbol = true;
			ret = get_symbols(nm, header, section_header, current_section, &symbols, &symbol_count);
			if (ret != NO_ERR) return ret;
			break;
		}
	}

	if (has_symbol == false) return NO_SYMBOL_ERR;	
	if (symbol_count > 1) {
		switch (get_sorting_type(&(nm->options))) {
			case NORMAL_SORT:
				ret = merge_sort(symbols, 0, symbol_count - 1, false);
				if (ret != NO_ERR) return ret;
				break;
			case REVERSE_SORT:
				ret = merge_sort(symbols, 0, symbol_count - 1, true);
				if (ret != NO_ERR) return ret;
				break;
			case NO_SORT: break;
			default: break;
		}
	}

	print_symbols(symbols, symbol_count, nm->is_64bits);
	clean_sym_struct(symbols, symbol_count);
	return NO_ERR;
}