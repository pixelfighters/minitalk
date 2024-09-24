# Paths
LIBFT_DIR = ./libft
INC = ./includes


# Compiler and Flags
UNAME := $(shell uname)
CC = cc
CFLAGS = -Wall -Wextra -Werror -I$(INC) -I$(LIBFT_DIR) -O3 -g
LFLAGS = -L$(LIBFT_DIR) -lft -L$(MINILIBX_DIR) -lm

# Adjust LFLAGS for non-macOS systems
ifeq ($(UNAME), Darwin)
    CC = clang
else ifeq ($(UNAME), FreeBSD)
    CC = clang
else
    LFLAGS += -lbsd
endif

# Project Files
NAME = client
SRC = client.c 
NAME_SRV = server
SRC_SRV = server.c 
OBJ = $(SRC:.c=.o)
OBJ_SRV = $(SRC_SRV:.c=.o)

# Targets
all: $(NAME) $(NAME_SRV)

$(NAME): $(OBJ) $(LIBFT_DIR)/libft.a
	$(CC) $(OBJ) -o $@ $(LFLAGS)


$(NAME_SRV): $(OBJ_SRV) $(LIBFT_DIR)/libft.a
	$(CC) $(OBJ_SRV) -o $@ $(LFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(NAME)
	rm -f $(OBJ_SRV) $(NAME_SRV)	

fclean: clean
	rm -f $(LIBFT_DIR)/*.o

re: fclean all

.PHONY: all clean fclean re
