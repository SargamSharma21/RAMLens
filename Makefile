CC = gcc
CFLAGS = -Wall -Wextra -std=c11

TARGET = ramlens

SRC = src/main.c src/memory.c
INCLUDES = -Iinclude

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(INCLUDES) $(SRC) -o $(TARGET)

clean:
	rm -f $(TARGET)