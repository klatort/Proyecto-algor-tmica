#include<Windows.h>
#include<iostream>
#include<fstream>
#include<vector>

using namespace std;

// Estructura para hacer la simulaci�n
struct matrizB
{
	int ancho, alto; // Dimensiones de la matr�z
	bool** matriz; // La matr�z
	matrizB(int ancho, int alto) {
		this->ancho = ancho;
		this->alto	= alto;
		// Inicializaci�n de la matr�z de tama�o variable
		matriz = new bool* [ancho];
		for (int i = 0; i < ancho; i++) {
			matriz[i] = new bool[alto];
		}
		for (int i = 0; i < ancho; i++) {
			for (int j = 0; j < alto; j++) {
				matriz[i][j] = 0;
			}
		}
	}
};


// Funci�n para dibujar en un punto espec�fico de la consola
void gotoxy(int x, int y) 
{
	HANDLE hcon = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD dwPos;
	dwPos.X = x;
	dwPos.Y = y;
	SetConsoleCursorPosition(hcon, dwPos);
}


// Impresi�n de la matr�z y eliminaci�n del rastro
void imprimir_mat(matrizB* mat, bool** aux)
{
	for (int i = 0; i < mat->ancho; i++) {
		for (int j = 0; j < mat->alto; j++) {
			// Borra el rastro del punto
			if (mat->matriz[i][j]) {
				gotoxy(j, i);
				cout << " ";
			}
			// Imprime la posici�n actual del punto
			if (aux[i][j]) {
				gotoxy(j, i);
				cout << (char)219;
			}
		}
	}
}


// L�gica del juego de la vida
void logica(matrizB* mat) 
{
	// Creaci�n de una matr�z auxiliar para poder trabajar la
	// l�gica del juego de la vida de manera correcta
	bool** aux = new bool* [mat->ancho];
	for (int i = 0; i < mat->ancho; i++) {
		aux[i] = new bool[mat->alto];
	}


	// Aplica la l�gica de el juego de la vida
	for (int i = 0; i < mat->ancho; i++) {
		for (int j = 0; j < mat->alto; j++) {
			// Variable que controla la cantidad de vecinos
			// alrededor de un punto
			int neighbors = 0;

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
				// Revisa si i es menor que el ancho de la matr�z
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
			// Revisa si i es menor que el ancho de la matr�z
			// para revisar si existen vecinos a la derecha
			// del punto
			if (i < mat->ancho - 1) {
				if (mat->matriz[i + 1][j] == 1) {
					neighbors++;
				}
			}
			// Revisa si j es menor al alto de la matr�z
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
				// Revisa si i es menor que el ancho de la matr�z
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

			// 3 vecinos y no hay punto entre ellos generar�n
			// otro punto
			if (neighbors == 3 && mat->matriz[i][j] == 0) {
				aux[i][j] = 1;
			}
			// 2 o 3 vecinos har�n que el punto se mantenga en su
			// estado actual
			else if (neighbors == 2 || neighbors == 3) {
				aux[i][j] = mat->matriz[i][j];
			}
			// Si hay 0, 1 o 4 a m�s vecinos, el punto entre ellos
			// se borrar�
			else if (neighbors == 0 || neighbors == 1 || neighbors >= 4) {
				aux[i][j] = 0;
			}
		}
	}

	// Impresi�n de la matr�z en su estado luego de la
	// simulaci�n, tal que se borra el rastro de su estado anterior
	imprimir_mat(mat, aux);


	// Reemplazo de la matr�z de la simulaci�n con la auxiliar para
	// actualizar los datos en esta
	for (int i = 0; i < mat->ancho; i++) {
		for (int j = 0; j < mat->alto; j++) {
			mat->matriz[i][j] = aux[i][j];
		}
	}
}

// Actualizar el display y dibujar una carita al centro del mismo
void actualizar_display(matrizB* mat) {
	gotoxy(mat->ancho / 2, mat->alto / 2);
	cout << (char)1;
	imprimir_mat(mat, mat->matriz);
}

int main() 
{
	// Dimensiones de la simulaci�n
	int tamanioX = 99;
	int tamanioY = 99;
	matrizB* vida = new matrizB(tamanioX, tamanioY);

	// Posici�n del cursor
	int posCursorX = 0, posCursorY = 0;

	while (true) 
	{
		while (true) 
		{
			if (GetKeyState(0x25) & 0x8000) // Flecha izquierda 
			{
				if (posCursorX > 0) {
					posCursorX--;	// Se mueve hacia la izquierda si y no es menor que los l�mites en X
				}
			}
			if (GetKeyState(0x27) & 0x8000) // Flecha derecha 
			{
				if (posCursorX < tamanioX - 1) {
					posCursorX++;	// Se mueve hacia la derecha si y no es mayor que los l�mites en x
				}
			}
			if (GetKeyState(0x26) & 0x8000) // Flecha arriba
			{
				if (posCursorY > 0) {
					posCursorY--;	// Se mueve hacia arriba si y no es m�s alto que los l�mites en y
				}
			}
			if (GetKeyState(0x28) & 0x8000) //Flecha abajo
			{
				if (posCursorY < tamanioX - 1) {
					posCursorY++;	// Se mueve hacia abajo si y no es m�s bajo que los l�mites en y
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
				break; // Empezar simulaci�n
			}
			if (GetKeyState('H') & 0x8000) 
			{
				guardar_mat(vida); // Guardar puntos en archivo
			}
			if (GetKeyState('G') & 0x8000) 
			{
				cargar_mat(vida); // Cargar puntos en archivo
			}
			if (GetKeyState('W') & 0x8000) 
			{
				// Limpiar pantalla
				vida = new matrizB(tamanioX, tamanioY);
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
		// Empezar simulaci�n
		while (true) {
			logica(vida);
			Sleep(50);
			if (GetKeyState('P') & 0x8000) break; // Pausar simulaci�n
		}
	}
	return 0;
}