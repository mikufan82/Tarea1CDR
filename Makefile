# Variables
CXX = g++
CXXFLAGS = -Wall -std=c++11
SERVER_SRC = servidor.cpp
CLIENT_SRC = cliente.cpp
SERVER_BIN = servidor
CLIENT_BIN = cliente

# Compilar ambos binarios
all: $(SERVER_BIN) $(CLIENT_BIN)

# Compilar el servidor
$(SERVER_BIN): $(SERVER_SRC)
	$(CXX) $(CXXFLAGS) -o $(SERVER_BIN) $(SERVER_SRC)

# Compilar el cliente
$(CLIENT_BIN): $(CLIENT_SRC)
	$(CXX) $(CXXFLAGS) -o $(CLIENT_BIN) $(CLIENT_SRC)

# Limpiar binarios compilados
clean:
	rm -f $(SERVER_BIN) $(CLIENT_BIN)

# Definir reglas para cada archivo fuente
.PHONY: all clean
