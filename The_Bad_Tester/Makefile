ifeq ($(shell uname -s),Linux)
LINUX=true
endif

# TODO: Turn into shared lib so the binds work correctly
NAME = JTester2.0.so

SRC_DIR = src/
OBJ_DIR = obj/

PRELOADS = $(patsubst %,preloads/%,$(shell ls preloads))

INCLUDE_DIRS = include/ $(PRELOADS)
INCLUDE = $(patsubst %,-I%,$(INCLUDE_DIRS))

CFLAGS = -Wall -Wextra -Werror

# I DO NOT CARE
CFLAGS += -Wno-unused-variable -Wno-unused-function -Wno-unused-parameter -Wno-unknown-pragmas

# make shared lib
CFLAGS += -fPIC

ifdef LINUX
CFLAGS += -Wno-unused-but-set-variable
PRELOAD_FLAGS = -fPIC -shared -nostartfiles -ldl -lgcc -rdynamic $(CFLAGS)
else
#CFLAGS +=
PRELOAD_FLAGS = -fPIC -shared -nostartfiles -ldl -dynamiclib $(CFLAGS)
endif

BIND_FLAGS = -fPIC -shared $(CFLAGS)

COMPILE_PATH = $(shell pwd)

CFLAGS += -D COMPILE_PATH=\"$(COMPILE_PATH)/\"

SOURCE_FILES = $(shell find $(SRC_DIR) -type f -name "*.c")
HEADER_FILES = $(shell find $(INCLUDE_DIRS) -type f -name "*.h")

OBJECTS = $(SOURCE_FILES:$(SRC_DIR)%.c=$(OBJ_DIR)%.o)

SOURCE_PRELOAD_LIBS = $(shell find preloads -type f -name *_preload.c)
SOURCE_BIND_LIBS = $(shell find preloads -type f -name *_bind.c)

PRELOAD_LIBS = $(SOURCE_PRELOAD_LIBS:%.c=%.so)
BIND_LIBS = $(SOURCE_BIND_LIBS:%.c=%.so)

all: $(NAME) $(PRELOAD_LIBS) $(BIND_LIBS)

#@gcc -o $(NAME) $(CFLAGS) $(OBJECTS) $(BIND_LIBS:%=$(COMPILE_PATH)/%)
$(NAME): $(OBJECTS) $(BIND_LIBS)
	@echo Make $@
	# ar -rcs $(NAME) $(OBJECTS)
	gcc -shared -o $(NAME) $(OBJECTS)

$(OBJ_DIR) $(INCLUDE_DIRS) $(SRC_DIR):
	@echo Make $@
	@mkdir $@

$(OBJECTS): $(OBJ_DIR)%.o: $(SRC_DIR)%.c $(HEADER_FILES) Makefile \
	| $(SRC_DIR) $(OBJ_DIR)
	@echo Make $@
	@mkdir -p $(shell dirname $@)
	@$(CC) $(CFLAGS) $(INCLUDE) -c -o $@ $<

#test.out: test.c Makefile
#	@echo Make $@
#	@$(CC) $(CFLAGS) $(INCLUDE) -o $@ $< $(BIND_LIBS)

# for some reason it does not work if i have the PRELOAD_FLAGS at the start
# also on mac we also need to compile with the bind preload since it uses interpose now
ifdef LINUX
%preload.so: %preload.c Makefile
else
%preload.so: %preload.c %bind.so Makefile
endif
	@echo Make $@
ifdef LINUX
	@gcc $(INCLUDE) -o $@ $< $(PRELOAD_FLAGS)
else
	gcc $(INCLUDE) -o $@ $< $(patsubst %preload.so,%bind.so,$@) $(PRELOAD_FLAGS)
endif

%bind.so: %bind.c Makefile
	@echo Make $@
	@gcc $(INCLUDE) -o $@ $< $(BIND_FLAGS)

clean:
	rm -fd $(OBJECTS) $(OBJ_DIR)
	rm -f $(PRELOAD_LIBS)
	rm -f $(BIND_LIBS)

re:
	$(MAKE) clean
	$(MAKE) all