# http://www.nongnu.org/avr-libc/user-manual/group__demo__project.html
# Modified by zhiyb (Yubo Zhi) - For Qt

PRG	= OTG
SRC	= main.c mem.c
SUBDIRS = 
VERSION	= 0.0.1

CROSS	= arm-linux-gnueabi
OPTIMIZE	= -O3
STRIP	= false
DEFS	= -Wall -Werror
LIBS	= 

# You should not have to change anything below here.

MOCC	= $(addprefix moc_,$(subst .h,.cpp,$(MOCH)))
OBJ	= $(subst .c,.o,$(subst .cpp,.o,$(SRC) $(MOCC)))

ifdef CROSS
CC	= $(CROSS)-g++
LN	= $(CROSS)-g++
STRIP	+= && $(CROSS)-strip
else
CC	= g++
LN	= g++
STRIP	+= && strip
endif
MOC	= moc

CFLAGS	= -g -c $(DEFS) $(OPTIMIZE)
LDFLAGS	= -g $(LIBS)

all: subdirs $(PRG)

$(PRG): $(OBJ)
	$(LN) $(LDFLAGS) -o $@ $^
	$(STRIP) $@ || true

%.o: %.cpp
	$(CC) $(CFLAGS) -o $@ $<

# Subdirectories

.PHONY: subdirs $(SUBDIRS)
subdirs: $(SUBDIRS)

$(SUBDIRS):
	$(MAKE) -C $@

# Dependency:

-include $(OBJ:.o=.d)

%.d: %.cpp
	@set -e; rm -f $@; \
	$(CC) -MM $(CFLAGS) $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$

%.d: %.c
	@set -e; rm -f $@; \
	$(CC) -MM $(CFLAGS) $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$

# Clean up

.PHONY: clean
clean:
	for d in $(SUBDIRS); do (cd $$d; $(MAKE) clean); done
	rm -rf $(OBJ) $(MOCC) *.d *.d.* $(PRG) $(EXTRA_CLEAN_FILES)

# Publish: source code & hex target archive

TAR	= tar
TARFLAGS	= -jc
ARCHIVE	= $(PRG)-$(VERSION).tar.bz2
EXTRA_CLEAN_FILES	+= $(ARCHIVE)

.PHONY: archive
archive: $(ARCHIVE)

$(ARCHIVE): $(PRG)
	$(TAR) $(TARFLAGS) -f $(ARCHIVE) $(SRC) $(MOCH) *.h $(PRG) Makefile
