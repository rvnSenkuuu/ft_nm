# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: tkara2 <tkara2@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/08/12 14:39:41 by tkara2            #+#    #+#              #
#    Updated: 2025/08/12 14:57:35 by tkara2           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME    = ft_nm
CC      = cc
CFLAGS  = -Wall -Werror -Wextra -Iincs -MMD -MP
RM      = rm -rf

SRCS    = srcs/main.c
INCS    = ./incs/ft_nm.h

OBJSDIR = .objs/
OBJS    = $(patsubst %.c, $(OBJSDIR)%.o, $(SRCS))
D_FILES = $(OBJS:.o=.d)

all: $(NAME)

$(NAME): $(OBJS) $(INCS)
	@$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

$(OBJSDIR)%.o: %.c
	mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJSDIR)

fclean: clean
	$(RM) $(NAME)

re: fclean all

sinclude $(D_FILES)

.PHONY: all clean fclean re