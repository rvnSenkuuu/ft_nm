/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_nm.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkara2 <tkara2@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 14:56:23 by tkara2            #+#    #+#             */
/*   Updated: 2025/08/30 12:39:18 by tkara2           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_NM_H
#define FT_NM_H

#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <elf.h>

#include "../lib/libft/includes/libft.h"

#define DEFAULT_FILE "a.out"

typedef struct {
	bool	opt_a;
	bool	opt_g;
	bool	opt_u;
	bool	opt_r;
	bool	opt_p;
} t_opt;

typedef struct {
	int	fd;
	char	*file_name;
	const char	*program_name;
	void	*file_map;
	struct stat	file_stat;
	t_opt	options;
} t_nm;

typedef struct {
	char	type;
	size_t	value;
	char	*name;
	char	*name_cpy;
} t_symbols_info;

typedef enum {
	NO_ERR,
	NM_ERR,
	NO_SYMBOL_ERR,
	MALLOC_ERR,
	OPEN_SYSCALL_ERR,
	FSTAT_SYSCALL_ERR,
	MMAP_SYSCALL_ERR,
	STAT_FILE_ERR,
	ELF_FILE_FORMAT_ERR
} t_err;

typedef enum {
	UNREACHABLE = -1,
	NO_SORT,
	REVERSE_SORT,
	NORMAL_SORT
} t_symbols_sort;

void	parse_opt(t_opt *options, int *starting_index, char **argv);
void	clean_nm_struct(t_nm *nm);
void	clean_sym_struct(t_symbols_info *sym_arr, size_t sym_count);
int	init_nm_struct(t_nm *nm, const char *file_name, t_opt *options);
int	check_elf_file(Elf64_Ehdr *elf_header, struct stat *file_stat);
t_err	ft_nm32(t_nm *nm);
t_err	ft_nm64(t_nm *nm);

t_err	merge_sort(t_symbols_info *symbols, int left, int right, bool reverse);
void	swap_symbols(t_symbols_info *a, t_symbols_info *b);
void	str_to_lower(char *str);
t_symbols_sort	get_sorting_type(t_opt *options);
char	*get_error_type(t_err error);

#endif