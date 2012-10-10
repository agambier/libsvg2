# Define directories
LIBXML2_OBJDIR:=$(OBJDIR)/libxml2
AUTODIRS:=$(LIBXML2_OBJDIR)

# Define source files and object files
CFILES:=${shell find ${CURDIR} -name "*.c" -print}
OFILES:=$(patsubst %.c,%.o,$(CFILES))
OFILES:=$(notdir $(OFILES))
OFILES:=$(addprefix $(LIBXML2_OBJDIR)/,$(OFILES))
CFLAGS:=$(CFLAGS)

# Define sub-directories
VPATH:=$(dir $(CFILES)) $(INCDIR)

.PHONY: all clean mrproper libxml2_pre libxml2_post directories directories_pre directories_post

all: directories libxml2_pre $(LIBXML2_AFILE) libxml2_post

libxml2_pre: directories
	@$(ECHO) "-- Building $(notdir $(LIBXML2_AFILE)) --"
	@$(ECHO) -n $(PROMPT)

libxml2_post: $(LIBXML2_AFILE)
	@$(ECHO) "-- $(notdir $(LIBXML2_AFILE)) done --"

$(LIBXML2_AFILE): $(OFILES)
	@$(ECHO) -n $(PROMPT)"("$(notdir $(LIBXML2_AFILE))") "
	$(CROSS_COMPILE)$(AR) rcs $(LIBXML2_AFILE) $(OFILES)

$(LIBXML2_OBJDIR)/%.o: %.c %.h libxml2_pre
	@$(ECHO) -n $(PROMPT)"("$(notdir $<)") "
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(addprefix -I,$(INCDIR)) -c $< -o $@ 
	
clean:
	@$(RM) $(LIBXML2_OBJDIR)

mrproper: clean
	@$(RM) $(LIBXML2_AFILE)

directories: directories_pre $(AUTODIRS) directories_post

directories_pre:
	@$(ECHO) -- Checking directories --

directories_post: $(AUTODIRS)
	@$(ECHO) -- directories done --
	
$(AUTODIRS): directories_pre
	@$(ECHO) -n $(PROMPT)
	$(MKDIR) -p $@
