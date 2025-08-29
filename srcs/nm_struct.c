/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nm_struct.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkara2 <tkara2@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 16:38:30 by tkara2            #+#    #+#             */
/*   Updated: 2025/08/29 13:33:58 by tkara2           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

int	init_nm_struct(t_nm *nm, const char *file_name, t_opt *options)
{
	nm->program_name = "nm";
	nm->file_name = ft_strdup(file_name);
	if (!nm->file_name) return MALLOC_ERR;

	nm->fd = open(nm->file_name, O_RDONLY);
	if (nm->fd < 0) return OPEN_SYSCALL_ERR;

	if (fstat(nm->fd, &nm->file_stat) < 0) return FSTAT_SYSCALL_ERR;

	nm->file_map = mmap(NULL, nm->file_stat.st_size, PROT_READ, MAP_PRIVATE, nm->fd, 0);
	if (nm->file_map == MAP_FAILED) return MMAP_SYSCALL_ERR;

	nm->options = *options;

	return NO_ERR;
}

void	clean_nm_struct(t_nm *nm)
{
	if (nm->fd != -1)
		close(nm->fd);
	nm->fd = -1;
	if (nm->file_name)
		free(nm->file_name);
	if (nm->file_map)
		munmap(nm->file_map, nm->file_stat.st_size);
	ft_memset(&nm->file_stat, 0, sizeof(nm->file_stat));
}

void	clean_sym_struct(t_symbols_info *sym_arr, size_t sym_count)
{
	for (size_t i = 0; i < sym_count; i++) {
		free(sym_arr[i].name_cpy);
	}
	free(sym_arr);
}