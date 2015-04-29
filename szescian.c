/*
Prosty studencki program graficzny
kodowany w jezyku C z uzyciem OpenGL
*/
#include <stdio.h>
#include "szescian.h"
#define PI 3.1415;

LONG WINAPI WndProc(HWND, UINT, WPARAM, LPARAM);
HGLRC SetUpOpenGL(HWND hWnd);

// Wartosci poczatkowe
#define	DEFAULT_Z_DIST		25
#define DEFAULT_PIVOT_VERT_ANGLE	0
#define DEFAULT_PIVOT_HORIZ_ANGLE	0
#define DEFAULT_PIVOT_X				0
#define DEFAULT_PIVOT_Y				0
#define IDM_RED 0
#define IDM_FALL 1
float z_dist = DEFAULT_Z_DIST;						// INSERT, PAGE UP
float pivot_vert_angle = DEFAULT_PIVOT_VERT_ANGLE;	// UP, DOWN
float pivot_horiz_angle = DEFAULT_PIVOT_HORIZ_ANGLE;	// LEFT, RIGHT
float pivot_x = DEFAULT_PIVOT_X;						// DELETE, PAGE DOWN
float pivot_y = DEFAULT_PIVOT_Y;						// HOME, END
int collisionEvent;
int speed = 100;

//kolizje
float posx = 0, posy = 0, posz = -18; // kula

float pos11x = 0.35, pos11y = -0.4, pos11z = 0.14; // 1 rzad 1 kregiel
float pos12x = 2.35, pos12y = -0.4, pos12z = 0.14; // 1 rzad 2 kregiel
float pos13x = 4.35, pos13y = -0.4, pos13z = 0.14; // 1 rzad 3 kregiel
float pos14x = 6.35, pos14y = -0.4, pos14z = 0.14; // 1 rzad 4 kregiel

float pos21x = 1.35, pos21y = -0.4, pos21z = -1.14; // 2 rzad 1 kregiel
float pos22x = 3.35, pos22y = -0.4, pos22z = -1.14; // 2 rzad 2 kregiel
float pos23x = 5.35, pos23y = -0.4, pos23z = -1.14; // 2 rzad 3 kregiel

float pos31x = 2.35, pos31y = -0.4, pos31z = -2.14; // 3 rzad 1 kregiel
float pos32x = 4.35, pos32y = -0.4, pos32z = -2.14; // 3 rzad 2 kregiel

float pos41x = 3.35, pos41y = -0.4, pos41z = -3.14; // 4 rzad 1 kregiel





// ************ Deklaracje funkcji ************************

void DrawOpenGLScene(void);  // glowna funkcja rysujaca

//   Skladowe funkcje obiektu graficznego

void szescian(void);
void kregiel(void);
// ..............................

//Hitboxy kolizji



	


//******** Fukcje skladowe ********************************
void draw(ObjModel *model)
{
	int i, j;
	if (model != NULL)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		{
			//glColor3f(0.0f, 0.0f, 0.0f);
			for (i = 0; i<model->NumTriangle; i++)
			{
				glBegin(GL_TRIANGLES);
				for (j = 0; j<3; j++)
				{
					glVertex3f(model->VertexArray[model->TriangleArray[i].Vertex[j] - 1].X, model->VertexArray[model->TriangleArray[i].Vertex[j] - 1].Y, model->VertexArray[model->TriangleArray[i].Vertex[j] - 1].Z);
				}
				glEnd();
			}
		}
	}
}
int kolizja(float p1_x, float p1_y, float p1_z, float p2_x, float p2_y, float p2_z)
{
	float dystans = 0;

	dystans = abs(sqrt(pow((p1_x - p2_x), 2) + pow((p1_y - p2_y), 2) + pow((p1_z - p2_z), 2)));
	if (dystans >= 0.3)
		return 0;
	else
		return 1;
}

int kolizja_1kregiel(void)
{
	if (kolizja(posx, posy, posz, pos11x, pos11y, pos11z) == 1
		|| kolizja(pos11x, pos11y, pos11z, pos12x, pos12y, pos12z) == 1
		|| kolizja(pos11x, pos11y, pos11z, pos13x, pos13y, pos13z) == 1
		|| kolizja(pos11x, pos11y, pos11z, pos14x, pos14y, pos14z) == 1
		|| kolizja(pos11x, pos11y, pos11z, pos21x, pos21y, pos21z) == 1
		|| kolizja(pos11x, pos11y, pos11z, pos22x, pos22y, pos22z) == 1
		|| kolizja(pos11x, pos11y, pos11z, pos23x, pos23y, pos23z) == 1
		|| kolizja(pos11x, pos11y, pos11z, pos31x, pos31y, pos31z) == 1
		|| kolizja(pos11x, pos11y, pos11z, pos32x, pos32y, pos32z) == 1
		|| kolizja(pos11x, pos11y, pos11z, pos41x, pos41y, pos41z) == 1)
		return 1;
	else return 0;
}

int kolizja_2kregiel(void)
{

	if (kolizja(posx, posy, posz, pos12x, pos12y, pos12z)==1
		|| kolizja(pos12x, pos12y, pos12z, pos13x, pos13y, pos13z) == 1
		|| kolizja(pos12x, pos12y, pos12z, pos14x, pos14y, pos14z) == 1
		|| kolizja(pos12x, pos12y, pos12z, pos21x, pos21y, pos21z) == 1
		|| kolizja(pos12x, pos12y, pos12z, pos22x, pos22y, pos22z) == 1
		|| kolizja(pos12x, pos12y, pos12z, pos23x, pos23y, pos23z) == 1
		|| kolizja(pos12x, pos12y, pos12z, pos31x, pos31y, pos31z) == 1
		|| kolizja(pos12x, pos12y, pos12z, pos32x, pos32y, pos32z) == 1
		|| kolizja(pos12x, pos12y, pos12z, pos41x, pos41y, pos41z) == 1
		)
		return 1;
	else return 0;

}

int kolizja_3kregiel(void)
{
	if (kolizja(posx, posy, posz, pos13x, pos13y, pos13z)==1
		|| kolizja(pos13x, pos13y, pos13z, pos14x, pos14y, pos14z) == 1
		|| kolizja(pos13x, pos13y, pos13z, pos21x, pos21y, pos21z) == 1
		|| kolizja(pos13x, pos13y, pos13z, pos22x, pos22y, pos22z) == 1
		|| kolizja(pos13x, pos13y, pos13z, pos23x, pos23y, pos23z) == 1
		|| kolizja(pos13x, pos13y, pos13z, pos31x, pos31y, pos31z) == 1
		|| kolizja(pos13x, pos13y, pos13z, pos32x, pos32y, pos32z) == 1
		|| kolizja(pos13x, pos13y, pos13z, pos41x, pos41y, pos41z) == 1
		)
		return 1;
	else return 0;
}

int kolizja_4kregiel(void)
{
	if (kolizja(posx, posy, posz, pos14x, pos14y, pos14z) == 1
		|| kolizja(pos14x, pos14y, pos14z, pos21x, pos21y, pos21z) == 1
		|| kolizja(pos14x, pos14y, pos14z, pos22x, pos22y, pos22z) == 1
		|| kolizja(pos14x, pos14y, pos14z, pos23x, pos23y, pos23z) == 1
		|| kolizja(pos14x, pos14y, pos14z, pos31x, pos31y, pos31z) == 1
		|| kolizja(pos14x, pos14y, pos14z, pos32x, pos32y, pos32z) == 1
		|| kolizja(pos14x, pos14y, pos14z, pos41x, pos41y, pos41z) == 1
		)
		return 1;
	else return 0;
}

int kolizja_5kregiel(void)
{
	if (kolizja(posx, posy, posz, pos21x, pos21y, pos21z)==1
		|| kolizja(pos21x, pos21y, pos21z, pos22x, pos22y, pos22z) == 1
		|| kolizja(pos21x, pos21y, pos21z, pos23x, pos23y, pos23z) == 1
		|| kolizja(pos21x, pos21y, pos21z, pos31x, pos31y, pos31z) == 1
		|| kolizja(pos21x, pos21y, pos21z, pos32x, pos32y, pos32z) == 1
		|| kolizja(pos21x, pos21y, pos21z, pos41x, pos41y, pos41z) == 1
		)
		return 1;
	else return 0;
}

int kolizja_6kregiel(void)
{
	if (kolizja(posx, posy, posz, pos22x, pos22y, pos22z)==1
		|| kolizja(pos22x, pos22y, pos22z, pos23x, pos23y, pos23z) == 1
		|| kolizja(pos22x, pos22y, pos22z, pos31x, pos31y, pos31z) == 1
		|| kolizja(pos22x, pos22y, pos22z, pos32x, pos32y, pos32z) == 1
		|| kolizja(pos22x, pos22y, pos22z, pos41x, pos41y, pos41z) == 1
		)
		return 1;
	else return 0;
}

int kolizja_7kregiel(void)
{
	if (kolizja(posx, posy, posz, pos23x, pos23y, pos23z)==1
		|| kolizja(pos23x, pos23y, pos23z, pos31x, pos31y, pos31z) == 1
		|| kolizja(pos23x, pos23y, pos23z, pos32x, pos32y, pos32z) == 1
		|| kolizja(pos23x, pos23y, pos23z, pos41x, pos41y, pos41z) == 1
		)
		return 1;
	else return 0;

}

int kolizja_8kregiel(void)
{
	if (kolizja(posx, posy, posz, pos31x, pos31y, pos31z)==1
		|| kolizja(pos31x, pos31y, pos31z, pos32x, pos32y, pos32z) == 1
		|| kolizja(pos31x, pos31y, pos31z, pos41x, pos41y, pos41z) == 1
		)
		return 1;
	else return 0;
}

int kolizja_9kregiel(void)
{
	if (kolizja(posx, posy, posz, pos32x, pos32y, pos32z) == 1
		|| kolizja(pos32x, pos32y, pos32z, pos41x, pos41y, pos41z) ==1)
		return 1;
	else return 0;
}

int kolizja_10kregiel(void)
{
	if (kolizja(posx, posy, posz, pos41x, pos41y, pos41z)==1)
		return 1;
	else return 0;
}


/*
void hitbox(void)
{

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	GLUquadricObj *quadric2 = gluNewQuadric();
	glColor4d(0.0, 0.0, 0.0, 0.0);
	glBegin(GL_QUAD_STRIP);
	gluCylinder(quadric2, 0.35, 0.35, 2.1, 30, 30);
	glEnd();
	glDisable(GL_BLEND);

}
*/


void siatka(void)
{
	int i;
	glColor3f(.8, .8, .8);
	glBegin(GL_QUADS);
	glVertex3f(-1, -0.55, 1);
	glVertex3f(-1, -0.55, -25);
	glVertex3f(7, -0.55, -25);
	glVertex3f(7, -0.55, 1);
	glEnd();

	glEnd();
}
int random(int min, int max)
{
	return rand() % ((max - min) + 1) + min;
}
//menu
void AddMenus(HWND hwnd)
{
	HMENU hMenubar;
	HMENU hMenu;

	hMenubar = CreateMenu();
	hMenu = CreateMenu();

	AppendMenuW(hMenu, MF_STRING, IDM_RED, L"&Czerwone");
	AppendMenuW(hMenu, MF_STRING, IDM_FALL, L"&Przewracanie");

	AppendMenuW(hMenubar, MF_POPUP, (UINT_PTR)hMenu, L"&Kolizja");
	SetMenu(hwnd, hMenubar);
}



//******************************************************** 
//  Glowna funkcja WINDOWS
//********************************************************

int WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpszCmdLine,
	int nCmdShow)
{
	static char szAppName[] = "OpenGL";
	static char szTitle[] = "SZESCIAN"; // Nazwa okna
	WNDCLASS wc;   // zmienna klasy okna
	MSG      msg;  // zmienna komunikatu 
	HWND     hWnd; // uchwyt okna

	// wypelnianie klasy okna 

	wc.style =
		CS_HREDRAW | CS_VREDRAW;// styl okna
	wc.lpfnWndProc =
		(WNDPROC)WndProc;       // procedura okna
	wc.cbClsExtra = 0;       // dodatkowe dane
	wc.cbWndExtra = 0;
	wc.hInstance =
		hInstance;              // instancja 
	wc.hIcon = NULL;    // nazwa ikony 
	wc.hCursor =
		LoadCursor(NULL, IDC_ARROW);// kursor
	wc.hbrBackground =
		(HBRUSH)(COLOR_WINDOW + 1);// domyslny kolor okna
	wc.lpszMenuName = NULL;
	wc.lpszClassName =
		szAppName;               // nazwa klasy

	// Rejestracja klasy okna

	RegisterClass(&wc);

	// Konstrukcja glownego okna 

	hWnd = CreateWindow(
		szAppName, // app name
		szTitle,   // Text for window title bar
		WS_OVERLAPPEDWINDOW// Window style 
		// NEED THESE for OpenGL calls to work!
		| WS_CLIPCHILDREN | WS_CLIPSIBLINGS,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0,
		NULL,     // no parent window
		NULL,     // Use the window class menu.
		hInstance,// This instance owns this window
		NULL      // We don't use any extra data
		);

	// Jezeli okno nie zostanie utworzone, zwrot 0
	if (!hWnd)
	{
		return(0);
	}


	ShowWindow(hWnd, nCmdShow);// Wyswietlanie okna
	UpdateWindow(hWnd);        // Aktualizacja okna

	// Uruchiomienie petli komunikatow
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg); // Przetwarzanie wiadomosci
		DispatchMessage(&msg);  // Zwolnienie wiadomosci
	}

	return(msg.wParam);
}


//******************************************************** 
//    Procedura okna
//******************************************************** 

LONG WINAPI WndProc(HWND hWnd,
	UINT msg,
	WPARAM wParam,
	LPARAM lParam)
{
	HDC hDC;

	static HGLRC hRC; // Note this is STATIC!

	PAINTSTRUCT ps;

	GLdouble gldAspect;

	GLsizei glnWidth, glnHeight;

	float change;


	// Petla komunikatow 

	switch (msg)
	{
	case WM_CREATE:
		hRC = SetUpOpenGL(hWnd);
		AddMenus(hWnd);
		SetTimer(hWnd, 101, speed, NULL);
		// Inicjalizacja OpenGL
		return 0;

	case WM_COMMAND:
		switch (LOWORD(wParam)) 
		{
		case IDM_RED:
			collisionEvent = 0;
			break;
		case IDM_FALL:
			collisionEvent = 1;
			break;
		}
		break;

	case WM_TIMER:
		if (posz<0)
			posz += 0.1;
		if (posz > 0)
		{
			posz = -18;
			posx = 4;
		}
		posz += 0.1;
		InvalidateRect(hWnd, NULL, FALSE);
		return 0;

	case WM_SIZE:
		hDC = GetDC(hWnd);			// Kontekst urzadzenia
		wglMakeCurrent(hDC, hRC);

		// Usawienie rozmiarow okna graficznego

		glnWidth = (GLsizei)LOWORD(lParam);
		glnHeight = (GLsizei)HIWORD(lParam);

		// Aspekt DC
		gldAspect = (GLdouble)glnWidth / (GLdouble)glnHeight;

		glMatrixMode(GL_PROJECTION);// Ustawienie macierzy projekcji

		glLoadIdentity();

		// Rodzaj transformacji ekranowej
		gluPerspective(
			30.0,      // kat 
			gldAspect, // aspekt
			1.0,       // polozenie plaszczyzny blizszej
			1000.0);  // polozenie plaszczyzny dalszej

		glViewport(0, 0, glnWidth, glnHeight);

		// czyszczenie
		glClearColor(0.5f, 0.5f, 0.5f, 1.0f);

		glEnable(GL_COLOR_MATERIAL);
		wglMakeCurrent(NULL, NULL);
		ReleaseDC(hWnd, hDC);
		return 0;

	case WM_PAINT:
		// rysowanie sceny

		hDC = BeginPaint(hWnd, &ps);
		wglMakeCurrent(hDC, hRC);

		DrawOpenGLScene();

		SwapBuffers(hDC);

		wglMakeCurrent(NULL, NULL);

		EndPaint(hWnd, &ps);
		return 0;

	case WM_DESTROY:
		// zamykanie 
		wglDeleteContext(hRC);
		KillTimer(hWnd, 101);
		PostQuitMessage(0);
		return 0;

	case WM_KEYDOWN: //************ OBSLUGA KLAWISZY ***********************
		if (GetAsyncKeyState(VK_SHIFT) & 0x8000)
			change = 1.0f;
		else
			change = 0.1f;

		switch ((int)wParam)
		{
			// obrot w pionie
		case VK_UP:
			pivot_vert_angle += 5;
			if (pivot_vert_angle >= 360)
				pivot_vert_angle -= 360;
			InvalidateRect(hWnd, NULL, FALSE);
			break;
		case VK_DOWN:
			pivot_vert_angle -= 5;
			if (pivot_vert_angle<0)
				pivot_vert_angle += 360;
			InvalidateRect(hWnd, NULL, FALSE);
			break;

			// obrot w poziomie

		case VK_RIGHT:
			pivot_horiz_angle += 5;
			if (pivot_horiz_angle >= 360)
				pivot_horiz_angle -= 360;
			InvalidateRect(hWnd, NULL, FALSE);
			break;
		case VK_LEFT:
			pivot_horiz_angle -= 5;
			if (pivot_horiz_angle<0)
				pivot_horiz_angle += 360;
			InvalidateRect(hWnd, NULL, FALSE);
			break;

			//ruch kuli

		case 0x57: //przod
			if (posz<0)
				posz += 0.25;
			if (posz>0)
				posz = 0;
			posz += 0.25;
			InvalidateRect(hWnd, NULL, FALSE);
			break;

		case 0x53: //tyl
			if (posz>-17)
				posz -= 0.25;
			if (posz<-17)
				posz = -17;
			posz -= 0.25;
			InvalidateRect(hWnd, NULL, FALSE);

			break;
		case 0x41:
			if (posx<7)
				posx += 0.25;
			if (posx>7)
				posx = 7;
			posx += 0.25;
			InvalidateRect(hWnd, NULL, FALSE);
			break;
		case 0x44:
			if (posx>-1)
				posx -= 0.25;
			if (posx<-1)
				posx = -1;
			posx -= 0.25;
			InvalidateRect(hWnd, NULL, FALSE);
			break;
			// przesuniecia w poziomie
		case VK_NEXT:
			if ((pivot_x + change)<400)
				pivot_x += change;
			InvalidateRect(hWnd, NULL, FALSE);
			break;
		case VK_PRIOR:
			if ((pivot_x - change)>-400)
				pivot_x -= change;
			InvalidateRect(hWnd, NULL, FALSE);
			break;

			// przesuniecia w pionie
		case VK_HOME:
			if ((pivot_y + change)<400)
				pivot_y += change;
			InvalidateRect(hWnd, NULL, FALSE);
			break;
		case VK_END:
			if ((pivot_y - change)>-400)
				pivot_y -= change;
			InvalidateRect(hWnd, NULL, FALSE);
			break;

			// odleglosc
		case VK_INSERT:
			if ((z_dist + change)<600)
				z_dist += change;
			InvalidateRect(hWnd, NULL, FALSE);
			break;
		case VK_DELETE:
			if ((z_dist - change)>1)
				z_dist -= change;
			InvalidateRect(hWnd, NULL, FALSE);
			break;

			// wyjscie z programu
		case VK_ESCAPE:
			PostMessage(hWnd, WM_CLOSE, 0, 0);
			return 0;
		}
	}


	return DefWindowProc(hWnd, msg, wParam, lParam);
}



//*******************************************************
//  Uruchomienie OpenGL w srodowisku Windows wymaga 
//  wlaczenie trybu pikselowego oraz ustawienia 
//  kontekstu renderowania
//*******************************************************

HGLRC SetUpOpenGL(HWND hWnd)
{
	static PIXELFORMATDESCRIPTOR pfd =
	{
		sizeof(PIXELFORMATDESCRIPTOR), // rozmiar struktury 
		1,                              // numer wersji
		PFD_DRAW_TO_WINDOW |            // flaga rysowania w oknie,
		PFD_SUPPORT_OPENGL |        // uzycie OpenGL
		PFD_DOUBLEBUFFER,	        // uzycie podwojnego buforowania
		PFD_TYPE_RGBA,                  // model kolorow - RGBA 
		24,                             // kolor 24-bitowy
		0, 0, 0,
		0, 0, 0,                        // trezerwa
		0, 0,                           // brak bufora alpha 
		0, 0, 0, 0, 0,                  // brak bufora akumulacji
		32,                             // bufor glebokosci 32-bitowy
		0,                              // brak bufora szablonu
		0,                              // brak bufora pomocniczego
		PFD_MAIN_PLANE,					// warstwa
		0,                              // rezerwa - 0 
		0,                              // maska
		0,                              // maska widoczna
		0                               // maska bledow
	};

	// zmienne
	int nMyPixelFormatID;

	HDC hDC;

	HGLRC hRC;

	hDC = GetDC(hWnd);

	// Inicjalizacja trybu pikselowego 

	nMyPixelFormatID = ChoosePixelFormat(hDC, &pfd);
	SetPixelFormat(hDC, nMyPixelFormatID, &pfd);

	// Inicjalizacja kontekstu renderowania

	hRC = wglCreateContext(hDC);
	ReleaseDC(hWnd, hDC);

	return hRC;
}



//******************************************************** 
//  Glowna funkcja rysujaca.
//******************************************************** 

void DrawOpenGLScene()
{
	char * memory;
	size_t bytes;
	char * memory2;
	size_t bytes2;
	//ObjModel* kulaobj;
	ObjModel* kregielobj;
	int x;
	GLfloat p_x = 1, zmiana;
	GLfloat position[4] = { 10.0f, 10.0, 100.0f, 0.0f };
	// pozycje do kolizji........................................................

	//.........................................................................
	GLUquadricObj *quadric1 = gluNewQuadric();
	// flagi czynnosci pomocniczych

	glEnable(GL_DEPTH_TEST);

	// czyszczenie buforow
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//glEnable(GL_CULL_FACE);
	glEnable(GL_COLOR_MATERIAL);

	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);

	// wlaczenie oswietlenia
	glLightfv(GL_LIGHT0, GL_POSITION, position);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);

	//memory = NULL;
	memory2 = NULL;
	//bytes = ObjLoadFile("kulaopt.obj", &memory);
	//kulaobj = ObjLoadModel(memory, bytes);
	bytes2 = ObjLoadFile("kregielobj.obj", &memory2);
	kregielobj = ObjLoadModel(memory2, bytes2);
	// transformacja obserwacji

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// umiejscowienie kamery w odleglym miejscu
	glTranslatef(pivot_x, pivot_y, -z_dist);
	glRotatef(pivot_vert_angle, 1, 0, 0);
	glRotatef(pivot_horiz_angle, 0, 1, 0);



	glPushMatrix();
	glPushMatrix();
	glTranslatef(posx, posy, posz);// sterowanie kula + -	
	glRotatef(posx, 1, 0, 0);
	glPushMatrix();
	//draw(kulaobj);
	gluSphere(quadric1, 0.55, 50, 50);
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();


	//1 rząd 4 kregle
	//1
	glPushMatrix();
	glColor3f(0.0f, 0.0f, 1.0f);
	glTranslatef(pos11x, pos11y, pos11z);
	glPushMatrix();
	if (kolizja_1kregiel() == 1)
	{
		if (collisionEvent == 1)
			glRotatef(90, 1, 0, 0);
		else
			glColor3f(1, 0, 0);
	}
	draw(kregielobj);
	glPopMatrix();
	glPopMatrix();

	//2
	glPushMatrix();
	glColor3f(0.0f, 1.0f, 1.0f);
	glTranslatef(pos12x, pos12y, pos12z);
	glPushMatrix();
	if (kolizja_2kregiel() == 1)
		glRotatef(90, random(0, 1), random(0, 1), random(0, 1));
	draw(kregielobj);
	glPopMatrix();
	glPopMatrix();


	//3
	glPushMatrix(); glColor3f(1.0f, 1.0f, 1.0f);
	glTranslatef(pos13x, pos13y, pos13z);
	glPushMatrix();
		if (kolizja_3kregiel() == 1)
			glRotatef(90, random(0, 1), random(0, 1), random(0, 1));
	draw(kregielobj);
	glPopMatrix();
	glPopMatrix();

	//4
	glPushMatrix();
	glColor3f(1.0f, 1.0f, 2.0f);
	glTranslatef(pos14x, pos14y, pos14z);
	glPushMatrix();
	if (kolizja_4kregiel() == 1)
		glRotatef(90, random(0, 1), random(0, 1), random(0, 1));
	draw(kregielobj);
	glPopMatrix();
	glPopMatrix();

	//2 rzad 3 kregle

	//1
	glPushMatrix();
	glColor3f(1.0f, 2.0f, 2.0f);
	glTranslatef(pos21x, pos21y, pos21z);
	glPushMatrix();
	if (kolizja_5kregiel() == 1)
	{
		glRotatef(90, random(0, 1), random(0, 1), random(0, 1));
		glColor3f(1, 0, 0);
	}
	draw(kregielobj);
	glPopMatrix();
	glPopMatrix();

	//2
	glPushMatrix();
	glColor3f(2.0f, 2.0f, 2.0f);
	glTranslatef(pos22x, pos22y, pos22z);
	glPushMatrix();
	if (kolizja_6kregiel() == 1)
		glRotatef(90, random(0, 1), random(0, 1), random(0, 1));
	draw(kregielobj);
	glPopMatrix();
	glPopMatrix();

	//3
	glPushMatrix();
	glColor3f(2.0f, 2.0f, 3.0f);
	glTranslatef(pos23x, pos23y, pos23z);
	glPushMatrix();
	if (kolizja_7kregiel() == 1)
		glRotatef(90, random(0, 1), random(0, 1), random(0, 1));
	draw(kregielobj);
	glPopMatrix();
	glPopMatrix();

	//3 rzad 2 kregle

	//1
	glPushMatrix();
	glColor3f(2.0f, 3.0f, 3.0f);
	glTranslatef(pos31x, pos31y, pos31z);
	glPushMatrix();
	if (kolizja_8kregiel() == 1)
		glRotatef(90, random(0, 1), random(0, 1), random(0, 1));
	draw(kregielobj);
	glPopMatrix();
	glPopMatrix();

	//2
	glPushMatrix();
	glColor3f(3.0f, 3.0f, 3.0f);
	glTranslatef(pos32x, pos32y, pos32z);
	glPushMatrix();
	if (kolizja_9kregiel() == 1)
		glRotatef(90, random(0, 1), random(0, 1), random(0, 1));
	draw(kregielobj);
	glPopMatrix();
	glPopMatrix();

	//4rzad 1 kregiel

	//1
	glPushMatrix();
	glColor3f(3.0f, 3.0f, 4.0f);
	glTranslatef(pos41x, pos41y, pos41z);
	glPushMatrix();
	if (kolizja_10kregiel() == 1)
		glRotatef(90, random(0, 1), random(0, 1), random(0, 1));
	draw(kregielobj);
	glPopMatrix();
	glPopMatrix();

	glPushMatrix();
	siatka();
	glPopMatrix();
	glFlush();
	gluDeleteQuadric(quadric1);
}
