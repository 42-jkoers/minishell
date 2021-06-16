NAME = libft.a

SRC_DIR = src/
OBJ_DIR = obj/
DEBUG_DIR = obj_debug/
INCLUDE_DIRS = include/
INCLUDE = $(INCLUDE_DIRS:%=-I%)
FLAGS = -DBUFFER_SIZE=128 -Wall -Wextra -Werror

#SOURCE_FILES = $(shell find $(SRC_DIR) -type f -name *.c)
include Sources.mk

ifdef DEBUG
FLAGS += -g
USE_OBJ_DIR = $(DEBUG_DIR)
OTHER_OBJ_DIR = $(OBJ_DIR)
else
FLAGS += -O2
USE_OBJ_DIR = $(OBJ_DIR)
OTHER_OBJ_DIR = $(DEBUG_DIR)
endif

OBJECTS = $(SOURCE_FILES:$(SRC_DIR)%.c=$(USE_OBJ_DIR)%.o)

.PHONY: all
all:
	$(MAKE) $(NAME)

.PHONY: debug
debug:
	$(MAKE) DEBUG=1

# because we have used the S flag, it does not have a index, (faster)
# run ar with the s flag to create the index
$(NAME): $(OBJECTS) Makefile | $(OBJ_DIR) $(DEBUG_DIR)
	ar -rc $(NAME) $(OBJECTS)
	# mark non-debug or debug dirty, after making libft.a
	touch -c $$(find $(OTHER_OBJ_DIR))

$(OBJ_DIR) $(DEBUG_DIR) $(SRC_DIR):
	mkdir $@

$(OBJECTS): $(USE_OBJ_DIR)%.o: $(SRC_DIR)%.c $(INCLUDE_DIRS) Makefile \
	| $(SRC_DIR) $(USE_OBJ_DIR)
	mkdir -p $(shell dirname $@)
	echo $(INCLUDE)
	$(CC) $(FLAGS) $(INCLUDE) -c -o $@ $<

.PHONY: Sources.mk
Sources.mk:
	printf "SOURCE_FILES = " > Sources.mk
	find $(SRC_DIR) -type f -name *.c -exec echo "{} \\" \; >> Sources.mk

.PHONY: clean
clean:
	rm -rf $(OBJ_DIR) $(DEBUG_DIR)

.PHONY: fclean
fclean: clean
	rm -f $(NAME)

.PHONY: re
re: | fclean all
