#include <iostream>

using namespace std;

void JugarPartida();
void LimpiarTablero();
void LimpiarPantalla();
void MostrarTablero();
void UbicarFicha();
bool RevisarHorizontal(int, int, char);
bool RevisarVertical(int, int, char);
bool RevisarDiagonal(int, int, char);
bool RevisarDiagonalInvertida(int, int, char);

//se crea un tablero de 8x8
char tablero[8][8]={{{'1'},{' '},{' '},{' '}, {' '}, {' '}, {' '}, {' '} },
					{{'2'},{' '},{' '},{' '}, {' '}, {' '}, {' '}, {' '} },
					{{'3'},{' '},{' '},{' '}, {' '}, {' '}, {' '}, {' '} },
					{{'4'},{' '},{' '},{' '}, {' '}, {' '}, {' '}, {' '} },
					{{'5'},{' '},{' '},{' '}, {' '}, {' '}, {' '}, {' '} },
					{{'6'},{' '},{' '},{' '}, {' '}, {' '}, {' '}, {' '} },
					{{'-'},{'-'},{'-'},{'-'}, {'-'}, {'-'}, {'-'}, {'-'} },
					{{' '},{'1'},{'2'},{'3'}, {'4'}, {'5'}, {'6'}, {'7'} }};

//Variables globales
char eleccion;
char turno;
int colT, colD;
int indD;
int cont = 0;
int h , k;
bool listo = false;

//juego en si
int main(){
	while(true){
		LimpiarPantalla();
		cout<<"\t:::CUATRO EN LINEA:::"<<endl;
		cout<<"\t\t1) Jugar."<<endl;
		cout<<"\t\t2) Salir."<<endl;
		cout<<"\tEleccion: ";
		cin>>eleccion;
		switch(eleccion){
			case '1':
				turno = 'O';
				JugarPartida();
				LimpiarTablero();
			break;
			case '2':
				 return 0;
			break;
			default:
			cout<<"Opcion no disponible. Ingrese algun caracter para continuar: ";
			cin>>eleccion;
			break;
		}
	}
	return 0;
}

//Limpia pantalla
void LimpiarPantalla(){
	if(system("clear") == -1){
		cout<<"Error al limpiar la pantalla."<<endl;
		LimpiarPantalla();
	}
}

//Limpia el tablero
void LimpiarTablero(){
	for(int i = 0; i < 6; i++){
		for(int j = 1; j < 8; j++){
			tablero[i][j] = ' ';
		}
	}
}

//Inicia el juego
void JugarPartida(){
	LimpiarPantalla();
	MostrarTablero();
	
	if(RevisarHorizontal(indD, colD, turno) || RevisarVertical(indD, colD, turno)  || RevisarDiagonal(indD,colD,turno) || RevisarDiagonalInvertida(indD,colD,turno)){
		cout<<"\tIngrese la alternativa que desee: ";
		cin>>eleccion;
		return;
	}
	
	if(turno == 'S'){
		turno = 'C';
	}else{
		turno = 'S';
	}
	
	cout<<"\n\tTurno de "<<turno<<endl;
	cout<<"\tElija una columna: ";
	cin>>colT;
	//Validar columna
	if(colT > 7 || colT < 1){
		cout<<"\n\tColumna Inexistente."<<endl;
		cout<<"\tIngrese la alternativa que desee: ";
		cin>>eleccion;
		JugarPartida();
	}else{
		//coloca ficha si es valida
		colD = colT;
		UbicarFicha();
	}
	JugarPartida();
}

//Muestra el tablero
void MostrarTablero(){
	cout<<"\t:::CUATRO EN LINEA:::"<<endl;
	for(int i = 0; i < 8; i ++){
		cout<<endl<<"\t";
		for(int j = 0; j < 9; j++){
			cout<<tablero[i][j]<<" ";
		}
	}
}
//coloca ficha
void UbicarFicha(){
	for(int i = 0; i < 6; i++){
		if(tablero[i][colD] == ' '){
			if(i > 0){
				tablero[i-1][colD] = ' ';
			}
			tablero[i][colD] = turno;
			indD = i;
		}
	}
}

//Revisa si hay 4 en linea en el caso de filas
bool RevisarHorizontal(int i, int j, char t){
	cont = 1;
	h = j;
	listo = false;
	//Izquierda
	while(!listo){
		h --;
		if(h > 0){
			if(tablero[i][h] == t){
				cont++;
			}else{
				listo = true;
			}
		}else{
			listo = true;
		}
	}
	h = j;
	listo = false;
	//Derecha
	while(!listo){
		h ++;
		if(h < 8){
			if(tablero[i][h] == t){
				cont++;
			}else{
				listo = true;
			}
		}else{
			listo = true;
		}
	}
	if(cont >= 4){
		cout<<"\n\tGano el jugador "<<t<<endl;
		return true;
	}
	return false;
}

//Revisa si hay 4 en linea en el caso de columnas
bool RevisarVertical(int i, int j , char t){
	cont = 1;
	h = i;
	listo = false;
	while(!listo){
		h++;
		if(h < 6){
			if(tablero[h][j] == t){
				cont++;
			}else{
				listo = true;
			}
		}else{
			listo = true;
		}
	}
	if(cont >= 4){
		cout<<"\n\tGano el jugador "<<t<<endl;
		return true;
	}
	return false;
}

//Revisa si hay 4 en linea en el caso de diagonales
bool RevisarDiagonal(int i, int j, char t){
	cont = 1;
	h = i;
	k = j;
	listo = false;
	//Izquierda
	while(!listo){
		h ++;
		k --;
		if(h < 6 && k > 0){
			if(tablero[h][k] == t){
				cont++;
			}else{
				listo = true;
			}
		}else{
			listo = true;
		}
	}
	h = i;
	k = j;
	listo = false;
	//Derecha
	while(!listo){
		h --;
		k ++;
		if(h > 0 && k < 8){
			if(tablero[h][k] == t){
				cont++;
			}else{
				listo = true;
			}
		}else{
			listo = true;
		}
	}
	if(cont >= 4){
		cout<<"\n\tGano el jugador "<<t<<endl;
		return true;
	}
	return false;
}

//mismo caso que RevisarDiagonal pero en sentido contrario
bool RevisarDiagonalInvertida(int i, int j, char t){
	cont = 1;
	h = i;
	k = j;
	listo = false;
	//Derecha
	while(!listo){
		h ++;
		k ++;
		if(h < 6 && k < 8){
			if(tablero[h][k] == t){
				cont++;
			}else{
				listo = true;
			}
		}else{
			listo = true;
		}
	}
	h = i;
	k = j;
	listo = false;
	//Izquierda
	while(!listo){
		h --;
		k --;
		if(h > 0 && k > 0){
			if(tablero[h][k] == t){
				cont++;
			}else{
				listo = true;
			}
		}else{
			listo = true;
		}
	}
	if(cont >= 4){
		cout<<"\n\tGano el jugador "<<t<<endl;
		return true;
	}
	return false;
}