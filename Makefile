#* ************************************************************************** *#
#*                                                                            *#
#*                                                        :::      ::::::::   *#
#*   Makefile                                           :+:      :+:    :+:   *#
#*                                                    +:+ +:+         +:+     *#
#*   By: psebasti <sebpalluel@free.fr>              +#+  +:+       +#+        *#
#*                                                +#+#+#+#+#+   +#+           *#
#*   Created: 2017/01/23 15:37:07 by psebasti          #+#    #+#             *#
#*   Updated: 2017/09/25 13:43:15 by psebasti         ###   ########.fr       *#
#*                                                                            *#
#* ************************************************************************** *#

NAME = get_next_line

SRC = get_next_line.c \
	  main.c

OBJ		=	$(SRC:.c=.o)

CMP		=	gcc

FLAGS	=	-Wall -Wextra -Werror

LIB = -L libft/ -lft

all : lib $(NAME)

$(NAME) :
	$(CMP) $(FLAGS) -o $(NAME) $(SRC) $(LIB)
	@echo "get_next_line compiled"

lib :
	@echo "compiling libft..."
	@make -C libft/ --no-print-directory
	@echo "libft compiled"

clean :
	@rm -f $(OBJ)
	@echo "cleaning libft..."
	@make -C libft clean --no-print-directory
	@echo "clean done"

fclean : clean
	@rm -f $(NAME)
	@echo "full cleaning libft..."
	@make -C libft fclean --no-print-directory

re : fclean all

.PHONY: $(NAME) all clean fclean re
