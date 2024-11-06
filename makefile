# Compiler and flags
CC = gcc
CFLAGS = -Wall

# Targets and sources
ASSEMBLER = assembler
SIMULATOR = simulator
ASSEMBLER_SRC = assembler.c
SIMULATOR_SRC = simulator.c

# Default target
all: $(ASSEMBLER) $(SIMULATOR)

# Run assembler or simulator with a file
assemble: $(ASSEMBLER)
	./$(ASSEMBLER) $(FILENAME)

simulate: $(SIMULATOR)
	./$(SIMULATOR) $(FILENAME)

# Compile targets
$(ASSEMBLER): $(ASSEMBLER_SRC)
	$(CC) $(CFLAGS) -o $@ $^

$(SIMULATOR): $(SIMULATOR_SRC)
	$(CC) $(CFLAGS) -o $@ $^

# Clean up
clean:
	rm -f $(ASSEMBLER) $(SIMULATOR)

# Usage help
help:
	@echo "Make file:"
	@echo "  make all                       - Compile both assembler and simulator"
	@echo "  make assemble FILENAME=filename.asm - Run assembler on specified file"
	@echo "  make simulate FILENAME=filename.bin - Run simulator on specified file"
	@echo "  make clean                     - Remove executables"
