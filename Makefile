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
	+@[ -d $(BUILDDIR) ] && mkdir -p $(BUILDDIR)

clean:
	@echo "Cleaning working directory ..."
	@rm -rf $(BUILDDIR) $(TARGET) html latex

$(BUILDDIR)/%.o: $(SRCDIR)/%.c
	$(eval NthSRC=$(shell echo $(NthSRC)+1 | bc))
	$(eval PERCENT=$(shell echo 100\*$(NthSRC)/$(NSRC) | bc))
	@echo "Building ... $(PERCENT)%: 	$(CC) $(CFLAGS) -o $@ -c $<"
	@$(CC)	$(CFLAGS)	-o $@	-c $<
	@$(CC) $^ -MM > $@.d

$(TARGET): $(OBJECT)
	@echo "Linking  ... 100%: 	$(LD) $(LDFLAGS) $^ -o $@"
	@$(LD) $(LDFLAGS)	$^ -o $@ 

doc:
	@echo "Building documentation ..."
	@doxygen $(DOXCONF)

-include  $(patsubst %.o,%.o.d,$(OBJECT))
