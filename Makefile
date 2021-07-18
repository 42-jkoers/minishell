CC				= gcc

CFLAGS      	= -Wall -Wextra -Werror -O3
# CFLAGS			= -Wall -Wextra -Werror
# -Wno-error=unused-result -Wno-unused-result -g

MAIN_DIR		= mains
SRCEXT		 	= c
SRCDIR			= src
HEADERDIRS		= include/ include/types include/functions libft/include
OBJEXT			= o
BUILDDIR		= obj

LIBS			= libft/libft.a
INCLUDES		= $(HEADERDIRS:%=-I%)
LINK			= -lreadline -lm

ifneq ($(shell uname),Linux)
LINK			+= -L$(HOME)/.brew/opt/readline/lib
INCLUDES		+= -I$(HOME)/.brew/opt/readline/include
endif

#MAINS = $(shell find $(MAIN_DIR) -type f -name *.c)
MAINS = $(MAIN_DIR)/minishell.c
NAME = $(MAINS:$(MAIN_DIR)/%.c=%)

include sources.mk

OBJ_WILDCARD = $(BUILDDIR)/*.$(OBJEXT)
OBJ	= $(foreach src,$(SRC),$(BUILDDIR)/$(notdir $(src:.$(SRCEXT)=.$(OBJEXT))))

VPATH = $(shell find $(SRCDIR) -type d | tr '\n' ':' | sed -E 's/(.*):/\1/')
.SUFFIXES:
SILECE_MAKE = | grep -v -E ".*Leaving directory|.*Entering directory"

all: $(NAME)

$(NAME): %: $(MAIN_DIR)/%.c $(BUILDDIR)/ $(OBJ) $(HEADERDIR)
	$(MAKE) -C libft $(SILECE_MAKE)
	$(CC) $(CFLAGS) $(INCLUDES) $(OBJ_WILDCARD) -o $@ mains/$@.c $(LIBS) $(LINK)

# sources

$(BUILDDIR)/%.$(OBJEXT): %.$(SRCEXT) $(HEADERDIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $(BUILDDIR)/$(notdir $@)

install-dependicy:
	sudo apt-get install libreadline-dev

clean:
ifneq ($(BUILDDIR),.)
	/bin/rm -rf $(BUILDDIR)/
endif

fclean: | clean
	/bin/rm -f $(NAME)

re: | fclean all

$(BUILDDIR)/:
	mkdir -p $(BUILDDIR)

findsources:
	echo "# SRC = \$$(shell find $(SRCDIR) -type f -name '*.c')" >> sources.mk
	echo "SRC = \\" >> sources.mk
	find src/ -name "*.c" -exec echo "{} \\" \; >> sources.mk

silent:
	@$(MAKE) > /dev/null

.PHONY: all clean fclean re findsources silent
