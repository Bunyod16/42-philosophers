GCC=gcc -Wall -Werror -Wextra
CFLAGS=-pthread
SOURCES=main.c \
		ft_utils.c \
		philo_life.c \
		assign_eat.c \
		ft_check.c

NAME=philo
MAKEFLAGS += --no-print-directory
OBJECTS = $(SOURCES:.c=.o)
DEBUG=
all: $(NAME)

$(NAME): $(OBJECTS)
	$(GCC) $(DEBUG) $(CFLAGS) $(OBJECTS) -o $(NAME) -fsanitize=thread

%.o: %.c
	$(GCC) -c $(CFLAGS) $?

clean:
	rm -rf $(OBJECTS)

re: clean all

fclean: clean
	rm -rf $(NAME)
