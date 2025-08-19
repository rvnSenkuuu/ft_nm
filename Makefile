# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: tkara2 <tkara2@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/08/12 14:39:41 by tkara2            #+#    #+#              #
#    Updated: 2025/08/19 18:11:11 by tkara2           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME    = ft_nm
CC      = cc
CFLAGS  = -Wall -Werror -Wextra -Iincs -I$(LIBFT_PATH) -MMD -MP -finline-functions
RM      = rm -rf

SRCS    = srcs/main.c \
		srcs/nm_struct.c \
		srcs/check_elf.c
INCS    = ./incs/ft_nm.h

OBJSDIR = .objs/
OBJS    = $(patsubst %.c, $(OBJSDIR)%.o, $(SRCS))
D_FILES = $(OBJS:.o=.d)

LIBFT_PATH = ./lib/libft/
LIBFT      = $(LIBFT_PATH)libft.a

all: $(NAME)

$(NAME): $(OBJS) $(INCS) $(LIBFT) 
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT) -o $(NAME)

$(OBJSDIR)%.o: %.c
	mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

$(LIBFT):
	$(MAKE) -C $(LIBFT_PATH)

clean:
	$(RM) $(OBJSDIR)
	$(MAKE) clean -C $(LIBFT_PATH)

fclean: clean
	$(RM) $(NAME)
	$(MAKE) fclean -C $(LIBFT_PATH)

re: fclean
	$(MAKE) all

sinclude $(D_FILES)

.PHONY: all clean fclean re