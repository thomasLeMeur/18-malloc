# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: tle-meur <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2015/11/23 18:11:41 by tle-meur          #+#    #+#              #
#    Updated: 2017/02/08 16:02:01 by tle-meur         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

ifeq ($(HOSTTYPE), )
	HOSTTYPE=$(shell uname -m)_$(shell uname -s)
endif


SUBNAME	=	libft_malloc

NAME	=	$(SUBNAME)_$(HOSTTYPE).so

CC		=	gcc

FLAGS	=	-Wall -Wextra -Werror

LIBS	=	-lpthread

INCSDIR	=	includes/

SRCSDIR	=	srcs/

SRCS	=	$(SRCSDIR)print.c $(SRCSDIR)malloc.c $(SRCSDIR)others.c \
			$(SRCSDIR)segments.c $(SRCSDIR)utilities.c

OBJS	=	$(SRCS:.c=.o)


all		:	$(NAME)

%.o		:	%.c
			@$(CC) -fPIC $(FLAGS) -o $@ -c $< -I $(INCSDIR)

$(NAME)	:	$(OBJS)
			@$(CC) -shared -o $(NAME) $(OBJS) $(LIBS) -I $(INCSDIR)
			@echo "$(NAME) created"
			@rm -f $(SUBNAME).so
			ln -s $(NAME) $(SUBNAME).so

clean	:
			@rm -f $(OBJS)

fclean	:	clean
			@rm -f $(NAME) $(SUBNAME).so
			@echo "$(NAME) deleted"

re		:	fclean $(NAME)


.PHONY	:	all clean fclean re
