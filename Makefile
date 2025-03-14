# Name: Makefile
# Project: B15F (board15 Famulus Edition)
# Author: Tristan Krause
# Creation Date: 2019-05-15

# Environment
COMPILER_PATH = g++

# Options
CFLAGS   = -std=c++17 -O3 -Wall -Wextra
LDFLAGS  = -lb15fdrv
OBJECTS  = b15fPC.o DataBlock.o CRC.o Sender.o MainSender.o SideSender.o
OUT      = b15fPC.elf

COMPILE = $(COMPILER_PATH) $(CFLAGS)

main: $(OBJECTS)
	$(COMPILE) $(OBJECTS) -o $(OUT) $(LDFLAGS)

help:
	@echo "This Makefile has the following targets:"
	@echo "make main .... to compile"
	@echo "make clean ... to delete objects and executables"
	
clean:
	@echo "Cleaning..."
	rm -f $(OBJECTS) $(OUT) *.bin gnuplotscript.gp

.cpp.o:
	$(COMPILE) -c $< -o $@
