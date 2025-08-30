/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkara2 <tkara2@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/27 12:19:35 by tkara2            #+#    #+#             */
/*   Updated: 2025/08/30 13:18:56 by tkara2           ###   ########.fr       */
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

t_err	merge(t_symbols_info *symbols, int left, int mid, int right, bool reverse)
{
	int	i, j, k;
	int	first_half = mid - left + 1;
	int	second_half = right - mid;

	t_symbols_info	*left_symbols = malloc(first_half * sizeof(*left_symbols));
	t_symbols_info	*right_symbols = malloc(second_half * sizeof(*right_symbols));

	if (!left_symbols || !right_symbols) return MALLOC_ERR;

	for (i = 0; i < first_half; i++)
		left_symbols[i] = symbols[left + i];
	for (j = 0; j < second_half; j++)
		right_symbols[j] = symbols[mid + 1 + j];

	i = 0;
	j = 0;
	k = left;
	while (i < first_half && j < second_half) {
		int	comparison = ft_strcmp(left_symbols[i].name_cpy, right_symbols[j].name_cpy);
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

	free(left_symbols);
	free(right_symbols);
	return NO_ERR;
}

t_err	merge_sort(t_symbols_info *symbols, int left, int right, bool reverse)
{
	t_err	ret = 0;

	if (left < right) {
		int	mid = left + (right - left) / 2;
		merge_sort(symbols, left, mid, reverse);
		merge_sort(symbols, mid + 1, right, reverse);
		ret = merge(symbols, left, mid, right, reverse);
		if (ret != NO_ERR) return ret;
	}

	return NO_ERR;
}

t_symbols_sort	get_sorting_type(t_opt *options)
{
	if (options->opt_r == true) return REVERSE_SORT;
	if (options->opt_p == true) return NO_SORT;
	if (options->opt_r == false && options->opt_p == false) return NORMAL_SORT;
	return UNREACHABLE;
}

void	print_symbols(t_symbols_info *symbols, size_t symbols_count, bool is_64bits)
{
	for (size_t i = 0; i < symbols_count; i++) {
		if (is_64bits == false && symbols[i].value == 0)
			printf("         ");
		else if (is_64bits == true && symbols[i].value == 0)
			printf("                 ");
		else if (is_64bits == false)
			printf("%08x ", (unsigned int)symbols[i].value);
		else
			printf("%016lx ", symbols[i].value);
		printf("%c %s\n", symbols[i].type, symbols[i].name);
	}
}