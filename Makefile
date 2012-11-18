# Common tools
ECHO=echo
RM=rm -rf
MV=mv
MKDIR=mkdir
PROMPT="> "
DOXY=doxygen
CC=gcc

CROSS_COMPILE:=

# Define directories and source files
LIBSVG2_DIR:=$(CURDIR)/sources
EXAMPLES_DIR:=$(CURDIR)/examples
BINDIR:=$(CURDIR)/bin
OBJDIR:=$(CURDIR)/objs
INCDIR_BASE:=$(CURDIR)/include
INCDIR:=${shell find $(INCDIR_BASE) -type d  -print | grep -v .svn }
DOCSDIR:=$(CURDIR)/docs
AUTODIRS:=$(BINDIR) $(OBJDIR) $(DOCSDIR)
LIBSVG2_AFILE:=$(BINDIR)/libsvg2.a


# include directories
CFLAGS:=$(CFLAGS) -I$(INCDIR) $(shell xml2-config --cflags) 

# Linker options
LDFLAGS:=-L$(BINDIR) $(shell xml2-config --libs) 

# Adds flags to CFLAGS
ifdef NDEBUG
CFLAGS:=$(CFLAGS) -Wall -Wextra -Wconversion -O3 -DNDEBUG
else
CFLAGS:=$(CFLAGS) -Wall -Wextra -Wconversion -O0 -g -rdynamic
endif

export

.PHONY: all help libsvg2 examples docs clean mrproper directories directories_pre directories_post

all: directories libsvg2 examples

libsvg2: directories
	@$(MAKE) -C $(LIBSVG2_DIR)

examples: directories libsvg2
	@$(MAKE) -C $(EXAMPLES_DIR)

clean:
	@$(MAKE) -C $(LIBSVG2_DIR) $@
	@$(MAKE) -C $(EXAMPLES_DIR) $@

mrproper:
	@$(MAKE) -C $(LIBSVG2_DIR) $@
	@$(MAKE) -C $(EXAMPLES_DIR) $@
	-@$(RM) $(BINDIR)
	-@$(RM) $(OBJDIR)
	-@$(RM) $(DOCSDIR)

directories: directories_pre $(AUTODIRS) directories_post

directories_pre:
	@$(ECHO) -- Checking directories --

directories_post:
	@$(ECHO) -- directories done --

$(AUTODIRS):
	@$(ECHO) -n $(PROMPT)
	$(MKDIR) -p $@

docs:
	@$(DOXY) ./libsvg2.doxyfile

help:
	@$(ECHO) "Rules"
	@$(ECHO) "-----"
	@$(ECHO) "all       Build all rules but documentation."
	@$(ECHO) "clean     Delete objects files."
	@$(ECHO) "docs      Build documentation."
	@$(ECHO) "examples  Compile examples."
	@$(ECHO) "libsvg2   Compile libsvg2 library."
	@$(ECHO) "help      Display this page."
	@$(ECHO) "mrproper  Perform a clean and delete binary files as well."
	@$(ECHO) ""
	@$(ECHO) "Variables"
	@$(ECHO) "---------"
	@$(ECHO) "CROSS_COMPILE  Compiler prefix. (eg. sh4-linux-)."

