NAME = libft.a

SOURCES_MK = Sources.mk
SRC_DIR = src/
OBJ_DIR = obj/
PREREQ_DIR = prereq/
DEBUG_DIR = obj_debug/
INCLUDE_DIRS = include/

CFLAGS = -DBUFFER_SIZE=128 -Wall -Wextra -Werror
CFLAGS += -O2
CFLAGS += $(INCLUDE_DIRS:%=-I%)

include $(SOURCES_MK)

OBJECTS = $(SOURCE_FILES:$(SRC_DIR)%.c=$(OBJ_DIR)%.o)
PREREQS = $(SOURCE_FILES:$(SRC_DIR)%.c=$(PREREQ_DIR)%.d)

.PHONY: all
all: $(NAME)

include $(PREREQS)

$(NAME): $(OBJECTS) Makefile | $(OBJ_DIR)
	ar -rc $(NAME) $(OBJECTS)
	# mark non-debug or debug dirty, after making libft.a

$(OBJECTS): Makefile | $(SRC_DIR) $(OBJ_DIR)
	mkdir -p $(shell dirname $@)
	$(CC) $(CFLAGS) -c -o $@ $(@:$(OBJ_DIR)%.o=$(SRC_DIR)%.c)

$(OBJ_DIR) $(SRC_DIR) $(PREREQ_DIR):
	mkdir $@

# magic includes
$(SOURCES_MK): $(SRC_DIR) Makefile
	printf "SOURCE_FILES = " > $@
	find $(SRC_DIR) -type f -name *.c -exec echo "{} \\" \; >> $@

$(PREREQ_DIR)%.d: $(SRC_DIR)%.c
	mkdir -p $(shell dirname $@)
	printf "$(shell dirname $(patsubst $(SRC_DIR)%,$(OBJ_DIR)%,$<))/" > $@
	$(CC) -MM $< $(CFLAGS) >> $@

# general management
.PHONY: clean
clean:
	rm -rf $(OBJ_DIR) $(DEBUG_DIR)
