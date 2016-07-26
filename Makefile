# ------------------------------------------------
# Generic Makefile
#
# Author: yanick.rochon@gmail.com
# Date  : 2010-11-05
#
# Changelog :
#   0.01 - first version
# ------------------------------------------------

# change these to set the proper directories where each files should be
SRCDIR   = ./src
OBJDIR   = ./obj
BINDIR   = ./bin
INCDIR   = ./include

# project name (generate executable with this name)
CC       = gcc

# compiling flags here
CFLAGS   = -Wall -I.
LINKER   = $(CC) -o -g

# linking flags here
LFLAGS   = -Wall
LIBS     =

all: dir_create adpcm
	
adpcm: adpcm_obj main_obj
	$(CC) $(OBJDIR)/adpcm.o $(OBJDIR)/main.o -o $(BINDIR)/adpcm

adpcm_obj: $(SRCDIR)/adpcm.c
	$(CC) -I$(INCDIR) -c $(SRCDIR)/adpcm.c -o $(OBJDIR)/adpcm.o

main_obj: $(SRCDIR)/main.c
	$(CC) -I$(INCDIR) -c $(SRCDIR)/main.c -o $(OBJDIR)/main.o

dir_create:
ifeq "$(wildcard $(OBJDIR) )" ""
	mkdir -p $(OBJDIR)
endif
ifeq "$(wildcard $(BINDIR) )" ""
	mkdir -p $(BINDIR)
endif

clean:
	rm -rf $(BINDIR)/adpcm
	rm -rf $(OBJDIR)/*.*