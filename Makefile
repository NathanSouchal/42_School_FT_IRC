# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: nsouchal <nsouchal@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/10/18 15:55:18 by tnicolau          #+#    #+#              #
#    Updated: 2024/11/05 12:59:57 by nsouchal         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = ircserv
CC = c++
FLAGS = -Wall -Wextra -Werror -std=c++98 -g3

DIR_SRCS 		= 	srcs/
DIR_CMD 		= 	srcs/commands/
DIR_HEADERS		=	includes/

HEADER =	$(DIR_HEADERS)Server.hpp \
			$(DIR_HEADERS)Client.hpp \
			$(DIR_HEADERS)Channel.hpp \
			$(DIR_HEADERS)numerics.hpp \
			$(DIR_HEADERS)utils.hpp

SRC =	$(DIR_SRCS)main.cpp \
		$(DIR_SRCS)Server.cpp \
		$(DIR_SRCS)Client.cpp \
		$(DIR_SRCS)Channel.cpp \
		$(DIR_SRCS)parseMessage.cpp \
		$(DIR_SRCS)serverSetup.cpp \
		$(DIR_SRCS)utils.cpp \
		$(DIR_CMD)invite.cpp \
		$(DIR_CMD)join.cpp \
		$(DIR_CMD)kick.cpp \
		$(DIR_CMD)lusers.cpp \
		$(DIR_CMD)mode.cpp \
		$(DIR_CMD)nick.cpp \
		$(DIR_CMD)privmsg.cpp \
		$(DIR_CMD)topic.cpp \
		$(DIR_CMD)user.cpp \
		$(DIR_CMD)part.cpp 

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
