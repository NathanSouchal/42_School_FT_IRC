# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: tnicolau <tnicolau@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/10/18 15:55:18 by tnicolau          #+#    #+#              #
#    Updated: 2024/10/22 08:52:30 by tnicolau         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = ircserv
CC = c++
FLAGS = -Wall -Wextra -Werror -std=c++98

DIR_SRCS 		= 	srcs/
DIR_HEADERS		=	includes/

HEADER =	$(DIR_HEADERS)Server.hpp \
			$(DIR_HEADERS)Client.hpp \
			$(DIR_HEADERS)Channel.hpp \
			$(DIR_HEADERS)numerics.hpp

SRC =	$(DIR_SRCS)main.cpp \
		$(DIR_SRCS)Server.cpp \
		$(DIR_SRCS)Client.cpp \
		$(DIR_SRCS)Channel.cpp \
		$(DIR_SRCS)commands.cpp

OBJS_DIR = .objs/
OBJS = $(patsubst %.cpp, $(OBJS_DIR)%.o, $(SRC))

$(OBJS_DIR)%.o: %.cpp $(HEADER)
				@mkdir -p $(@D)
				$(CC) $(FLAGS) -c $< -o $@ -Iincludes

$(NAME):	$(OBJS) $(HEADER)
			$(CC) $(OBJS) $(FLAGS) -o $(NAME)

all: $(NAME)

clean:
		rm -rf ${OBJS_DIR}

fclean:	clean
		rm -f ${NAME}

re:	fclean all

.PHONY:	all clean fclean re
