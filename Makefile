CC          	= gcc

#CFLAGS      	= -Wall -Wextra -Wuninitialized -O3
CFLAGS      	= -Wall -Wextra -Werror -Wuninitialized -O3 -Wno-error=unused-result -Wno-unused-result

MAIN_DIR		= mains
SRCEXT      	= c
SRCDIR      	= src
HEADERDIRS		= include/ include/types include/functions libft/include
OBJEXT      	= o
BUILDDIR    	= obj

LIBS			= libft/libft.a
INCLUDES		= $(HEADERDIRS:%=-I%)
LINK			= -lreadline

MAINS = $(shell find $(MAIN_DIR) -type f -name *.c)
NAMES = $(MAINS:$(MAIN_DIR)/%.c=%)

include sources.mk

OBJ	= $(foreach src,$(SRC),$(BUILDDIR)/$(notdir $(src:.$(SRCEXT)=.$(OBJEXT))))

VPATH = $(shell find $(SRCDIR) -type d | tr '\n' ':' | sed -E 's/(.*):/\1/')
.SUFFIXES:
SILECE_MAKE = | grep -v -E ".*Leaving directory|.*Entering directory"

all: $(NAMES)

$(NAMES): %: $(MAIN_DIR)/%.c $(BUILDDIR)/ $(OBJ) $(HEADERDIR) $(SETTINGS)
	$(MAKE) -C libft $(SILECE_MAKE)
	$(CC) $(CFLAGS) $(INCLUDES) $(BUILDDIR)/*.$(OBJEXT) $(LIBS) -o $@ $\
	mains/$@.c $(LINK)

# sources

$(BUILDDIR)/%.$(OBJEXT): %.$(SRCEXT) $(HEADERDIR) $(SETTINGS)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $(BUILDDIR)/$(notdir $@) $(LINK)

clean:
ifneq ($(BUILDDIR),.)
	/bin/rm -rf $(BUILDDIR)/
endif

fclean: clean
	/bin/rm -f $(NAMES)

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
