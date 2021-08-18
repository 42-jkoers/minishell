# there are 4 defines to define how you want to compile
#	#1 = TEST_FOLDER > what folder should we compile?
#	#2 = GCC_ARGS > do you want extra GCC args when compiling the test folder? (-Wall -Werror and that stuff is already defined)
#	#3 = LIB > What extra files are added to making the main?
#	#4 = PRELOADS > What preloads do you want? for example: PRELOADS=malloc and PRELOADS="malloc other_preload"
#	#5 = NAME > what name do we give the resulting executable?

ifndef TEST_FOLDER
$(error oi what are you doing specify a TEST_FOLDER)
endif

ifeq ($(shell uname -s),Linux)
LINUX=true
endif

EXEC_NAME ?= "do_test.out"

#PRELOADS = $(patsubst %,../preloads/%,$(shell ls ../preloads))

PRELOADS += "malloc"	# Annoying that i have to use this since the JTester code USES a function from that
PATH_PRELOADS = $(patsubst %,../preloads/%,$(PRELOADS))

INCLUDE_DIRS = ../include/ $(PATH_PRELOADS)
INCLUDE = $(patsubst %,-I%,$(INCLUDE_DIRS))

MAKE_PATH = $(shell pwd)

SOURCE_BIND_LIBS = $(shell find $(PATH_PRELOADS) -type f -name *_bind.c)
BIND_LIBS = $(SOURCE_BIND_LIBS:%.c=%.so)

CFLAGS = -D COMPILE_PATH=\"$(TEST_FOLDER)/\" $(GCC_ARGS)

# I DO NOT CARE
CFLAGS += -Wno-unused-variable -Wno-unused-function -Wno-unused-parameter -Wno-unknown-pragmas

ifdef LINUX
CFLAGS += -Wno-unused-but-set-variable
endif

TEST_C_FILES = $(shell find $(TEST_FOLDER) -type f -name "*.c")

# Update JTester2.0.a just in case
_ = $(shell make --no-print-directory -C ..)

GCC_ARGS += -Wall -Wextra -Werror

$(TEST_FOLDER)/$(EXEC_NAME): $(TEST_C_FILES) ../JTester2.0.so Makefile
	@echo Make $@
	@gcc -o $(TEST_FOLDER)/$(EXEC_NAME) $(INCLUDE) $(CFLAGS) $(TEST_C_FILES) $(LIB) $(MAKE_PATH)/../JTester2.0.so $(BIND_LIBS:%=$(MAKE_PATH)/%)
