# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ekhaled <ekhaled@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/11/22 09:52:09 by ekhaled           #+#    #+#              #
#    Updated: 2023/11/22 09:57:40 by ekhaled          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		:=	philo_bonus

SRC_DIR		:=	sources
OBJ_DIR		:=	objects
SRCS		:=	destroy_bonus.c \
				dinner_bonus.c \
				display_bonus.c \
				init_bonus.c \
				main_bonus.c \
				parse_bonus.c \
				parse_helpers_bonus.c \
				processes_manager_bonus.c \
				threads_manager_bonus.c \
				time_manager_bonus.c
SRCS		:=	${SRCS:%=${SRC_DIR}/%}
OBJS		:=	${SRCS:${SRC_DIR}/%.c=${OBJ_DIR}/%.o}

LIBFLAGS	:=	-pthread
CFLAGS		:=	-Wall -Wextra -Werror
CPPFLAGS	:=	-I includes


RM			:=	${RM} -r
DIR_DUP		=	mkdir -p ${@D}


all: ${NAME}

${NAME}: ${OBJS}
	${CC} $^ ${LIBFLAGS} -o $@

${OBJ_DIR}/%.o: ${SRC_DIR}/%.c
	${DIR_DUP}
	${CC} ${LIBFLAGS} ${CFLAGS} ${CPPFLAGS} -c -o $@ $<

clean:
	${RM} ${OBJ_DIR}
	

fclean: clean
	${RM} ${NAME}

re: fclean
	${MAKE} all