# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: yeju <yeju@student.42seoul.kr>             +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/11/02 00:41:56 by yeju              #+#    #+#              #
#    Updated: 2021/11/05 23:10:55 by yeju             ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC				=	gcc
CFLAGS			=	-Wall -Wextra -Werror

SERVER			=	server
CLIENT			=	client
SERVER_B		=	server_bonus
CLIENT_B		=	client_bonus

SER_SRCS		=	server.c utils1.c utils2.c
SER_OBJS		=	$(SER_SRCS:.c=.o)
CLI_SRCS		=	client.c utils1.c utils2.c
CLI_OBJS		=	$(CLI_SRCS:.c=.o)

SER_SRCS_B	=	server_bonus.c utils1.c utils2.c
SER_OBJS_B	=	$(SER_SRCS_B:.c=.o)
CLI_SRCS_B	=	client_bonus.c utils1.c utils2.c
CLI_OBJS_B	=	$(CLI_SRCS_B:.c=.o)

all: $(SERVER) $(CLIENT) $(SER_OBJS) $(CLI_OBJS) 

$(SERVER) :
	$(CC) $(CFLAGS) $(SER_SRCS) -o $(SERVER)
$(CLIENT) :
	$(CC) $(CFLAGS) $(CLI_SRCS) -o $(CLIENT)

bonus: fclean
	$(CC) $(CFLAGS) $(SER_SRCS_B) -o $(SERVER)
	$(CC) $(CFLAGS) $(CLI_SRCS_B) -o $(CLIENT)

clean:
	rm -rf $(SER_OBJS) $(CLI_OBJS) $(SER_OBJS_B) $(CLI_OBJS_B)

fclean: clean
	rm -rf $(SERVER) $(CLIENT) $(SERVER_B) $(CLIENT_B)

re: fclean all

.PHONY: all bonus clean fclean re