/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkara2 <tkara2@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/27 12:19:35 by tkara2            #+#    #+#             */
/*   Updated: 2025/08/29 13:57:22 by tkara2           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

void	swap_symbols(t_symbols_info *a, t_symbols_info *b)
{
	t_symbols_info tmp = *a;
	*a = *b;
	*b = tmp;
}

void	str_to_lower(char *str)
{
	int	len = ft_strlen(str);
	for (int i = 0; i < len; i++) {
		if (str[i] >= 'A' && str[i] <= 'Z')
			str[i] += 32;
	}
}

char	*get_error_type(t_err error)
{
	switch (error) {
		case NO_SYMBOL_ERR: return "No symbols";
		case MALLOC_ERR: return "Malloc failed";
		case OPEN_SYSCALL_ERR: return "Open syscall failed";
		case FSTAT_SYSCALL_ERR: return "Fstat syscall failed";
		case MMAP_SYSCALL_ERR: return "Mmap syscall failed";
		case STAT_FILE_ERR: return "Stat file error";
		case ELF_FILE_FORMAT_ERR: return "Elf file format error";
		default: return "Unreachable";
	}
}

void	sort_symbols(t_symbols_info *symbols, size_t symbols_count, t_symbols_sort sort)
{
	if (sort == NORMAL_SORT) {
		for (size_t i = 0; i < symbols_count - 1; i++) {
			for (size_t j = 0; j < symbols_count - i - 1; j++) {
				if (ft_strncmp(symbols[j].name_cpy, symbols[j + 1].name_cpy, ft_strlen(symbols[j].name_cpy)) > 0)
					swap_symbols(&symbols[j], &symbols[j + 1]);
			}
		}
	} else {
		for (size_t i = 0; i < symbols_count - 1; i++) {
			for (size_t j = 0; j < symbols_count - i - 1; j++) {
				if (ft_strncmp(symbols[j].name_cpy, symbols[j + 1].name_cpy, ft_strlen(symbols[j].name_cpy)) < 0)
					swap_symbols(&symbols[j], &symbols[j + 1]);
			}
		}
	}
}

t_symbols_sort	get_sorting_type(t_opt *options)
{
	if (options->opt_r == true) return REVERSE_SORT;
	if (options->opt_p == true) return NO_SORT;
	if (options->opt_r == false && options->opt_p == false) return NORMAL_SORT;
	return -1;
}