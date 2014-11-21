TARGET=proj
SOURCE=$(wildcard src/*.c)
SRCDIR=src
BUILDDIR=build
NSRC=$(shell echo "$(SOURCE)" | wc | sed -e 's/\ \ */,/g' | cut -d "," -f 3)
NthSRC=0
HEADER=$(wildcard src/*.h)
OBJECT=$(patsubst src/%.c,build/%.o,$(SOURCE))
CFLAGS=-std=c99 -Wall -Wextra -pedantic -DDEBUG
LD=$(CC)
CERR=$(CC).err
DOXCONF=config.dox

.PHONY: init clean doc help

all: init $(TARGET)

init:
	+@[ -d $(BUILDDIR) ] || mkdir -p $(BUILDDIR)

clean:
	@echo "Cleaning working directory ..."
	@rm -rf $(BUILDDIR) $(TARGET) html latex

$(BUILDDIR)/%.o: $(SRCDIR)/%.c
	$(eval NthSRC=$(shell echo $(NthSRC)+1 | bc))
	$(eval PERCENT=$(shell echo 100\*$(NthSRC)/$(NSRC) | bc))
	@echo -e "\e[34mCC\e[39m \e[32m\e[1mBuilding\e[21m\e[39m ... \e[31m$(PERCENT)%\e[39m: 	$(CC) $(CFLAGS) -o $@ -c $<"
	@$(CC)	$(CFLAGS)	-o $@	-c $<
	@$(CC) $^ -MM > $@.d

$(TARGET): $(OBJECT)
	@echo -e  "\e[34mLD\e[39m \e[32m\e[1mLinking\e[21m\e[39m  ... \e[31m100%\e[39m: 	$(LD) $(LDFLAGS) $^ -o $@"
	@$(LD) $(LDFLAGS)	$^ -o $@ 

doc:
	@echo -e "DOXYGEN \e[32m\e[1mBuilding documentation\e[21m\e[39m ..."
	@doxygen $(DOXCONF)

-include  $(patsubst %.o,%.o.d,$(OBJECT))
