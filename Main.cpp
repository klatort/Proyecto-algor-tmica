#include<Windows.h>
#include<iostream>
#include<fstream>
#include<vector>

using namespace std;

// Estructura para hacer la simulación
struct matrizB
{
	int ancho, alto;	// Dimensiones de la matríz
	int offSet;			// Regula la impresión para que sea infinita
	bool** matriz;		// La matríz
	matrizB(int ancho, int alto, int offSet) {
		this->offSet	= offSet;
		this->ancho		= ancho + offSet;
		this->alto		= alto + offSet;
		// Inicialización de la matríz de tamaño variable
		matriz = new bool* [this->alto];
		for (int i = 0; i < this->alto; i++) {
			matriz[i] = new bool[this->ancho];
			for (int j = 0; j < this->ancho; j++) {
				matriz[i][j] = 0;
			}
		}
	}
};

// Función para obtener la resolución de la pantalla
void GetDesktopResolution(int& horizontal, int& vertical)
{
	RECT desktop;
	const HWND hDesktop = GetDesktopWindow();
	GetWindowRect(hDesktop, &desktop);
	horizontal = desktop.right;
	vertical = desktop.bottom;
}

// Función para dibujar en un punto específico de la consola
void gotoxy(int x, int y) 
{
	HANDLE hcon = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD dwPos;
	dwPos.X = x;
	dwPos.Y = y;
	SetConsoleCursorPosition(hcon, dwPos);
}


// Impresión de la matríz y eliminación del rastro
void imprimir_mat(matrizB* mat, bool** aux)
{
	string aux;
	gotoxy(0, 0);
	for (int i = mat->offSet / 2; i < mat->alto - (mat->offSet / 2); i++) {
		for (int j = mat->offSet / 2; j < mat->ancho - (mat->offSet / 2); j++) {
			if (mat->matriz[i][j] == 1)
				aux += (char)219;
			else if (mat->matriz[i][j] == 0)
				aux += ' ';
			else
				aux += (char)2;
		}
		aux += '\n';
	}
	cout << aux;
	if (!actividad)
	{
		cout 
			<< "Simulacion pausada\nMover cursor: flechas"
			<< "\tInsertar punto: 'c'\tInsertar punto: 'v'\t Limpiar simulacion: 'w'"
			<< "\tIniciar simulacion: 'q'\tSiguiente en pasos: 'p'"
			<< "\t Cargar simulacion: 'f'\tGuardar simulacion: 'g'\tSalir: escape";
	}
	else
	{
		cout
			<< "Simulacion corriendo\nPausar simulacion: 'p'\tSalir: escape";
	}
}


// Lógica del juego de la vida
void logica(matrizB* mat) 
{
	// Creación de una matríz auxiliar para poder trabajar la
	// lógica del juego de la vida de manera correcta
	bool** aux = new bool* [mat->alto];
	for (int i = 0; i < mat->alto; i++) {
		aux[i] = new bool[mat->ancho];
		for (int j = 0; j < mat->ancho; j++) {
			aux[i][j] = mat->matriz[i][j];
		}
	}

	// Aplica la lógica de el juego de la vida
	for (int i = 0; i < mat->ancho; i++) {
		for (int j = 0; j < mat->alto; j++) {
			// Variable que controla la cantidad de vecinos
			// alrededor de un punto
			int neighbours = 0;

			// Revisa si j es mayor a cero para revisar si 
			// existen vecinos arriba del punto
			if (j > 0) {
				// Revisa si i es mayor a cero para revisar si 
				// existen vecinos a la izquierda del punto
				if (i > 0) {
					// Revisa si hay un vecino en la diagonal
					// superior izquierda del punto
					if (mat->matriz[i - 1][j - 1] == 1) {
						neighbors++;
					}
				}
				// Revisa si existe un vecino arriba del punto
				if (mat->matriz[i][j - 1] == 1) {
					neighbors++;
				}
				// Revisa si i es menor que el ancho de la matríz
				// para revisar si existen vecinos a la derecha
				// del punto
				if (i < mat->alto - 1) {
					// Revisa que haya un vecino en la diagonal
					// superior derecha del punto
					if (mat->matriz[i + 1][j - 1] == 1) {
						neighbors++;
					}
				}
			}
			// Revisa si i es mayor a cero para revisar si 
			// existen vecinos a la izquierda del punto
			if (i > 0) {
				if (mat->matriz[i - 1][j] == 1) {
					neighbors++;
				}
			}
			// Revisa si i es menor que el ancho de la matríz
			// para revisar si existen vecinos a la derecha
			// del punto
			if (i < mat->ancho - 1) {
				if (mat->matriz[i + 1][j] == 1) {
					neighbors++;
				}
			}
			// Revisa si j es menor al alto de la matríz
			// para revisar si existen vecinos debajo del punto
			if (j < mat->alto - 1) {
				// Revisa si i es mayor a cero para revisar si 
				// existen vecinos a la izquierda del punto
				if (i > 0) {
					// Revisa si hay un vecino en la diagonal
					// inferior izquierda del punto
					if (mat->matriz[i - 1][j + 1] == 1) {
						neighbors++;
					}
				}
				// Revisa si existe un vecino debajo del
				// punto
				if (mat->matriz[i][j + 1] == 1) {
					neighbors++;
				}
				// Revisa si i es menor que el ancho de la matríz
				// para revisar si existen vecinos a la derecha
				// del punto
				if (i < mat->ancho - 1) {
					// Revisa si hay un vecino en la diagonal
					// inferior derecha del punto
					if (mat->matriz[i + 1][j + 1] == 1) {
						neighbors++;
					}
				}
			}

			// Condiciones del juego de la vida

			// 3 vecinos y no hay punto entre ellos generarán
			// otro punto
			if (neighbours == 3 && mat->matriz[i][j] == 0) {
				aux[i][j] = 1;
			}
			// 2 o 3 vecinos harán que el punto se mantenga en su
			// estado actual
			else if (neighbours == 2 || neighbours == 3) {
				aux[i][j] = mat->matriz[i][j];
			}
			// Si hay 0, 1 o 4 a más vecinos, el punto entre ellos
			// se borrará
			else if (neighbours == 0 || neighbours == 1 || neighbours >= 4) {
				aux[i][j] = 0;
			}
		}
	}
	// Impresión de la matríz en su estado luego de la
	// simulación, tal que se borra el rastro de su estado anterior
	imprimir_mat(mat, aux);


	// Reemplazo de la matríz de la simulación con la auxiliar para
	// actualizar los datos en esta
	for (int i = 0; i < mat->alto; i++) {
		for (int j = 0; j < mat->ancho; j++) {
			mat->matriz[i][j] = aux[i][j];
		}
	}
}

// Actualizar el display y dibujar una carita al centro del mismo
void actualizar_display(matrizB* mat) {
	imprimir_mat(mat);
	int x = (mat->ancho - mat->offSet) / 2;
	int y = (mat->alto - mat->offSet) / 2;
	gotoxy(x, y);
	cout << (char)1;
	imprimir_mat(mat, mat->matriz);
}

// Cargar el archivo que se guardó
void cargar_mat(matrizB* mat) {
	ifstream load("drawing.txt");
	string aux;
	load >> aux;
	for (int i = 0; i < mat->alto; i++) {
		for (int j = 0; j < mat->ancho; j++) {
			if (i * mat->ancho + j < aux.length()) 
			{
				mat->matriz[i][j] = (int)aux.at(i * mat->ancho + j) - 48;
			}
			else {
				mat->matriz[i][j] = 0;
			}
		}
	}
}
int main() 
{
	// Dimensiones de la simulación
	int tamanioX = 99;
	int tamanioY = 99;
	matrizB* vida = new matrizB(tamanioX, tamanioY);

	// Posición del cursor
	int posCursorX = 0, posCursorY = 0;

	while (true) 
	{
		while (true) 
		{
			if (GetKeyState(0x25) & 0x8000) // Flecha izquierda 
			{
				if (posCursorX > 0) {
					posCursorX--;	// Se mueve hacia la izquierda si y no es menor que los límites en X
				}
			}
			if (GetKeyState(0x27) & 0x8000) // Flecha derecha 
			{
				if (posCursorX < tamanioX - 1) {
					posCursorX++;	// Se mueve hacia la derecha si y no es mayor que los límites en x
				}
			}
			if (GetKeyState(0x26) & 0x8000) // Flecha arriba
			{
				if (posCursorY > 0) {
					posCursorY--;	// Se mueve hacia arriba si y no es más alto que los límites en y
				}
			}
			if (GetKeyState(0x28) & 0x8000) //Flecha abajo
			{
				if (posCursorY < tamanioY - 1) {
					posCursorY++;	// Se mueve hacia abajo si y no es más bajo que los límites en y
				}
			}
			if (GetKeyState('C') & 0x8000) 
			{
				vida->matriz[posCursorY][posCursorX] = true; // Colocar punto
			}
			if (GetKeyState('V') & 0x8000) 
			{
				vida->matriz[posCursorY][posCursorX] = false; // Quitar punto
			}
			if (GetKeyState('Q') & 0x8000) 
			{
				system("cls");
				break; // Empezar simulación
			}
			if (GetKeyState('G') & 0x8000) 
			{
				guardar_mat(vida); // Guardar puntos en archivo
			}
			if (GetKeyState('F') & 0x8000) 
			{
				cargar_mat(vida); // Cargar puntos en archivo
			}
			if (GetKeyState('W') & 0x8000) 
			{
				// Limpiar pantalla
				vida = new matrizB(tamanioX, tamanioY, infinity);
				system("cls");
			}
			// Mostrar puntero para ubicarse en la consola
			gotoxy(posCursorX, posCursorY);
			cout << (char)219;
			// Espera de 50 milisegundos para no sufrir
			Sleep(50);
			// Tintineando
			gotoxy(posCursorX, posCursorY);
			cout << " ";
			// Actualizar buffer
			actualizar_display(vida);
		}
		// Empezar simulación
		while (true) {
			logica(vida);
			Sleep(50);
			if (GetKeyState('P') & 0x8000) break; // Pausar simulación
			if (GetKeyState(VK_ESCAPE) & 0x8000) return 0; //Salir del programa
		}
	}
	return 0;
}
