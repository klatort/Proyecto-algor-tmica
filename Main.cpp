#include<Windows.h>
#include<iostream>
#include<fstream>
#include<vector>
#include<conio.h>
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
	// Get a handle to the desktop window
	const HWND hDesktop = GetDesktopWindow();
	// Get the size of screen to the variable desktop
	GetWindowRect(hDesktop, &desktop);
	// The top left corner will have coordinates (0,0)
	// and the bottom right corner will have coordinates
	// (horizontal, vertical)
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
void imprimir_mat(matrizB* mat)
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
	for (int i = 0; i < mat->alto; i++) {
		for (int j = 0; j < mat->ancho; j++) {
			// Variable que controla la cantidad de vecinos
			// alrededor de un punto
			int neighbors = 0;

			// Revisa si i es mayor a cero para revisar si 
			// existen vecinos arriba del punto
			if (i > 0) {
				// Revisa si j es mayor a cero para revisar si 
				// existen vecinos a la izquierda del punto
				if (j > 0) {
					// Revisa si hay un vecino en la diagonal
					// superior izquierda del punto
					if (mat->matriz[i - 1][j - 1] == 1) {
						neighbors++;
					}
				}
				// Revisa si existe un vecino arriba del punto
				if (mat->matriz[i - 1][j] == 1) {
					neighbors++;
				}
				// Revisa si j es menor que el ancho de la matríz
				// para revisar si existen vecinos a la derecha
				// del punto
				if (j < mat->ancho - 1) {
					// Revisa que haya un vecino en la diagonal
					// superior derecha del punto
					if (mat->matriz[i - 1][j + 1] == 1) {
						neighbors++;
					}
				}
			}
			// Revisa si j es mayor a cero para revisar si 
			// existen vecinos a la izquierda del punto
			if (j > 0) {
				if (mat->matriz[i][j - 1] == 1) {
					neighbors++;
				}
			}
			// Revisa si j es menor que el ancho de la matríz
			// para revisar si existen vecinos a la derecha
			// del punto
			if (j < mat->ancho - 1) {
				if (mat->matriz[i][j + 1] == 1) {
					neighbors++;
				}
			}
			// Revisa si i es menor al alto de la matríz
			// para revisar si existen vecinos debajo del punto
			if (i < mat->alto - 1) {
				// Revisa si j es mayor a cero para revisar si 
				// existen vecinos a la izquierda del punto
				if (j > 0) {
					// Revisa si hay un vecino en la diagonal
					// inferior izquierda del punto
					if (mat->matriz[i + 1][j - 1] == 1) {
						neighbors++;
					}
				}
				// Revisa si existe un vecino debajo del
				// punto
				if (mat->matriz[i + 1][j] == 1) {
					neighbors++;
				}
				// Revisa si j es menor que el ancho de la matríz
				// para revisar si existen vecinos a la derecha
				// del punto
				if (j < mat->ancho - 1) {
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
			if (neighbors == 3 && mat->matriz[i][j] == 0) {
				aux[i][j] = 1;
			}
			// 2 o 3 vecinos harán que el punto se mantenga en su
			// estado actual
			else if (neighbors == 2 || neighbors == 3) {
				aux[i][j] = mat->matriz[i][j];
			}
			// Si hay 0, 1 o 4 a más vecinos, el punto entre ellos
			// se borrará
			else if (neighbors == 0 || neighbors == 1 || neighbors >= 4) {
				aux[i][j] = 0;
			}
		}
	}
	// Impresión de la matríz en su estado luego de la
	// simulación, tal que se borra el rastro de su estado anterior
	imprimir_mat(mat);
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
}

void guardar_mat(matrizB* mat) {
	ofstream output_file("drawing.txt");
	for (int i = 0; i < mat->alto; i++) {
		for (int j = 0; j < mat->ancho; j++) {
			output_file << mat->matriz[i][j];
		}
	}
}

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
	//CONSOLE_SCREEN_BUFFER_INFO csbi;
	//GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
	//int tamanioX = csbi.srWindow.Right - csbi.srWindow.Left + 1;
	//int tamanioY = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
	HWND hWnd = GetConsoleWindow();
	ShowWindow(hWnd, SW_SHOWMAXIMIZED);
	CONSOLE_FONT_INFOEX cfi;
	cfi.cbSize = sizeof(cfi);
	cfi.nFont = 0;
	cfi.dwFontSize.X = 10;                   // Width of each character in the font
	cfi.dwFontSize.Y = 10;                  // Height
	cfi.FontFamily = FF_DONTCARE;
	cfi.FontWeight = FW_NORMAL;
	wcscpy_s(cfi.FaceName, L"Consolas"); // Choose your font
	SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &cfi);
	//SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE), { 190, 98 });

	int screenW;
	int screenH;

	GetDesktopResolution(screenW, screenH);
	cout << screenH << " " << screenW;
	_getch();

	// Dimensiones de la simulación
	int tamanioX = screenW / 10 - 2; //1920/10 - 2
	int tamanioY = screenH / 10 - 10; //1080/10 - 6
	int infinity = 200;
	matrizB* vida = new matrizB(tamanioX, tamanioY, infinity);
	for (int i = 0; i < 190; i++) {
		for (int j = 0; j < 98; j++) {
			gotoxy(i, j);
			cout << (char)219;
		}
	}
	_getch();
	//while (true) { i++; if (i > 9) { i = 1; } cout << i << " "; }
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
				vida->matriz[posCursorY + vida->offSet / 2][posCursorX + vida->offSet / 2] = true; // Colocar punto
			}
			if (GetKeyState('V') & 0x8000) 
			{
				vida->matriz[posCursorY + vida->offSet / 2][posCursorX + vida->offSet / 2] = false; // Quitar punto
			}
			if (GetKeyState('Q') & 0x8000) 
			{
				break; // Empezar simulación
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
				vida = new matrizB(tamanioX, tamanioY, infinity);
				system("cls");
			}
			if (GetKeyState('P') & 0x8000)
			{
				logica(vida);
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
		}
	}
	return 0;
}