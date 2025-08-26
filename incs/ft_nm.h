/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_nm.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkara2 <tkara2@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 14:56:23 by tkara2            #+#    #+#             */
/*   Updated: 2025/08/26 18:32:15 by tkara2           ###   ########.fr       */
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
	int	fd;
	char	*file_name;
	const char	*program_name;
	void	*file_map;
	struct stat	file_stat;
} t_nm;

typedef struct
{
	char	type;
	size_t	value;
	size_t	symbol_count;
	char	*name;
} t_sym_arr;


int	ft_nm64(t_nm *nm);
void	ft_nm32(t_nm *nm);
void	clean_nm_struct(t_nm *nm);
int	init_nm_struct(t_nm *nm, const char *file_name);
int	check_elf_file(Elf64_Ehdr *elf_header, struct stat *file_stat);

#endif