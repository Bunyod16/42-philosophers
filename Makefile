GCC=gcc -Wall -Werror -Wextra
CFLAGS=-pthread
SOURCES=main.c \
		ft_utils.c
NAME=philo
MAKEFLAGS += --no-print-directory
OBJECTS = $(SOURCES:.c=.o)

all: $(NAME)

$(NAME): $(OBJECTS)
	$(GCC) $(CFLAGS) $(OBJECTS) -o $(NAME)

%.o: %.c
	$(GCC) -c $(CFLAGS) $?

clean:
	rm -rf $(OBJECTS)

re: clean all

fclean: clean
	rm -rf $(NAME)
