
NAME      		= minishell

CC          	= gcc

#CFLAGS      	= -Wall -Wextra -Wuninitialized -O3
CFLAGS      	= -Wall -Wextra -Werror -Wuninitialized -O3 -Wno-error=unused-result

SRCEXT      	= c
SRCDIR      	= src
HEADERDIRS		= include/ include/types include/functions libft/include
OBJEXT      	= o
BUILDDIR    	= obj

LIBS			= libft/libft.a
INCLUDES		= $(HEADERDIRS:%=-I%)
LINK			=

include sources.mk

OBJ	= $(foreach src,$(SRC),$(BUILDDIR)/$(notdir $(src:.$(SRCEXT)=.$(OBJEXT))))

VPATH = $(shell find $(SRCDIR) -type d | tr '\n' ':' | sed -E 's/(.*):/\1/')
.SUFFIXES:
SILECE_MAKE = | grep -v -E ".*Leaving directory|.*Entering directory"

all: $(NAME)

$(NAME): $(BUILDDIR)/ $(OBJ) $(HEADERDIR) $(SETTINGS)
	$(MAKE) -C libft $(SILECE_MAKE)
	$(CC) $(CFLAGS) $(INCLUDES) $(BUILDDIR)/*.$(OBJEXT) $(LIBS) -o $(NAME) $\
	$(LINK)

# sources

$(BUILDDIR)/%.$(OBJEXT): %.$(SRCEXT) $(HEADERDIR) $(SETTINGS)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $(BUILDDIR)/$(notdir $@) $(LINK)

clean:
ifneq ($(BUILDDIR),.)
	/bin/rm -rf $(BUILDDIR)/
endif

fclean:
	$(MAKE) clean $(SILECE_MAKE)
	/bin/rm -f $(NAME)

re:
	$(MAKE) fclean $(SILECE_MAKE)
	$(MAKE) all $(SILECE_MAKE)

$(BUILDDIR)/:
	mkdir -p $(BUILDDIR)

findsources:
	echo "# SRC = \$$(shell find $(SRCDIR) -type f -name '*.c')" >> sources.mk
	echo "SRC = \\" >> sources.mk
	find src/ -name "*.c" -exec echo "{} \\" \; >> sources.mk

silent:
	@$(MAKE) > /dev/null

.PHONY: all clean fclean re findsources silent
