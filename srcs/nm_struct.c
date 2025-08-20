/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nm_struct.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkara2 <tkara2@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 16:38:30 by tkara2            #+#    #+#             */
/*   Updated: 2025/08/20 13:42:05 by tkara2           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

int	init_nm_struct(t_nm *nm, const char *file_name)
{
	nm->program_name = "nm";
	nm->file_name = ft_strdup(file_name);
	if (!nm->file_name) {
		perror("Ft_strdup");
		return 1;
	}

	nm->fd = open(nm->file_name, O_RDONLY);
	if (nm->fd < 0) {
		perror("Open");
		return 1;
	}

	if (fstat(nm->fd, &nm->file_stat) < 0) {
		perror("Fstat");
		return 1;
	}

	nm->file_map = mmap(NULL, nm->file_stat.st_size, PROT_READ, MAP_PRIVATE, nm->fd, 0);
	if (nm->file_map == MAP_FAILED) {
		perror("Mmap");
		return 1;
	}

	return 0;
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