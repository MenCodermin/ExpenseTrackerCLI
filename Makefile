########################################################################
####################### Makefile Template ##############################
########################################################################

# Compiler settings - Can be customized.
CC = g++
CXXFLAGS = -std=c++17 -Wall -Wextra
LDFLAGS = -lstdc++fs

# Makefile settings - Can be customized.
APPNAME = myapp
EXT = .cpp
SRCDIR = src
OBJDIR = obj

############## Do not change anything from here downwards! #############
SRC = $(wildcard $(SRCDIR)/*$(EXT))
OBJ = $(SRC:$(SRCDIR)/%$(EXT)=$(OBJDIR)/%.o)
DEP = $(OBJ:$(OBJDIR)/%.o=%.d)

# OS-specific settings
RM = rm -f
DEL = del
EXE = .exe
ifeq ($(OS),Windows_NT)
    CLEAN_CMD = $(DEL)
    APP_EXT = $(EXE)
else
    CLEAN_CMD = $(RM)
    APP_EXT = 
endif

########################################################################
####################### Targets beginning here #########################
########################################################################

all: $(APPNAME)

# Builds the app
$(APPNAME): $(OBJ)
	$(CC) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

# Ensure object directory exists
$(OBJDIR):
	mkdir -p $(OBJDIR)

# Creates the dependency rules
%.d: $(SRCDIR)/%$(EXT)
	@$(CC) $(CXXFLAGS) $< -MM -MT $(@:%.d=$(OBJDIR)/%.o) >$@

# Includes all .h files
-include $(DEP)

# Building rule for .o files and its .c/.cpp in combination with all .h
$(OBJDIR)/%.o: $(SRCDIR)/%$(EXT) | $(OBJDIR)
	$(CC) $(CXXFLAGS) -o $@ -c $<

################### Cleaning rules ###################
# Cleans complete project
.PHONY: clean
clean:
ifeq ($(OS),Windows_NT)
	$(CLEAN_CMD) $(subst /,\,$(OBJDIR)\*.o)
	$(CLEAN_CMD) $(subst /,\,$(OBJDIR)\*.d)
	$(CLEAN_CMD) $(APPNAME)$(APP_EXT)
else
	$(CLEAN_CMD) $(OBJDIR)/*.o $(OBJDIR)/*.d $(APPNAME)$(APP_EXT)
endif

# Cleans only all files with the extension .d
.PHONY: cleandep
cleandep:
ifeq ($(OS),Windows_NT)
	$(CLEAN_CMD) $(subst /,\,$(OBJDIR)\*.d)
else
	$(CLEAN_CMD) $(OBJDIR)/*.d
endif
