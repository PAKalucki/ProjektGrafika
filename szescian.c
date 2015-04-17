/*
     Prosty studencki program graficzny 
	 kodowany w jezyku C z uzyciem OpenGL 
*/

#include "szescian.h"
#define PI 3.1415;

LONG WINAPI WndProc( HWND, UINT, WPARAM, LPARAM );
HGLRC SetUpOpenGL( HWND hWnd );

// Wartosci poczatkowe
#define	DEFAULT_Z_DIST		25
#define DEFAULT_PIVOT_VERT_ANGLE	0
#define DEFAULT_PIVOT_HORIZ_ANGLE	0
#define DEFAULT_PIVOT_X				0
#define DEFAULT_PIVOT_Y				0
GLfloat move = -18;
GLfloat move2 = 0;
float z_dist=DEFAULT_Z_DIST;						// INSERT, PAGE UP
float pivot_vert_angle=DEFAULT_PIVOT_VERT_ANGLE;	// UP, DOWN
float pivot_horiz_angle=DEFAULT_PIVOT_HORIZ_ANGLE;	// LEFT, RIGHT
float pivot_x=DEFAULT_PIVOT_X;						// DELETE, PAGE DOWN
float pivot_y=DEFAULT_PIVOT_Y;						// HOME, END


// ************ Deklaracje funkcji ************************

void DrawOpenGLScene( void );  // glowna funkcja rysujaca

//   Skladowe funkcje obiektu graficznego

void szescian(void);
void kregiel(void);
// ..............................

//******** Fukcje skladowe ********************************
void draw(ObjModel *model)
{
int i,j;
if (model != NULL)
	{
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		{
		glColor3f(0.0f,0.0f,0.0f);
		for(i=0;i<model->NumTriangle;i++)
		{
		glBegin(GL_TRIANGLES);
			for(j=0;j<3;j++)
			{
			glVertex3f(model->VertexArray[model->TriangleArray[i].Vertex[j]-1].X,model->VertexArray[model->TriangleArray[i].Vertex[j]-1].Y,model->VertexArray[model->TriangleArray[i].Vertex[j]-1].Z);
			}
			glEnd();
		}
		}
	}
}
void kolizja(float p1_x, float p1_y, float p1_z, float p2_x, float p2_y, float p2_z)
{
	float dystans=0;
	float kolor;

	dystans = abs(sqrt(pow((p1_x-p2_x),2)+pow((p1_y-p2_y),2)+pow((p1_z-p2_z),2)));
	if(dystans>=0.8)
		kolor=0.0;
	else
		kolor=10.0;

	glColor3f(kolor, kolor, kolor);
	glBegin(GL_POLYGON);
		glVertex3f(1.0f,0.0f,-20.0f);
		glVertex3f(1.0f,1.0f,-20.0f);
		glVertex3f(-1.0f,1.0f,-20.0f);
		glVertex3f(-1.0f,0.0f,-20.0f);
	glEnd();

}
void siatka (void)
{
        int i;
	glColor3f(.8,.8,.8);
glBegin(GL_QUADS);
glVertex3f( -1,-0.001, 1);
glVertex3f( -1,-0.001,-17);
glVertex3f(7,-0.001,-17);
glVertex3f(7,-0.001,1);
glEnd();

glBegin(GL_LINES);
for(i=0;i<=7;i++) {	
    if (i==0) { 
		glColor3f(.8,.3,.3);
		glVertex3f(-1,0,1);
		glVertex3f(-1,0,-17);
		i++;
	}
	if(i>0&&i<7){
		glColor3f(.25,.25,.25);
		glVertex3f(i,0,1);
		glVertex3f(i,0,-17);
	}
	if (i==7) { 
		glColor3f(.8,.3,.3);
		glVertex3f(7,0,1);
		glVertex3f(7,0,-17);
		i++;
	}
	i++;
}
for(i=0;i<=17;i++) {
    if (i==0) { 
		glColor3f(.8,.3,.3);
		glVertex3f(-1,0,1);
		glVertex3f(7,0,1);
		i++;
	}
	if(i>0&&i<17){
		glColor3f(.25,.25,.25);
		glVertex3f(-1,0,-i);
		glVertex3f(7,0,-i);
	}
	if(i==17){
		glColor3f(.8,.3,.3);
		glVertex3f(-1,0,-17);
		glVertex3f(7,0,-17);
	}
	i++;
};

glEnd();
}
int random(int min, int max)
{
    int tmp;
    if (max>=min)
        max-= min;
    else
    {
        tmp= min - max;
        min= max;
        max= tmp;
    }
    return max ? (rand() % max + min) : min;
}
//******************************************************** 
//  Glowna funkcja WINDOWS
//******************************************************** 

int WINAPI WinMain (HINSTANCE hInstance,
					HINSTANCE hPrevInstance,
                    LPSTR lpszCmdLine,
					int nCmdShow)
{
    static char szAppName[] = "OpenGL";                    
    static char szTitle[]=    "SZESCIAN"; // Nazwa okna
    WNDCLASS wc;   // zmienna klasy okna
    MSG      msg;  // zmienna komunikatu 
    HWND     hWnd; // uchwyt okna

    // wypelnianie klasy okna 

    wc.style         =
        CS_HREDRAW | CS_VREDRAW;// styl okna
    wc.lpfnWndProc   = 
        (WNDPROC)WndProc;       // procedura okna
    wc.cbClsExtra    = 0;       // dodatkowe dane
    wc.cbWndExtra    = 0;       
    wc.hInstance     =
        hInstance;              // instancja 
    wc.hIcon         = NULL;    // nazwa ikony 
    wc.hCursor       =
        LoadCursor(NULL, IDC_ARROW);// kursor
    wc.hbrBackground = 
        (HBRUSH)(COLOR_WINDOW+1);// domyslny kolor okna
    wc.lpszMenuName  = NULL;     
    wc.lpszClassName =
        szAppName;               // nazwa klasy

    // Rejestracja klasy okna

    RegisterClass( &wc );
  
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
    if ( !hWnd )
        {
        return( 0 );
        }

    
	ShowWindow( hWnd, nCmdShow );// Wyswietlanie okna
    UpdateWindow( hWnd );        // Aktualizacja okna

    // Uruchiomienie petli komunikatow
    while (GetMessage(&msg, NULL, 0, 0)) 
	{
        TranslateMessage( &msg ); // Przetwarzanie wiadomosci
        DispatchMessage( &msg );  // Zwolnienie wiadomosci
    }

    return( msg.wParam ); 
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
           hRC = SetUpOpenGL( hWnd );	// Inicjalizacja OpenGL
           return 0;
    
       case WM_SIZE:
            hDC = GetDC (hWnd);			// Kontekst urzadzenia
            wglMakeCurrent (hDC, hRC);
        
            // Usawienie rozmiarow okna graficznego
            
			glnWidth = (GLsizei) LOWORD (lParam);
            glnHeight = (GLsizei) HIWORD (lParam);
            
			// Aspekt DC
			gldAspect = (GLdouble)glnWidth/(GLdouble)glnHeight;
       
            glMatrixMode( GL_PROJECTION );// Ustawienie macierzy projekcji
            
			glLoadIdentity();
            
			// Rodzaj transformacji ekranowej
            gluPerspective(
                30.0,      // kat 
                gldAspect, // aspekt
                1.0,       // polozenie plaszczyzny blizszej
                1000.0 );  // polozenie plaszczyzny dalszej

            glViewport( 0, 0, glnWidth, glnHeight );
            
			// czyszczenie
			glClearColor(0.5f,0.5f,0.5f,1.0f);
			
			glEnable(GL_COLOR_MATERIAL);
			wglMakeCurrent( NULL, NULL );
            ReleaseDC( hWnd, hDC );
            return 0;

        case WM_PAINT:
            // rysowanie sceny

            hDC = BeginPaint( hWnd, &ps );
            wglMakeCurrent( hDC, hRC );
            
            DrawOpenGLScene();
			
			SwapBuffers(hDC);
            
            wglMakeCurrent( NULL, NULL );

            EndPaint( hWnd, &ps );
            return 0;       
            
        case WM_DESTROY:
            // zamykanie 
            wglDeleteContext( hRC );
            PostQuitMessage( 0 );
            return 0;

		case WM_KEYDOWN: //************ OBSLUGA KLAWISZY ***********************
			if (GetAsyncKeyState(VK_SHIFT) & 0x8000)
				change=1.0f;
			else
				change=0.1f;

			switch ((int)wParam)
			{
				// obrot w pionie
				case VK_UP:
					pivot_vert_angle+=5;
					if (pivot_vert_angle>=360)
						pivot_vert_angle-=360;
					InvalidateRect(hWnd, NULL, FALSE);
					break;
				case VK_DOWN:
					pivot_vert_angle-=5;
					if (pivot_vert_angle<0)
						pivot_vert_angle+=360;
					InvalidateRect(hWnd, NULL, FALSE);
					break;

				// obrot w poziomie
				
				case VK_RIGHT:
					pivot_horiz_angle+=5;
					if (pivot_horiz_angle>=360)
						pivot_horiz_angle-=360;
					InvalidateRect(hWnd, NULL, FALSE);
					break;
				case VK_LEFT:
					pivot_horiz_angle-=5;
					if (pivot_horiz_angle<0)
						pivot_horiz_angle+=360;
					InvalidateRect(hWnd, NULL, FALSE);
					break;
					
				//ruch kuli

				case 0x57: //przod
					if(move<0)
						move+=0.25;
					if(move>0)
						move=0;
					move+=0.25;
					InvalidateRect(hWnd, NULL, FALSE);
					break;

				case 0x53: //tyl
					if(move>-17)
						move-=0.25;
					if(move<-17)
						move=-17;
					move-=0.25;
					InvalidateRect(hWnd, NULL, FALSE);
					
					break;
				case 0x41:
					if(move2<7)
						move2+=0.25;
					if(move2>7)
						move2=7;
					move2+=0.25;
					InvalidateRect(hWnd, NULL, FALSE);
					break;
				case 0x44:
					if(move2>-1)
						move2-=0.25;
					if(move2<-1)
						move2=-1;
					move2-=0.25;
					InvalidateRect(hWnd, NULL, FALSE);
					break;
				// przesuniecia w poziomie
				case VK_NEXT:
					if ((pivot_x+change)<400)
						pivot_x+=change;
					InvalidateRect(hWnd, NULL, FALSE);
					break;
				case VK_PRIOR:
					if ((pivot_x-change)>-400)
						pivot_x-=change;
					InvalidateRect(hWnd, NULL, FALSE);
					break;

				// przesuniecia w pionie
				case VK_HOME:
					if ((pivot_y+change)<400)
						pivot_y+=change;
					InvalidateRect(hWnd, NULL, FALSE);
					break;
				case VK_END:
					if ((pivot_y-change)>-400)
						pivot_y-=change;
					InvalidateRect(hWnd, NULL, FALSE);
					break;

				// odleglosc
				case VK_INSERT:
					if ((z_dist+change)<600)
						z_dist+=change;
					InvalidateRect(hWnd, NULL, FALSE);
					break;
				case VK_DELETE:
					if ((z_dist-change)>1)
						z_dist-=change;
					InvalidateRect(hWnd, NULL, FALSE);
					break;

				// wyjscie z programu
				case VK_ESCAPE:
					PostMessage(hWnd, WM_CLOSE, 0, 0);
					return 0;
			}
        }

    
    return DefWindowProc( hWnd, msg, wParam, lParam );
}



//*******************************************************
//  Uruchomienie OpenGL w srodowisku Windows wymaga 
//  wlaczenie trybu pikselowego oraz ustawienia 
//  kontekstu renderowania
//*******************************************************

HGLRC SetUpOpenGL( HWND hWnd )
{
    static PIXELFORMATDESCRIPTOR pfd =
	{
        sizeof (PIXELFORMATDESCRIPTOR), // rozmiar struktury 
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

    hDC = GetDC( hWnd );
    
	// Inicjalizacja trybu pikselowego 

	nMyPixelFormatID = ChoosePixelFormat( hDC, &pfd );
    SetPixelFormat( hDC, nMyPixelFormatID, &pfd );
	
	// Inicjalizacja kontekstu renderowania
    
	hRC = wglCreateContext( hDC );
    ReleaseDC( hWnd, hDC );

    return hRC;
}



//******************************************************** 
//  Glowna funkcja rysujaca.
//******************************************************** 

void DrawOpenGLScene( )
{
	char * memory;
	size_t bytes;
	char * memory2;
	size_t bytes2;
	ObjModel* kulaobj;
	ObjModel* kregielobj[4];
	int x;
	GLfloat p_x=1,zmiana;
	GLfloat position[4]={10.0f, 10.0, 100.0f, 0.0f};
    
	// flagi czynnosci pomocniczych

    glEnable( GL_DEPTH_TEST );
    
	// czyszczenie buforow
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	
	//glEnable(GL_CULL_FACE);
	glEnable(GL_COLOR_MATERIAL);

	glutInitDisplayMode(GLUT_SINGLE|GLUT_RGBA);
	
	// wlaczenie oswietlenia
	glLightfv(GL_LIGHT0, GL_POSITION, position);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);

    memory=NULL;
	bytes = ObjLoadFile("kulaopt.obj",&memory);
	kulaobj = ObjLoadModel(memory,bytes);
	bytes2 = ObjLoadFile("kregielobj.obj",&memory2);
	for(x=0;x<4;x++)
		kregielobj[x] = ObjLoadModel(memory2,bytes2);
    // transformacja obserwacji
    
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();

    // umiejscowienie kamery w odleglym miejscu
  	glTranslatef( pivot_x, pivot_y, -z_dist );
	glRotatef(pivot_vert_angle, 1, 0, 0);
	glRotatef(pivot_horiz_angle, 0, 1, 0);
	glPushMatrix();
	glPushMatrix();
	while(1)
	{
		move=move+0.00001;
		zmiana=random(0,1);
		if(zmiana==0&&move<=7)
			move2+=0.1;
		else
			if(move2>=-1)
				move2-=0.1;
		if(move>=0)
			break;
		glTranslatef(move2, -0.14, move);// sterowanie kula + -	
	}
	glPopMatrix();
	glTranslatef(0.35, 0.08, 0.14);
	for(x=0;x<4;x++)
	{
		if(p_x==1)
			p_x=0;
		else
			glTranslatef(2,0,0);
		draw(kregielobj[x]);
	}
	glPopMatrix();
	glPushMatrix();
		kolizja(move2,0.5,move,0.2,0.5,0.14);
	glPopMatrix();
	siatka();
    glFlush ();
	//glutMainLoop();
	//glutPostRedisplay();
}
