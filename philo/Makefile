# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ekhaled <ekhaled@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/10/23 14:56:45 by ekhaled           #+#    #+#              #
#    Updated: 2023/10/23 15:38:13 by ekhaled          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		:=	philo


SRC_DIR		:=	sources
OBJ_DIR		:=	objects
SRCS		:=	destroy.c \
				diner.c \
				display.c \
				helpers.c \
				init.c \
				main.c \
				parse.c
SRCS		:=	${SRCS:%=${SRC_DIR}/%}
OBJS		:=	${SRCS:${SRC_DIR}/%.c=${OBJ_DIR}/%.o}

# CC			:=	cc
LIBFLAGS	:=	-pthread
CFLAGS		:=	-Wall -Wextra -Werror
CPPFLAGS	:=	-I includes


# RM			:=	rm -r
DIR_DUP		=	mkdir -p ${@D}


all: ${NAME}

${NAME}: ${OBJS}
	${CC} $^ ${LIBFLAGS} -o $@

${OBJ_DIR}/%.o: ${SRC_DIR}/%.c
	${DIR_DUP}
	${CC} ${LIBFLAGS} ${CFLAGS} ${CPPFLAGS} -c -o $@ $<

clean:
	${RM} ${OBJS}

fclean: clean
	${RM} ${NAME}

re: fclean
	${MAKE} all