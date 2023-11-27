NAME = minishell
SRC = minishell.c env.c unset.c export.c pwd.c cd.c echo.c strtok.c ext_command.c
OBJ = $(SRC:%.c=%.o)
CC = gcc
FLAGS = -Werror -Wall -Wextra

all : $(NAME)
$(NAME) :
	$(CC) -o $(NAME) $(SRC) -lreadline $(FLAGS)

clean :
	rm -f $(OBJ)
fclean : clean
	rm -f $(NAME)
re : fclean all