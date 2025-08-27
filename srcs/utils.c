/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkara2 <tkara2@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/27 12:19:35 by tkara2            #+#    #+#             */
/*   Updated: 2025/08/27 12:20:39 by tkara2           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

void	swap_symbols(t_sym_arr *a, t_sym_arr *b)
{
	t_sym_arr tmp = *a;
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