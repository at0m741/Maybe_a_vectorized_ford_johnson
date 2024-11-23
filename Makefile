NAME = PmergeMe

CC = c++

CFLAGS = -Wall -Wextra -Werror -std=c++98 -march=native -mavx2 

SRC = main.cpp \
	  Jacobsthal.cpp \
	  PmergeMe.cpp \
	  utils_avx.cpp	\
	  compare.cpp \
	  compare2.cpp \
	  PmergeMe2.cpp \
	  Jacobsthal2.cpp \

OBJ = $(SRC:.cpp=.o)

ifeq ($(DEBUG), true)
	CFLAGS += -g -D DEBUG
endif

ifeq ($(AVX512), 1)
	CFLAGS += -mavx512f -mavx512dq -mavx512cd -mavx512bw -mavx512vl
endif

all: $(NAME)

opti: CFLAGS += -O3 -mtune=native
opti: re

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
