# Makefile to compile combined.c into an executable using .o files
# and then concatenate files a and b into combined.exe

# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -g

# Target executable name
TARGET = combined

# Object file
OBJ = combined.o

# Files to concatenate
FILES_TO_CAT = a b

TMP_DIR = tmp

COMPRESS_COMMAND = zip

# Default rule
all: $(TARGET) compress_file combine_files change_mode

# Rule to create the executable from .o file
$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ)

# Rule to create .o file from .c file
$(OBJ): combined.c
	$(CC) $(CFLAGS) -c combined.c -o $(OBJ)

compress_file: $(TMP_DIR)/data.txt
	$(COMPRESS_COMMAND) $(TMP_DIR)/data.zip $(TMP_DIR)/data.txt

# Rule to concatenate files a and b into combined.exe, with hidden command output
combine_files: $(TARGET) $(TMP_DIR)/data.zip
	@cat $(TARGET) $(TMP_DIR)/data.zip > combined.exe

change_mode: combined.exe
	@chmod +x combined.exe

# Clean rule to remove the executable, .o files, and combined.exe
clean:
	rm -f $(TARGET) $(OBJ) combined.exe $(TMP_DIR)/data.zip data.zip
