/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nm_64bits.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkara2 <tkara2@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 14:09:08 by tkara2            #+#    #+#             */
/*   Updated: 2025/08/28 16:08:48 by tkara2           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

static char	get_symbol_type(Elf64_Sym *symbol, Elf64_Shdr *section_header, Elf64_Ehdr *header)
{
	unsigned char	type = ELF64_ST_TYPE(symbol->st_info);
	unsigned char	bind = ELF64_ST_BIND(symbol->st_info);
	
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
	Elf64_Shdr	section = section_header[symbol->st_shndx];

	if (type == STT_OBJECT || type == STT_NOTYPE) {
		if (section.sh_type == SHT_NOBITS) return (bind == STB_GLOBAL) ? 'B' : 'b';
		if (section.sh_flags & SHF_EXECINSTR) return (bind == STB_GLOBAL) ? 'T' : 't';
		if (section.sh_flags & SHF_WRITE) return (bind == STB_GLOBAL) ? 'D' : 'd';
		if (section.sh_flags & SHF_ALLOC) return (bind == STB_GLOBAL) ? 'R' : 'r';
	}

	if (type == STT_SECTION) return 's';

	return '?';
}

static void	sort_symbols(t_symbols_info *symbols, size_t symbols_count)
{
	for (size_t i = 0; i < symbols_count - 1; i++) {
		for (size_t j = 0; j < symbols_count - i - 1; j++) {
			if (ft_strncmp(symbols[j].name_cpy, symbols[j + 1].name_cpy, ft_strlen(symbols[j].name_cpy)) > 0)
			swap_symbols(&symbols[j], &symbols[j + 1]);
		}
	}
}

static void	print_symbols(t_symbols_info *symbols, size_t symbols_count)
{
	for (size_t i = 0; i < symbols_count; i++) {
		if (symbols[i].value == 0)
			printf("                 ");
		else
			printf("%016lx ", symbols[i].value);
		printf("%c %s\n", symbols[i].type, symbols[i].name);
	}
}

static t_err	get_symbols(t_nm *nm, Elf64_Ehdr *header, Elf64_Shdr *section_header, Elf64_Shdr *current, t_symbols_info **sym_arr, size_t *symbol_count)
{
	Elf64_Shdr	*strtab_section = &section_header[current->sh_link];
	Elf64_Sym	*symbols = (Elf64_Sym *)(nm->file_map + current->sh_offset);
	
	int	count = 0;
	int	total_symbol_count = current->sh_size / sizeof(Elf64_Sym);
	char	*symtab_data = (char *)(nm->file_map + strtab_section->sh_offset);
			
	t_symbols_info	*symbol_arr = malloc(total_symbol_count * sizeof(**sym_arr));
	if (!sym_arr) return MALLOC_ERR;

	for (int j = 0; j < total_symbol_count; j++) {
		Elf64_Sym	*symbol = &symbols[j];
		if (ELF64_ST_TYPE(symbol->st_info) == STT_FILE || symbol->st_name == 0)
			continue;

		symbol_arr[count].type = get_symbol_type(symbol, section_header, header);
		symbol_arr[count].value = symbol->st_value;
		symbol_arr[count].name = symtab_data + symbol->st_name;
		symbol_arr[count].name_cpy = ft_strtrim(symtab_data + symbol->st_name, "_");
		str_to_lower(symbol_arr[count].name_cpy);
		count++;
	}
	*symbol_count = count;
	*sym_arr = symbol_arr;
	return NO_ERR;
}

t_err	ft_nm64(t_nm *nm)
{
	Elf64_Ehdr	*header = (Elf64_Ehdr *)nm->file_map;
	Elf64_Shdr	*section_header = (Elf64_Shdr *)(nm->file_map + header->e_shoff);

	bool	has_symbol = false;
	t_err	ret_val = 0;
	size_t	symbol_count = 0;
	t_symbols_info	*sym_arr = NULL;

	for (int i = 0; i < header->e_shnum; i++) {
		Elf64_Shdr	*current_section = &section_header[i];

		if (current_section->sh_type == SHT_SYMTAB) {
			has_symbol = true;
			ret_val = get_symbols(nm, header, section_header, current_section, &sym_arr, &symbol_count);
			if (ret_val != NO_ERR) return ret_val;
		}
	}
	if (has_symbol == false) return NO_SYMBOL_ERR;
	
	sort_symbols(sym_arr, symbol_count);
	print_symbols(sym_arr, symbol_count);
	clean_sym_struct(sym_arr, symbol_count);
	return NO_ERR;
}