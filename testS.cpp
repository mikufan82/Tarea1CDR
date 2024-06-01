#include <sys/socket.h> // socket()
#include <arpa/inet.h>  // hton*()
#include <string.h>     // memset()
#include <unistd.h>
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <ctime>

using namespace std;

// Funciones del juego
void JugarPartida(int socket_cliente);
void LimpiarTablero();
void MostrarTablero(int socket_cliente);
void UbicarFicha(int col, char turno);
bool RevisarHorizontal(int, int, char);
bool RevisarVertical(int, int, char);
bool RevisarDiagonal(int, int, char);
bool RevisarDiagonalInvertida(int, int, char);
int MovimientoServidor();

// Tablero de 8x8
char tablero[8][8] = {
    {'1', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
    {'2', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
    {'3', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
    {'4', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
    {'5', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
    {'6', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
    {'-', '-', '-', '-', '-', '-', '-', '-'},
    {' ', '1', '2', '3', '4', '5', '6', '7'}
};

// Variables globales
char turno = 'C';
int indD;
int colD;
bool listo = false;

// Función principal del servidor
int main(int argc, char **argv) {
    if (argc != 2) {
        cerr << "Uso: " << argv[0] << " <puerto>" << endl;
        return 1;
    }

    int port = atoi(argv[1]);
    int socket_server = 0;
    struct sockaddr_in direccionServidor, direccionCliente;

    // Crear el socket
    cout << "Creating listening socket ...\n";
    if ((socket_server = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        cerr << "Error creating listening socket: " << strerror(errno) << endl;
        exit(EXIT_FAILURE);
    }

    // Configurar los atributos de la estructura sockaddr_in
    cout << "Configuring socket address structure ...\n";
    memset(&direccionServidor, 0, sizeof(direccionServidor));
    direccionServidor.sin_family = AF_INET;
    direccionServidor.sin_addr.s_addr = htonl(INADDR_ANY);
    direccionServidor.sin_port = htons(port);

    // Enlazar el socket
    cout << "Binding socket ...\n";
    if (bind(socket_server, (struct sockaddr *) &direccionServidor, sizeof(direccionServidor)) < 0) {
        cerr << "Error calling bind(): " << strerror(errno) << endl;
        exit(EXIT_FAILURE);
    }

    // Poner el socket en modo de escucha
    cout << "Calling listening ...\n";
    if (listen(socket_server, 1024) < 0) {
        cerr << "Error calling listen(): " << strerror(errno) << endl;
        exit(EXIT_FAILURE);
    }

    cout << "Waiting client request ...\n";
    socklen_t addr_size = sizeof(struct sockaddr_in);

    // Bucle principal para aceptar conexiones y manejar clientes
    while (true) {
        int socket_cliente;
        if ((socket_cliente = accept(socket_server, (struct sockaddr *)&direccionCliente, &addr_size)) < 0) {
            cerr << "Error calling accept(): " << strerror(errno) << endl;
            exit(EXIT_FAILURE);
        }

        if (fork() == 0) {
            // Proceso hijo para manejar al cliente
            close(socket_server);
            JugarPartida(socket_cliente);
            close(socket_cliente);
            exit(0);
        } else {
            // Proceso padre cierra el socket del cliente
            close(socket_cliente);
        }
    }

    close(socket_server);
    return 0;
}

// Inicia el juego con el cliente
void JugarPartida(int socket_cliente) {
    LimpiarTablero();

    while (true) {
        MostrarTablero(socket_cliente);
        if (RevisarHorizontal(indD, colD, turno) || RevisarVertical(indD, colD, turno) || RevisarDiagonal(indD, colD, turno) || RevisarDiagonalInvertida(indD, colD, turno)) {
            string mensaje = "¡Gano el jugador " + string(1, turno) + "!\n";
            send(socket_cliente, mensaje.c_str(), mensaje.length(), 0);
            break;
        }

        if (turno == 'C') {
            turno = 'S';
            int colT = MovimientoServidor();
            colD = colT;
            UbicarFicha(colD, turno);
        } else {
            turno = 'C';
            string mensaje = "\nTurno de " + string(1, turno) + ". Elija una columna (1-7): ";
            send(socket_cliente, mensaje.c_str(), mensaje.length(), 0);

            char buffer[2];
            recv(socket_cliente, buffer, 2, 0);
            int colT = buffer[0] - '0';

            if (colT < 1 || colT > 7) {
                mensaje = "Columna Inexistente.\n";
                send(socket_cliente, mensaje.c_str(), mensaje.length(), 0);
                turno = 'S';
            } else {
                colD = colT;
                UbicarFicha(colD, turno);
            }
        }
    }
}

// Limpia el tablero
void LimpiarTablero() {
    for (int i = 0; i < 6; i++) {
        for (int j = 1; j < 8; j++) {
            tablero[i][j] = ' ';
        }
    }
}

// Muestra el tablero al cliente
void MostrarTablero(int socket_cliente) {
    stringstream ss;
    ss << "\t:::CUATRO EN LINEA:::" << endl;
    for (int i = 0; i < 8; i++) {
        ss << "\t";
        for (int j = 0; j < 8; j++) {
            ss << tablero[i][j] << " ";
        }
        ss << endl;
    }
    string tablero_str = ss.str();
    send(socket_cliente, tablero_str.c_str(), tablero_str.length(), 0);
}

// Coloca una ficha en la columna elegida
void UbicarFicha(int col, char turno) {
    for (int i = 5; i >= 0; i--) {
        if (tablero[i][col] == ' ') {
            tablero[i][col] = turno;
            indD = i;
            break;
        }
    }
}

// Genera el movimiento del servidor de manera aleatoria
int MovimientoServidor() {
    srand(time(0));
    int col;
    do {
        col = rand() % 7 + 1; // Genera un número entre 1 y 7
    } while (tablero[0][col] != ' '); // Verifica si la columna no está llena
    return col;
}

// Revisa si hay 4 en línea horizontalmente
bool RevisarHorizontal(int i, int j, char t) {
    int cont = 1;
    int h = j;
    bool listo = false;
    // Izquierda
    while (!listo) {
        h--;
        if (h > 0 && tablero[i][h] == t) {
            cont++;
        } else {
            listo = true;
        }
    }
    h = j;
    listo = false;
    // Derecha
    while (!listo) {
        h++;
        if (h < 8 && tablero[i][h] == t) {
            cont++;
        } else {
            listo = true;
        }
    }
    return cont >= 4;
}

// Revisa si hay 4 en línea verticalmente
bool RevisarVertical(int i, int j, char t) {
    int cont = 1;
    int h = i;
    bool listo = false;
    while (!listo) {
        h++;
        if (h < 6 && tablero[h][j] == t) {
            cont++;
        } else {
            listo = true;
        }
    }
    return cont >= 4;
}

// Revisa si hay 4 en línea en la diagonal principal
bool RevisarDiagonal(int i, int j, char t) {
    int cont = 1;
    int h = i;
    int k = j;
    bool listo = false;
    // Izquierda
    while (!listo) {
        h++;
        k--;
        if (h < 6 && k > 0 && tablero[h][k] == t) {
            cont++;
        } else {
            listo = true;
        }
    }
    h = i;
    k = j;
    listo = false;
    // Derecha
    while (!listo) {
        h--;
        k++;
        if (h > 0 && k < 8 && tablero[h][k] == t) {
            cont++;
        } else {
            listo = true;
        }
    }
    return cont >= 4;
}

// Revisa si hay 4 en línea en la diagonal invertida
// Revisa si hay 4 en línea en la diagonal invertida
bool RevisarDiagonalInvertida(int i, int j, char t) {
    int cont = 1;
    int h = i;
    int k = j;
    bool listo = false;
    // Derecha
    while (!listo) {
        h++;
        k++;
        if (h < 6 && k < 8 && tablero[h][k] == t) {
            cont++;
        } else {
            listo = true;
        }
    }
    h = i;
    k = j;
    listo = false;
    // Izquierda
    while (!listo) {
        h--;
        k--;
        if (h > 0 && k > 0 && tablero[h][k] == t) {
            cont++;
        } else {
            listo = true;
        }
    }
    return cont >= 4;
}
