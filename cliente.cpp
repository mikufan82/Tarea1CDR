#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
#include <cstdlib>
#include <cstring>

using namespace std;

int main(int argc, char *argv[]) {
    if (argc != 3) {
        cerr << "Uso: " << argv[0] << " <direccion IP> <puerto>" << endl;
        return 1;
    }

    const char *server_ip = argv[1];
    int port = atoi(argv[2]);
    int sock = 0;
    struct sockaddr_in serv_addr;

    // Crear el socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        cerr << "Error creando el socket: " << strerror(errno) << endl;
        return 1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);

    // Convertir la dirección IP
    if (inet_pton(AF_INET, server_ip, &serv_addr.sin_addr) <= 0) {
        cerr << "Dirección IP inválida o no soportada: " << server_ip << endl;
        return 1;
    }

    // Conectar al servidor
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        cerr << "Conexión fallida: " << strerror(errno) << endl;
        return 1;
    }

    cout << "Conectado al servidor " << server_ip << " en el puerto " << port << endl;

    char buffer[1024] = {0};
    while (true) {
        // Recibir mensaje del servidor
        int valread = read(sock, buffer, 1024);
        if (valread <= 0) {
            cerr << "Conexión cerrada por el servidor o error al recibir datos" << endl;
            break;
        }

        cout << buffer << endl;

        // Enviar columna elegida al servidor
        if (strstr(buffer, "Elija una columna (1-7):")) {
            string input;
            cout << "Ingrese la columna: ";
            cin >> input;
            send(sock, input.c_str(), input.length(), 0);
        }

        memset(buffer, 0, sizeof(buffer));
    }

    close(sock);
    return 0;
}
