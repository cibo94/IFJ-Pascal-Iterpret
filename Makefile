# Project Name #
TARGET=proj
# Source dir #
SRCDIR=src
# Build dir #
BUILDDIR=build
# Source ignore list #
IGNORE=
# C compiler #
CC=gcc
# C compiler flags #
CFLAGS=-std=c99 -Wall -Wextra -pedantic -DDEBUG
# Linker #
LD=$(CC)
# Linker flags #
LDFLAGS=
# Doxygen config file #
DOXCONF=config.dox


#####################################
# 			DO NOT TOUCH			#
#####################################
SOURCE=$(filter-out $(IGNORE),$(wildcard $(SRCDIR)/*.c))
NSRC=$(shell echo "$(SOURCE)" | wc | sed -e 's/\ \ */,/g' | cut -d "," -f 3)
NthSRC=0
HEADER=$(wildcard $(SRCDIR)/*.h)
OBJECT=$(patsubst $(SRCDIR)/%.c,$(BUILDDIR)/%.o,$(SOURCE))
CERR=$(CC).err
.PHONY: clean doc help

$(TARGET): $(OBJECT)
	@bash -c "echo -e \"\e[34mLD\e[39m \e[32m\e[1mLinking \e[21m\e[39m ... \e[31m100%\e[39m: 	$(LD) $(LDFLAGS) $^ -o $@\""
	@$(LD) $(LDFLAGS)	$^ -o $@ 

$(BUILDDIR)/%.o: $(SRCDIR)/%.c
	+@[ -d $(BUILDDIR) ] || mkdir -p $(BUILDDIR)
	$(eval NthSRC=$(shell echo $(NthSRC)+1 | bc))
	$(eval PERCENT=$(shell echo 100\*$(NthSRC)/$(NSRC) | bc))
	@bash -c "echo -e \"\e[39m\e[34mCC\e[39m \e[32m\e[1mBuilding\e[21m\e[39m ... \e[31m$(PERCENT)%\e[39m: 	$(CC) $(CFLAGS) -o $@ -c $<\""
	@$(CC)	$(CFLAGS)	-o $@	-c $<
	@$(CC) $^ -MM > $@.d

-include  $(patsubst %.o,%.o.d,$(OBJECT))

clean:
	@echo "Cleaning working directory ..."
	@rm -rf $(OBJECT) $(patsubst %.o,%.o.d,$(OBJECT)) $(TARGET) html latex
	@bash -c "echo -e \"\e[31m $(OBJECT) $(patsubst %.o,%.o.d,$(OBJECT)) $(TARGET)\e[39m\" | sed \"s/\ /\n\tremoving\: /g\""

doc:
	@bash -c "echo -e \"\e[34mDOXYGEN\e[39m \e[32m\e[1mBuilding documentation\e[21m\e[39m ...\""
	@doxygen $(DOXCONF)
