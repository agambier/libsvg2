# Define directories
EXAMPLES_OBJDIR:=$(OBJDIR)/examples
AUTODIRS:=$(EXAMPLES_OBJDIR)

# Define source files and object files
CFILES:=${shell find ${CURDIR} -name "*.c" -print}
OFILES:=$(patsubst %.c,%.o,$(CFILES))
OFILES:=$(notdir $(OFILES))
OFILES:=$(addprefix $(EXAMPLES_OBJDIR)/,$(OFILES))
OUTFILES:=$(patsubst %.c,%,$(CFILES))
OUTFILES:=$(notdir $(OUTFILES))
OUTFILES:=$(addprefix $(BINDIR)/,$(OUTFILES))
CFLAGS:=$(CFLAGS)
LDFLAGS:=-lsvg2 $(LDFLAGS) -L$(BINDIR)
# Define sub-directories
VPATH:=$(dir $(CFILES)) $(INCDIR)

.PHONY: all clean mrproper examples_pre examples_post directories directories_pre directories_post

all: directories examples_pre $(OUTFILES) examples_post

examples_pre:
	@$(ECHO) "-- Building examples --"

examples_post: $(OUTFILES)
	@$(ECHO) "-- examples done --"

$(BINDIR)/%: %.c $(LIBSVG2_AFILE)
	@$(ECHO) -n $(PROMPT)"("$(notdir $<)" 1/2) "
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(addprefix -I,$(INCDIR)) -c $< -o $(EXAMPLES_OBJDIR)/$(notdir $@).o
	@$(ECHO) -n $(PROMPT)"("$(notdir $<)" 2/2) "
	$(CROSS_COMPILE)$(CC) $(EXAMPLES_OBJDIR)/$(notdir $@).o $(LDFLAGS) -o $@


clean:
	@$(RM) $(EXAMPLES_OBJDIR)

mrproper: clean
	@$(RM) $(OUTFILES)

directories: directories_pre $(AUTODIRS) directories_post

directories_pre:
	@$(ECHO) -- Checking directories --

directories_post:
	@$(ECHO) -- directories done --

$(AUTODIRS):
	@$(ECHO) -n $(PROMPT)
	$(MKDIR) -p $@
