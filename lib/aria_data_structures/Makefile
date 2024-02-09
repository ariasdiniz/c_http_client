LIBNAME    = libaria.so
SRCDIR     = $(.CURDIR)/src
INCDIR     = $(.CURDIR)/include
BUILDDIR   = $(.CURDIR)/build
OBJDIR     = $(BUILDDIR)/obj
CFILES     = $(SRCDIR)/hash/*.c $(SRCDIR)/linkedlist/*.c

CC = cc
CFLAGS = -I$(INCDIR) -Wall -Wextra -pedantic

##
# Public
$(LIBNAME): clean obj
	@mkdir -p $(OBJDIR) $(LIBDIR)
	$(CC) -shared $(OBJDIR)/*.o -o $(BUILDDIR)/$(LIBNAME)

##
# Private
obj:
	@for src in $(CFILES); do \
		obj=$$(basename $$(echo "$${src}" | sed 's/\.c$$/\.o/')); \
		$(CC) $(CFLAGS) -c "$${src}" -o "${OBJDIR}/$${obj}" ; \
	done

clean:
	@rm -rf $(BUILDDIR)/*
	@mkdir -p $(BUILDDIR) $(LIBDIR) $(OBJDIR)
