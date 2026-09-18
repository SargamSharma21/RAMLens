CC = gcc
CFLAGS = -Wall -Wextra -std=c11

TARGET = ramlens

SRC = src/main.c

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

clean:
	rm -f $(TARGET)