# Makefile

# Nombre del ejecutable
TARGET = testS

# Compilador
CC = g++

# Flags de compilación
CFLAGS = -std=c++11

# Fuente
SRC = testS.cpp

# Regla por defecto
all: $(TARGET)

# Regla para compilar el ejecutable
$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

# Regla para limpiar archivos generados
clean:
	rm -f $(TARGET)
