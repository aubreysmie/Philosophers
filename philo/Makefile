# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ekhaled <ekhaled@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/10/23 14:56:45 by ekhaled           #+#    #+#              #
#    Updated: 2023/11/18 19:17:31 by ekhaled          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		:=	philo


SRC_DIR		:=	sources
OBJ_DIR		:=	objects
SRCS		:=	destroy.c \
				dinner_helpers.c \
				dinner.c \
				display.c \
				init.c \
				main.c \
				parse_helpers.c \
				parse.c \
				threads_manager.c \
				time_checks.c
SRCS		:=	${SRCS:%=${SRC_DIR}/%}
OBJS		:=	${SRCS:${SRC_DIR}/%.c=${OBJ_DIR}/%.o}

# CC			:=	cc
LIBFLAGS	:=	-pthread
DEBUGFLAG	:=	-gdwarf-4
CFLAGS		:=	-Wall -Wextra -Werror ${DEBUGFLAG}
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
