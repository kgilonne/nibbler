NAME = nibbler
C_ARES = ./lib/libcares.a
CC = gcc
VPATH = srcs
INC = -I./include

LIB = -L./lib -lcares
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
	LIB += -lrt
endif

LDFLAGS = -static 
CFLAGS = -O3 -Wall -Wextra -Werror $(INC) $(LDFLAGS)
SRC = \
	main.c \
	domain_examiner.c \
	examin_result_loop.c \
	examin_perform_records.c \
	examin_print_result.c \
	examin_check_error.c \
	examin_check_spf.c \
	ft_strjoin.c \
	ft_strsplit.c \
	ft_putnbr_fd.c \
	ft_clean.c
OBJDIR = objs
OBJ = $(addprefix $(OBJDIR)/, $(patsubst %.c, %.o, $(SRC)))

all: $(C_ARES) $(NAME)

$(C_ARES):
	tar -xf ./lib/c-ares-1.10.0.tar.gz
	cd ./c-ares-1.10.0;	./configure --prefix=$(PWD) --disable-shared; make; make install
	rm -r c-ares-1.10.0/

clean_c-ares:
	rm -rf $(C_ARES) ./lib/libcares.la ./lib/pkgconfig
	rm -f include/ares*
	rm -rf share

reinstall_c-ares: clean_c-ares $(C_ARES)

$(OBJDIR):
	mkdir $(OBJDIR)

$(OBJDIR)/%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(NAME): $(OBJDIR) $(OBJ)
	$(CC) $(CFLAG) $(OBJ) $(LIB) -o $(NAME) $(LIBS)

clean:
	rm -rf $(OBJDIR)

fclean: clean
	rm -f $(NAME)

re: fclean all 

.PHONY: all clean fclean re clean_c-ares
