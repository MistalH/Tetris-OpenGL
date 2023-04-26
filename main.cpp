#define WIN32_LEAN_AND_MEAN				// "odchudza" aplikacjê Windows

// Pliki nag³ówkowe
#include <windows.h>					// standardowy plik nag³ówkowy Windows
#include <gl/gl.h>						// standardowy plik nag³ówkowy OpenGL
#include <gl/glu.h>						// plik nag³ówkowy biblioteki GLU
//#include <gl/glaux.h>					// funkcje pomocnicze OpenGL 

#pragma comment(lib, "legacy_stdio_definitions.lib")

// Zmienne globalne
HDC g_HDC;								// globalny kontekst urz¹dzenia
bool fullScreen = false;				// true = tryb pe³noekranowy; false = tryb okienkowy

// Parametry oœwietlenia
float ambientLight[] = { 0.2f, 0.2f, 0.2f, 1.0f };	    // œwiat³o otoczenia
float diffuseLight[] = { 1.0f, 1.0f, 1.0f, 1.0f };		// œwiat³o rozproszone
float lightPosition[] = { 1.0f, 0.0f, 1.0f, 0.0f };	    // po³o¿enie Ÿród³a œwiat³a

// Parametry materia³u
float ambientMaterial[] = { 0.2f, 0.2f, 0.2f, 1.0f };
float diffuseMaterial[] = { 0.4f, 0.8f, 0.5f, 1.0f };

// Po³o¿enie szeœcianu
float X = 0, Y = 2.5f, Z = 0, RX = 0, RY = 0, RZ = 0;

// Etap animacji
int etap = 0;

int swipe = 0;
int rotate = 0;
float angle = 0;
float rot = 0;

int tick = 0;

void Initialize()       // Inicjacja OpenGL
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);		// czarny kolor t³a

	glShadeModel(GL_SMOOTH);					// cieniowanie g³adkie
	glEnable(GL_DEPTH_TEST);					// w³¹czenie bufora g³êbi
	glEnable(GL_CULL_FACE);						// ukrywanie tylnych stron wielok¹tów
	glFrontFace(GL_CCW);						// porz¹dek wierzcho³ków przeciwny do kierunku ruchu wskazówek zegara
	glEnable(GL_LIGHTING);						// w³¹czenie oœwietlenia

	// Ustawienie w³aœciwoœci materia³u dla pierwszego Ÿród³a œwiat³a LIGHT0
	glMaterialfv(GL_FRONT, GL_AMBIENT, ambientMaterial);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuseMaterial);

	// Ustawienie Ÿród³a œwiat³a GL_LIGHT0
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);		// sk³adowa otoczenia
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);		// sk³adowa rozproszona
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);	// po³o¿enie Ÿród³a œwiat³a

	// W³¹czenie Ÿród³a œwiat³a
	glEnable(GL_LIGHT0);
	glEnable(GL_COLOR_MATERIAL);
}

void DrawCube(float xPos, float yPos, float zPos, float zRot) //Narysowanie szeœcianu w xPos,yPos,zPos obróconego o k¹ty xRot,yRot,zRot
{

	glPushMatrix();
	glTranslatef(xPos, yPos, zPos);
	glRotatef(zRot, 0.0f, 0.0f, 1.0f);



	glPushMatrix();
	glTranslatef(xPos, yPos, zPos);
	glRotatef(zRot, 0.0f, 0.0f, 1.0f);
	glBegin(GL_POLYGON);
	glNormal3f(0.0f, 1.0f, 0.0f);	// górna œciana (w p³aszczyŸnie XZ)
	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex3f(0.5f, 0.5f, 0.5f);
	glVertex3f(0.5f, 0.5f, -0.5f);
	glVertex3f(-0.5f, 0.5f, -0.5f);
	glVertex3f(-0.5f, 0.5f, 0.5f);
	glEnd();

	glBegin(GL_POLYGON);
	glNormal3f(0.0f, 0.0f, 1.0f);	// przednia œciana (w p³aszczyŸnie XY)
	glColor3f(1.0f, 1.0f, 0.0f);
	glVertex3f(0.5f, 0.5f, 0.5f);
	glVertex3f(-0.5f, 0.5f, 0.5f);
	glVertex3f(-0.5f, -0.5f, 0.5f);
	glVertex3f(0.5f, -0.5f, 0.5f);
	glEnd();
	glBegin(GL_POLYGON); // Wielok¹ty
	glNormal3f(1.0f, 0.0f, 0.0f);	// prawa œciana (w p³aszczyŸnie YZ) stawiana jest normalna (1.0f, 0.0f, 0.0f), co oznacza, ¿e powierzchnia wychodzi prostopadle do osi X.
	glColor3f(1.0f, 0.0f, 1.0f);
	glVertex3f(0.5f, 0.5f, 0.5f);
	glVertex3f(0.5f, -0.5f, 0.5f);
	glVertex3f(0.5f, -0.5f, -0.5f);
	glVertex3f(0.5f, 0.5f, -0.5f);
	glEnd();
	glBegin(GL_POLYGON);
	glNormal3f(-1.0f, 0.0f, 0.0f);	// lewa œciana (w p³aszczyŸnie YZ)
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(-0.5f, 0.5f, 0.5f);
	glVertex3f(-0.5f, 0.5f, -0.5f);
	glVertex3f(-0.5f, -0.5f, -0.5f);
	glVertex3f(-0.5f, -0.5f, 0.5f);
	glEnd();
	glBegin(GL_POLYGON);
	glNormal3f(0.0f, -1.0f, 0.0f);	// dolna œciana (w p³aszczyŸnie XZ)
	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3f(-0.5f, -0.5f, 0.5f);
	glVertex3f(-0.5f, -0.5f, -0.5f);
	glVertex3f(0.5f, -0.5f, -0.5f);
	glVertex3f(0.5f, -0.5f, 0.5f);
	glEnd();
	glBegin(GL_POLYGON);
	glNormal3f(0.0f, 0.0f, -1.0f);	// tylna œciana (w p³aszczyŸnie XY)
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(0.5f, -0.5f, -0.5f);
	glVertex3f(-0.5f, -0.5f, -0.5f);
	glVertex3f(-0.5f, 0.5f, -0.5f);
	glVertex3f(0.5f, 0.5f, -0.5f);
	glEnd();
	glPopMatrix();
}

void reset()
{
	X = 0.0f;
	Y = 2.5f;
	angle = 0.0f;
}


/*
Pierwsz¹ rzecz¹, któr¹ kod robi jest sprawdzanie czy wartoœæ zmiennej "Y" jest mniejsza ni¿ -2.5f. Jeœli tak, to jest wywo³ywana funkcja "reset()".

Nastêpnie wartoœæ "Y" jest zmniejszana o 0.0003f. Zmienna "tick" jest równie¿ zmniejszana o 1, chyba ¿e jest ju¿ równa 0.

Jeœli "tick" jest równy 0, wykonywane s¹ dalsze dzia³ania, w zale¿noœci od wartoœci "rotate" i "swipe". Wartoœæ "X"
jest zwiêkszana lub zmniejszana o 0.5f, a k¹t "angle" jest zwiêkszany lub zmniejszany o 90.0f.

Na koñcu, wartoœæ "tick" jest ustawiana na 300.


*/
void move()
{
	if (Y < -2.5f) // wielkosc okna sie konczy na - 2.5 i gra sie resetuje
		reset();
	Y -= 0.0003f; // predkosc spadania klocka im mniejsza tym szybciej
	tick = tick > 0 ? tick - 1 : 0;
	if (tick == 0) {
		switch (rotate) {
		case 1:
			angle += 90.0f;
			break;
		case 2:
			angle -= 90.0f;
			break;
		case 3:
			angle += 180.0f;
			break;
		}
		switch (swipe) {
		case 1:
			if (X > -3.0f)
				X -= 0.5f;
			break;
		case 2:
			if (X < 3.0f)
				X += 0.5f;
			break;
		}
		tick = 300; // odpowiednia predkosc przy 100 jedno klikniecie zmienia kolory za szybko a 900 ma laga
	}
}

void Render()   // Renderowanie sceny
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		// Opró¿nienie bufora ekranu i bufora g³êbi
	glLoadIdentity();										// Zresetowanie macierzy modelowania

	glTranslatef(0, 0, -7);		// Wykonanie przekszta³ceñ geometrycznych
	glRotatef(rot, 0, 1, 0);

	glPushMatrix();
	move();
	glTranslatef(0, Y, 0);
	DrawCube(X, Y, Z, angle);			// Narysowanie szeœcianu
	glPopMatrix();

	glFlush();								// Zrzucenie bufora graficznego na ekran

	swipe = 0;
	rotate = 0;

	SwapBuffers(g_HDC);						// Prze³¹czenie buforów
}


void SetupPixelFormat(HDC hDC)  // Funkcja okreœlaj¹ca format pikseli
{
	int nPixelFormat;					// indeks formatu pikseli

	static PIXELFORMATDESCRIPTOR pfd = {
		sizeof(PIXELFORMATDESCRIPTOR),	// rozmiar struktury
		1,								// domyœlna wersja
		PFD_DRAW_TO_WINDOW |			// grafika w oknie
		PFD_SUPPORT_OPENGL |			// grafika OpenGL 
		PFD_DOUBLEBUFFER,				// podwójne buforowanie
		PFD_TYPE_RGBA,					// tryb kolorów RGBA 
		32,								// 32-bitowy opis kolorów
		0, 0, 0, 0, 0, 0,				// nie specyfikuje bitów kolorów
		0,								// bez bufora alfa
		0,								// nie specyfikuje bitu przesuniêcia
		0,								// bez bufora akumulacji
		0, 0, 0, 0,						// ignoruje bity akumulacji
		16,								// 16-bitowy bufor Z
		0,								// bez bufora powielania
		0,								// bez buforów pomocniczych
		PFD_MAIN_PLANE,					// g³ówna p³aszczyzna rysowania
		0,								// zarezerwowane
		0, 0, 0 };						// ignoruje maski warstw

	nPixelFormat = ChoosePixelFormat(hDC, &pfd);	// wybiera najbardziej zgodny format pikseli 
	SetPixelFormat(hDC, nPixelFormat, &pfd);		// okreœla format pikseli dla danego kontekstu urz¹dzenia
}


LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)  // Procedura okienkowa
{
	static HGLRC hRC;					// kontekst tworzenia grafiki
	static HDC hDC;						// kontekst urz¹dzenia
	int width, height;					// szerokoœæ i wysokoœæ okna

	switch (message)						// obs³uga komunikatów
	{
	case WM_CREATE:					// Utworzenie okna
		hDC = GetDC(hwnd);			// pobiera kontekst urz¹dzenia dla okna
		g_HDC = hDC;
		SetupPixelFormat(hDC);		// wywo³uje funkcjê okreœlaj¹c¹ format pikseli
		// tworzy kontekst grafiki i czyni go bie¿¹cym
		hRC = wglCreateContext(hDC);
		wglMakeCurrent(hDC, hRC);
		return 0;
		break;

	case WM_CLOSE:					// Zamkniêcie okna
		wglMakeCurrent(hDC, NULL);  // usuwa kontekst renderowania okna
		wglDeleteContext(hRC);
		PostQuitMessage(0);			// wysy³a WM_QUIT do kolejki komunikatów konczy g³owna petle 
		return 0;
		break;

	case WM_SIZE:					// Zmiana wymiarów okna
		height = HIWORD(lParam);
		width = LOWORD(lParam);
		if (height == 0)				// zabezpieczenie przed dzieleniem przez 0
			height = 1;
		glViewport(0, 0, width, height);		// nadanie nowych wymiarów okna OpenGL
		glMatrixMode(GL_PROJECTION);			// prze³¹czenie macierzy rzutowania
		glLoadIdentity();						// zresetowanie macierzy rzutowania
		gluPerspective(54.0f, (GLfloat)width / (GLfloat)height, 1.0f, 1000.0f);  // wyznaczenie proporcji obrazu i ustawienie rzutowania perspektywicznego
		glMatrixMode(GL_MODELVIEW);				// prze³¹czenie macierzy modelowania
		glLoadIdentity();						// zresetowanie macierzy modelowania
		return 0;
		break;
		//ruch obiektow
		/*
		Sprawdza on wartoœæ parametru wParam dla zdarzeñ WM_CHAR i WM_KEYDOWN.
		Jeœli wartoœæ wParam jest równa 'a' lub 'd' dla WM_CHAR, to zmienna "swipe" jest ustawiana na odpowiedni¹ wartoœæ (1 lub 2).
		Jeœli wartoœæ wParam jest równa VK_LEFT lub VK_RIGHT dla WM_KEYDOWN, zmienna "rotate" jest ustawiana na odpowiedni¹ wartoœæ (1 lub 2)
		. Te zmienne mog¹ byæ u¿ywane do okreœlenia dalszego dzia³ania programu.
		wParam jest u¿ywany do przekazywania informacji o klawiszu, który zosta³ naciœniêty lub wciœniêty.
		Wartoœæ wParam jest porównywana z sta³ymi (np. 'a', 'd', VK_LEFT, VK_RIGHT) w celu okreœlenia, jaki klawisz zosta³ naciœniêty,
		i odpowiednie dzia³anie jest wykonywane zgodnie z tym.

		*/

	case WM_CHAR:
		switch (wParam) {
		case 'a':
			swipe = 1;
			break;
		case 'd':
			swipe = 2;
			break;
		}
	case WM_KEYDOWN:
		switch (wParam) {
		case VK_LEFT:
			rotate = 1;
			break;
		case VK_RIGHT:
			rotate = 2;
			break;

		case 'r':
			rot += 0.5;
			break;
		}

	case VK_UP:
		switch (wParam) {
		case 's':
			rotate = 3;
			break;
		}

	default:
		break;
	}

	return (DefWindowProc(hwnd, message, wParam, lParam));

}

// Funkcja g³ówna, od której rozpoczyna siê wykonywanie aplikacji
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	WNDCLASSEX windowClass;		// klasa okna
	HWND	   hwnd;			// uchwyt okna
	MSG		   msg;				// komunikat
	bool	   done;			// znacznik zakoñczenia aplikacji
	DWORD	   dwExStyle;		// rozszerzony styl okna
	DWORD	   dwStyle;			// styl okna
	RECT	   windowRect;		// rozmiar okna

	// parametry okna
	int width = 800;
	int height = 600;
	int bits = 32;

	// fullScreen = TRUE/FALSE;		// Prze³¹cznik aplikacja okienkowa/aplikacja pe³noekranowa

	windowRect.left = (long)0;						// struktura okreœlaj¹ca rozmiary okna
	windowRect.right = (long)width;
	windowRect.top = (long)0;
	windowRect.bottom = (long)height;

	// definicja klasy okna
	windowClass.cbSize = sizeof(WNDCLASSEX);
	windowClass.style = CS_HREDRAW | CS_VREDRAW;
	windowClass.lpfnWndProc = WndProc;
	windowClass.cbClsExtra = 0;
	windowClass.cbWndExtra = 0;
	windowClass.hInstance = hInstance;
	windowClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);	// domyœlna ikona
	windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);		// domyœlny kursor
	windowClass.hbrBackground = NULL;								// bez t³a
	windowClass.lpszMenuName = NULL;								// bez menu
	windowClass.lpszClassName = "MojaKlasa";
	windowClass.hIconSm = LoadIcon(NULL, IDI_WINLOGO);		// logo Windows

	// zarejestrowanie klasy okna
	if (!RegisterClassEx(&windowClass))
		return 0;

	if (fullScreen)								// W³¹czenie trybu pe³noekranowego
	{
		DEVMODE dmScreenSettings;						// tryb urz¹dzenia
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth = width;			// szerokoœæ ekranu
		dmScreenSettings.dmPelsHeight = height;			// wysokoœæ ekranu
		dmScreenSettings.dmBitsPerPel = bits;			// iloœæ bitów na piksel
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		// je¿eli prze³¹czenie trybu na pe³noekranowy nie powiod³o siê, prze³¹czenie z powrotem na tryb okienkowy
		if (ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
		{
			MessageBox(NULL, "Prze³¹czenie trybu wyswietlania nie powiod³o siê", NULL, MB_OK);
			fullScreen = FALSE;
		}
	}

	if (fullScreen)								// Tryb pe³noekranowy?
	{
		dwExStyle = WS_EX_APPWINDOW;				// rozszerzony styl okna
		dwStyle = WS_POPUP;						// styl okna
		ShowCursor(FALSE);						// ukrycie kursora myszy
	}
	else										// Tryb okienkowy?
	{
		dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;	// definicja klasy okna
		dwStyle = WS_OVERLAPPEDWINDOW;					// styl okna
	}

	AdjustWindowRectEx(&windowRect, dwStyle, FALSE, dwExStyle);		// skorygowanie rozmiaru okna
	// utworzenie okna
	hwnd = CreateWindowEx(NULL,									// styl rozszerzony
		"MojaKlasa",							// nazwa klasy
		"Ruch szeœcianu jak w tetrisie",	// nazwa aplikacji
		dwStyle | WS_CLIPCHILDREN |
		WS_CLIPSIBLINGS,
		0, 0,								// wspó³rzêdne x,y
		windowRect.right - windowRect.left,
		windowRect.bottom - windowRect.top,	// szerokoœæ, wysokoœæ
		NULL,									// uchwyt okna nadrzêdnego
		NULL,									// uchwyt menu
		hInstance,							// instancja aplikacji
		NULL);

	if (!hwnd)	// sprawdzenie, czy utworzenie okna nie powiod³o siê (wtedy wartoœæ hwnd równa NULL)
		return 0;

	ShowWindow(hwnd, SW_SHOW);			// wyœwietlenie okna
	UpdateWindow(hwnd);					// aktualizacja okna

	done = false;						// inicjacja zmiennej warunku pêtli przetwarzania komunikatów
	Initialize();						// inicjacja OpenGL

	while (!done)	// pêtla przetwarzania komunikatów
	{
		PeekMessage(&msg, NULL, 0, 0, PM_REMOVE);
		if (msg.message == WM_QUIT)		// aplikacja otrzyma³a komunikat WM_QUIT?
			done = true;				// jeœli tak, to koñczy dzia³anie
		else
		{
			Render();					// jeœli nie to renderuje scenê,
			TranslateMessage(&msg);		// t³umaczy komunikat i wysy³a do systemu
			DispatchMessage(&msg);
		}
	}

	if (fullScreen)						// je¿eli by³ tryb pe³noekranowy
	{
		ChangeDisplaySettings(NULL, 0);	// to przywrócenie pulpitu
		ShowCursor(TRUE);				// i wskaŸnika myszy
	}

	return msg.wParam;
}