#include<Windows.h>
#include<iostream>
#include<fstream>
#include<conio.h>
using namespace std;

// Estructura para hacer la simulación
struct matrizB
{
	int ancho, alto;	// Dimensiones de la matríz
	int offSet;			// Regula la impresión para que sea infinita
	bool** matriz;		// La matríz
	matrizB(int ancho, int alto, int offSet) {
		this->offSet = offSet;
		this->ancho = ancho + offSet;
		this->alto = alto + offSet;
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
void imprimir_mat(matrizB* mat, bool actividad = false)
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

	for (int i = 1; i < mat->alto - 1; i++) {
		for (int j = 1; j < mat->ancho - 1; j++) {
			// Variable que controla la cantidad de vecinos
			// alrededor de un punto
			int neighbours = 0;

			if (mat->matriz[i - 1][j - 1] == 1) neighbours++;
			if (mat->matriz[i - 1][j] == 1)		neighbours++;
			if (mat->matriz[i - 1][j + 1] == 1)	neighbours++;
			if (mat->matriz[i][j - 1] == 1)		neighbours++;
			if (mat->matriz[i][j + 1] == 1)		neighbours++;
			if (mat->matriz[i + 1][j - 1] == 1) neighbours++;
			if (mat->matriz[i + 1][j] == 1)		neighbours++;
			if (mat->matriz[i + 1][j + 1] == 1) neighbours++;

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
	imprimir_mat(mat, true);
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

// Guardar en un archivo lo que se tenga dibujado en la simulación
void guardar_mat(matrizB* mat) {
	ofstream output_file("drawing.txt");
	for (int i = mat->offSet / 2; i < mat->alto - mat->offSet / 2; i++) {
		for (int j = mat->offSet / 2; j < mat->ancho - mat->offSet / 2; j++) {
			output_file << mat->matriz[i][j];
		}
	}
}

// Cargar el archivo que se guardó
void cargar_mat(matrizB* mat) {
	ifstream load("drawing.txt");
	string aux;
	load >> aux;
	for (int i = mat->offSet / 2; i < mat->alto - mat->offSet / 2; i++) {
		for (int j = mat->offSet / 2; j < mat->ancho - mat->offSet / 2; j++) {
			if ((i - mat->offSet / 2) *
				(mat->ancho - mat->offSet) +
				(j - mat->offSet / 2) < aux.length())
			{
				mat->matriz[i][j] =
					(int)aux.at((i - mat->offSet / 2) *
						(mat->ancho - mat->offSet) +
						(j - mat->offSet / 2)) - 48;
			}
		}
	}
}

//Menu con indicaciones
void menu(int w, int h)
{
	gotoxy(w / 2 - 14, 0);
	cout << "Bienvenido al simulador del";
	gotoxy(w / 2 - 14, 1);
	cout
		<< "juego de la vida de Conway\n"
		<< "Las reglas del juego son:\n"
		<< "Cada punto blanco es una vida, esta puede tener mas vidas alrededor que seran sus vecinos\n"
		<< "Si un punto tiene 3 vecinos y no existe algo en dicho punto, se creara vida\n"
		<< "Si hay 2 o 3 vecinos cerca de un punto, este mantendra la vida que tenga\n"
		<< "Si hay 0, 1, 4 o mas vecinos, se terminara con la vida en dicho punto\n"
		<< "La cara feliz que se ve en la pantalla es el centro de la simulacion";
	system("pause");
}

int main()
{
	CONSOLE_FONT_INFOEX cfi;
	cfi.cbSize = sizeof(cfi);
	cfi.nFont = 0;
	cfi.dwFontSize.X = 10;                   // Ancho de caracter
	cfi.dwFontSize.Y = 10;                   // Alto de caracter
	cfi.FontFamily = FF_DONTCARE;
	cfi.FontWeight = FW_NORMAL;
	wcscpy_s(cfi.FaceName, L"Consolas");	 // Font
	// Seteando el font
	SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &cfi);
	//SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE), { 190, 98 });

	int screenW;
	int screenH;

	GetDesktopResolution(screenW, screenH);

	HWND hWnd = GetConsoleWindow();
	ShowWindow(hWnd, SW_SHOWMAXIMIZED);		 // Ventana maximizada

	// Dimensiones de la simulación
	int tamanioX = screenW / 10 - 2; //1920/10 - 2
	int tamanioY = screenH / 10 - 11; //1080/10 - 11

	menu(tamanioX, tamanioY);

	int infinity = 100;
	matrizB* vida = new matrizB(tamanioX, tamanioY, infinity);
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
				vida->matriz
					[posCursorY + vida->offSet / 2]
				[posCursorX + vida->offSet / 2] = true; // Colocar punto
			}
			if (GetKeyState('V') & 0x8000)
			{
				vida->matriz
					[posCursorY + vida->offSet / 2]
				[posCursorX + vida->offSet / 2] = false; // Quitar punto
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
			if (GetKeyState('P') & 0x8000)
			{
				logica(vida); //Avanzar un paso
			}
			if (GetKeyState(VK_ESCAPE) & 0x8000)
			{
				return 0; //Salir del programa
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
