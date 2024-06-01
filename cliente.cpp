#include <iostream>
#include <cstring>
#include <arpa/inet.h>
#include <unistd.h>

void printBoard(char board[6][7]) {
    for(int i = 0; i < 6; ++i) {
        for(int j = 0; j < 7; ++j) {
            std::cout << board[i][j] << " ";
        }
        std::cout << std::endl;
    }
}

int main(int argc, char const *argv[]) {
    if (argc != 3) {
        std::cerr << "Uso: " << argv[0] << " <direccion_ip> <puerto>\n";
        return 1;
    }

    const char* server_ip = argv[1];
    int port = atoi(argv[2]);

    struct sockaddr_in serv_addr;
    int sock = 0, valread;
    char buffer[1024] = {0};
    char board[6][7];

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        std::cout << "\nError al crear el socket\n";
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);

    if (inet_pton(AF_INET, server_ip, &serv_addr.sin_addr) <= 0) {
        std::cout << "\nDirección IP inválida o no soportada\n";
        return -1;
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        std::cout << "\nConexión fallida\n";
        return -1;
    }

    while (true) {
        valread = read(sock, board, sizeof(board));
        printBoard(board);

        std::cout << "Ingrese columna (0-6): ";
        int col;
        std::cin >> col;
        send(sock, std::to_string(col).c_str(), sizeof(std::to_string(col).c_str()), 0
