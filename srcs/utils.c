/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkara2 <tkara2@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/27 12:19:35 by tkara2            #+#    #+#             */
/*   Updated: 2025/08/29 18:51:32 by tkara2           ###   ########.fr       */
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

void	merge(t_symbols_info *symbols, int left, int mid, int right, bool reverse)
{
	int	i, j, k;
	int	first_half = mid - left + 1;
	int	second_half = right - mid;

	t_symbols_info	left_symbols[first_half], right_symbols[second_half];

	for (i = 0; i < first_half; i++)
		left_symbols[i] = symbols[left + i];
	for (j = 0; j < second_half; j++)
		right_symbols[j] = symbols[mid + 1 + j];

	i = 0;
	j = 0;
	k = left;
	while (i < first_half && j < second_half) {
		int	comparison = ft_strncmp(left_symbols[i].name_cpy, right_symbols[j].name_cpy, ft_strlen(left_symbols[i].name_cpy));
		bool	start_left = (reverse == true) ? (comparison > 0) : (comparison < 0);

		if (start_left) {
			symbols[k] = left_symbols[i];
			i++;
		} else {
			symbols[k] = right_symbols[j];
			j++;
		}
		k++;
	}

	while (i < first_half) {
		symbols[k] = left_symbols[i];
		i++;
		k++;
	}
	while (j < second_half) {
		symbols[k] = right_symbols[j];
		j++;
		k++;
	}
}

void	merge_sort(t_symbols_info *symbols, int left, int right, bool reverse)
{
	if (left < right) {
		int	mid = left + (right - left) / 2;
		merge_sort(symbols, left, mid, reverse);
		merge_sort(symbols, mid + 1, right, reverse);
		merge(symbols, left, mid, right, reverse);
	}
}

t_symbols_sort	get_sorting_type(t_opt *options)
{
	if (options->opt_r == true) return REVERSE_SORT;
	if (options->opt_p == true) return NO_SORT;
	if (options->opt_r == false && options->opt_p == false) return NORMAL_SORT;
	return UNREACHABLE;
}