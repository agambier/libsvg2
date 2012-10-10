# Define directories
LIBSVG2_OBJDIR:=$(OBJDIR)/libsvg2
AUTODIRS:=$(LIBSVG2_OBJDIR)

# Define source files and object files
CFILES:=${shell find ${CURDIR} -name "*.c" -print}
OFILES:=$(patsubst %.c,%.o,$(CFILES))
OFILES:=$(notdir $(OFILES))
OFILES:=$(addprefix $(LIBSVG2_OBJDIR)/,$(OFILES))
CFLAGS:=$(CFLAGS)

# Define sub-directories
VPATH:=$(dir $(CFILES)) $(INCDIR)

.PHONY: all clean mrproper libsvg2_pre libsvg2_post directories directories_pre directories_post

all: directories libsvg2_pre $(LIBSVG2_AFILE) libsvg2_post

libsvg2_pre: directories
	@$(ECHO) "-- Building $(notdir $(LIBSVG2_AFILE)) --"
	@$(ECHO) -n $(PROMPT)

libsvg2_post: $(LIBSVG2_AFILE)
	@$(ECHO) "-- $(notdir $(LIBSVG2_AFILE)) done --"

$(LIBSVG2_AFILE): $(OFILES)
	@$(ECHO) -n $(PROMPT)"("$(notdir $(LIBSVG2_AFILE))") "
	$(CROSS_COMPILE)$(AR) rcs $(LIBSVG2_AFILE) $(OFILES)

$(LIBSVG2_OBJDIR)/%.o: %.c %.h libsvg2_pre
	@$(ECHO) -n $(PROMPT)"("$(notdir $<)") "
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(addprefix -I,$(INCDIR)) -c $< -o $@ 
	
clean:
	@$(RM) $(LIBSVG2_OBJDIR)

mrproper: clean
	@$(RM) $(LIBSVG2_AFILE)

directories: directories_pre $(AUTODIRS) directories_post

directories_pre:
	@$(ECHO) -- Checking directories --

directories_post: $(AUTODIRS)
	@$(ECHO) -- directories done --
	
$(AUTODIRS): directories_pre
	@$(ECHO) -n $(PROMPT)
	$(MKDIR) -p $@
