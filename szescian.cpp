/*
Prosty studencki program graficzny
kodowany w jezyku C z uzyciem OpenGL
*/
#define _CRT_SECURE_NO_WARNINGS
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
GLfloat move = -15;
float z_dist = DEFAULT_Z_DIST;						// INSERT, PAGE UP
float pivot_vert_angle = DEFAULT_PIVOT_VERT_ANGLE;	// UP, DOWN
float pivot_horiz_angle = DEFAULT_PIVOT_HORIZ_ANGLE;	// LEFT, RIGHT
float pivot_x = DEFAULT_PIVOT_X;						// DELETE, PAGE DOWN
float pivot_y = DEFAULT_PIVOT_Y;						// HOME, END


// ************ Deklaracje funkcji ************************

void DrawOpenGLScene(void);  // glowna funkcja rysujaca

//   Skladowe funkcje obiektu graficznego
void siatka(void);
void szescian(void);
void kregiel(void);
// ..............................

//******** Fukcje skladowe ********************************

struct coordinate{
	float x, y, z;
	coordinate(float a, float b, float c) : x(a), y(b), z(c) {};
};

//for faces, it can contain triangles and quads as well, the four variable contain which is that
struct face{
	int facenum;
	bool four;
	int faces[4];
	face(int facen, int f1, int f2, int f3) : facenum(facen){  //constructor for triangle
		faces[0] = f1;
		faces[1] = f2;
		faces[2] = f3;
		four = false;
	}
	face(int facen, int f1, int f2, int f3, int f4) : facenum(facen){ //overloaded constructor for quad
		faces[0] = f1;
		faces[1] = f2;
		faces[2] = f3;
		faces[3] = f4;
		four = true;
	}
};

int loadObject(const char* filename)
{
	std::vector<std::string*> coord;        //read every single line of the obj file as a string
	std::vector<coordinate*> vertex;
	std::vector<face*> faces;
	std::vector<coordinate*> normals;       //normal vectors for every face
	std::ifstream in(filename);     //open the .obj file
	if (!in.is_open())       //if not opened, exit with -1
	{
		std::cout << "Nor oepened" << std::endl;
		return -1;
	}
	char buf[256];
	//read in every line to coord
	while (!in.eof())
	{
		in.getline(buf, 256);
		coord.push_back(new std::string(buf));
	}
	//go through all of the elements of coord, and decide what kind of element is that
	for (int i = 0; i<coord.size(); i++)
	{
		if (coord[i]->c_str()[0] == '#')   //if it is a comment (the first character is #)
			continue;       //we don't care about that
		else if (coord[i]->c_str()[0] == 'v' && coord[i]->c_str()[1] == ' ') //if vector
		{
			float tmpx, tmpy, tmpz;
			sscanf(coord[i]->c_str(), "v %f %f %f", &tmpx, &tmpy, &tmpz);       //read in the 3 float coordinate to tmpx,tmpy,tmpz
			vertex.push_back(new coordinate(tmpx, tmpy, tmpz));       //and then add it to the end of our vertex list
		}
		else if (coord[i]->c_str()[0] == 'v' && coord[i]->c_str()[1] == 'n')        //if normal vector
		{
			float tmpx, tmpy, tmpz;   //do the same thing
			sscanf(coord[i]->c_str(), "vn %f %f %f", &tmpx, &tmpy, &tmpz);
			normals.push_back(new coordinate(tmpx, tmpy, tmpz));
		}
		else if (coord[i]->c_str()[0] == 'f')     //if face
		{
			int a, b, c, d, e;
			if (count(coord[i]->begin(), coord[i]->end(), ' ') == 3)     //if it is a triangle (it has 3 space in it)
			{
				sscanf(coord[i]->c_str(), "f %d//%d %d//%d %d//%d", &a, &b, &c, &b, &d, &b);
				faces.push_back(new face(b, a, c, d));     //read in, and add to the end of the face list
			}
			else{
				sscanf(coord[i]->c_str(), "f %d//%d %d//%d %d//%d %d//%d", &a, &b, &c, &b, &d, &b, &e, &b);
				faces.push_back(new face(b, a, c, d, e));   //do the same, except we call another constructor, and we use different pattern
			}
		}
	}
	//raw
	int num;        //the id for the list
	num = glGenLists(1);      //generate a uniqe
	glNewList(num, GL_COMPILE);      //and create it
	for (int i = 0; i<faces.size(); i++)
	{
		if (faces[i]->four)      //if it's a quad draw a quad
		{
			glBegin(GL_QUADS);
			//basically all I do here, is use the facenum (so the number of the face) as an index for the normal, so the 1st normal owe to the first face
			//I subtract 1 because the index start from 0 in C++
			glNormal3f(normals[faces[i]->facenum - 1]->x, normals[faces[i]->facenum - 1]->y, normals[faces[i]->facenum - 1]->z);
			//draw the faces
			glVertex3f(vertex[faces[i]->faces[0] - 1]->x, vertex[faces[i]->faces[0] - 1]->y, vertex[faces[i]->faces[0] - 1]->z);
			glVertex3f(vertex[faces[i]->faces[1] - 1]->x, vertex[faces[i]->faces[1] - 1]->y, vertex[faces[i]->faces[1] - 1]->z);
			glVertex3f(vertex[faces[i]->faces[2] - 1]->x, vertex[faces[i]->faces[2] - 1]->y, vertex[faces[i]->faces[2] - 1]->z);
			glVertex3f(vertex[faces[i]->faces[3] - 1]->x, vertex[faces[i]->faces[3] - 1]->y, vertex[faces[i]->faces[3] - 1]->z);
			glEnd();
		}
		else{
			glBegin(GL_TRIANGLES);
			glNormal3f(normals[faces[i]->facenum - 1]->x, normals[faces[i]->facenum - 1]->y, normals[faces[i]->facenum - 1]->z);
			glVertex3f(vertex[faces[i]->faces[0] - 1]->x, vertex[faces[i]->faces[0] - 1]->y, vertex[faces[i]->faces[0] - 1]->z);
			glVertex3f(vertex[faces[i]->faces[1] - 1]->x, vertex[faces[i]->faces[1] - 1]->y, vertex[faces[i]->faces[1] - 1]->z);
			glVertex3f(vertex[faces[i]->faces[2] - 1]->x, vertex[faces[i]->faces[2] - 1]->y, vertex[faces[i]->faces[2] - 1]->z);
			glEnd();
		}
	}
	glEndList();
	//delete everything to avoid memory leaks
	for (int i = 0; i<coord.size(); i++)
		delete coord[i];
	for (int i = 0; i<faces.size(); i++)
		delete faces[i];
	for (int i = 0; i<normals.size(); i++)
		delete normals[i];
	for (int i = 0; i<vertex.size(); i++)
		delete vertex[i];
	return num;     //return with the id
}

/*void szescian(void)
{
glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
{
// Parametry wierzcholkow

GLfloat sa[3]={0.0f,0.0f,0.0f};
GLfloat sb[3]={1.0f,0.0f,0.0f};
GLfloat sc[3]={1.0f,1.0f,0.0f};
GLfloat sd[3]={0.0f,1.0f,0.0f};
GLfloat se[3]={0.0f,0.0f,-1.0f};
GLfloat sf[3]={1.0f,0.0f,-1.0f};
GLfloat sg[3]={1.0f,1.0f,-1.0f};
GLfloat sh[3]={0.0f,1.0f,-1.0f};

// Sciany skladowe
glColor3f(1.0f, 0.0f, 0.0f);
glBegin(GL_POLYGON);
glVertex3fv(sa);
glVertex3fv(sb);
glVertex3fv(sc);
glVertex3fv(sd);
glEnd();

glColor3f(0.0f, 1.0f, 0.0f);
glBegin(GL_POLYGON);
glVertex3fv(sb);
glVertex3fv(sf);
glVertex3fv(sg);
glVertex3fv(sc);
glEnd();

glColor3f(0.0f, 0.0f, 1.0f);
glBegin(GL_POLYGON);
glVertex3fv(sf);
glVertex3fv(se);
glVertex3fv(sh);
glVertex3fv(sg);
glEnd();

glColor3f(1.0f, 1.0f, 0.0f);
glBegin(GL_POLYGON);
glVertex3fv(se);
glVertex3fv(sa);
glVertex3fv(sd);
glVertex3fv(sh);
glEnd();

glColor3f(0.0f, 1.0f, 1.0f);
glBegin(GL_POLYGON);
glVertex3fv(sd);
glVertex3fv(sc);
glVertex3fv(sg);
glVertex3fv(sh);
glEnd();

glColor3f(1.0f, 0.0f, 1.0f);
glBegin(GL_POLYGON);
glVertex3fv(sa);
glVertex3fv(sb);
glVertex3fv(sf);
glVertex3fv(se);
glEnd();
}
}*/
void kula(void)
{

	GLUquadric *kulka = gluNewQuadric();
	glColor3f(1.0f, 0, 0);
	glBegin(GL_QUAD_STRIP);

	gluSphere(kulka, 1.0, 50, 50);

	glEnd();

}


void kregiel(void)
{

	double angle;
	GLint circle_points = 100;
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	{
		// Parametry wierzcholkow podstawy

		GLfloat pa[3] = { 0.0f, 0.0f, 0.0f };
		GLfloat pb[3] = { 0.9f, 0.0f, 0.0f };
		GLfloat pc[3] = { 0.9f, 2.5f, 0.0f };
		GLfloat pd[3] = { 0.0f, 2.5f, 0.0f };
		GLfloat pe[3] = { 0.0f, 0.0f, -0.9f };
		GLfloat pf[3] = { 0.9f, 0.0f, -0.9f };
		GLfloat pg[3] = { 0.9f, 2.5f, -0.9f };
		GLfloat ph[3] = { 0.0f, 2.5f, -0.9f };

		GLfloat pi[3] = { -0.1864f, 0.0f, -0.45f };
		GLfloat pj[3] = { -0.14519f, 0.0f, -0.225f };
		GLfloat pk[3] = { -0.14519f, 0.0f, -0.675f };
		GLfloat pl[3] = { 1.0864f, 0.0f, -0.45f };
		GLfloat pm[3] = { 1.04519f, 0.0f, -0.225f };
		GLfloat pn[3] = { 1.04519f, 0.0f, -0.675f };
		GLfloat po[3] = { 0.45f, 0.0f, 0.1864f };
		GLfloat pp[3] = { 0.225f, 0.0f, 0.14519f };
		GLfloat pr[3] = { 0.675f, 0.0f, 0.14519f };
		GLfloat ps[3] = { 0.45f, 0.0f, -1.0864f };
		GLfloat pt[3] = { 0.225f, 0.0f, -1.04519f };
		GLfloat pu[3] = { 0.675f, 0.0f, -1.04519f };

		GLfloat pi1[3] = { -0.1864f, 2.5f, -0.45f };
		GLfloat pj1[3] = { -0.14519f, 2.5f, -0.225f };
		GLfloat pk1[3] = { -0.14519f, 2.5f, -0.675f };
		GLfloat pl1[3] = { 1.0864f, 2.5f, -0.45f };
		GLfloat pm1[3] = { 1.04519f, 2.5f, -0.225f };
		GLfloat pn1[3] = { 1.04519f, 2.5f, -0.675f };
		GLfloat po1[3] = { 0.45f, 2.5f, 0.1864f };
		GLfloat pp1[3] = { 0.225f, 2.5f, 0.14519f };
		GLfloat pr1[3] = { 0.675f, 2.5f, 0.14519f };
		GLfloat ps1[3] = { 0.45f, 2.5f, -1.0864f };
		GLfloat pt1[3] = { 0.225f, 2.5f, -1.04519f };
		GLfloat pu1[3] = { 0.675f, 2.5f, -1.04519f };

		// Parametry wierzcholkow czesci srodkowej

		GLfloat sa[3] = { 0.0f, 2.5f, -0.0f };
		GLfloat sb[3] = { 0.9f, 2.5f, -0.0f };
		GLfloat sc[3] = { 0.9f, 3.7f, -0.0f };
		GLfloat sd[3] = { 0.0f, 3.7f, -0.0f };
		GLfloat se[3] = { 0.0f, 2.5f, -0.9f };
		GLfloat sf[3] = { 0.9f, 2.5f, -0.9f };
		GLfloat sg[3] = { 0.9f, 3.7f, -0.9f };
		GLfloat sh[3] = { 0.0f, 3.7f, -0.9f };

		// Parametry wierzcholkow czesci gornej

		GLfloat ga[3] = { 0.0f, 3.7f, -0.0f };
		GLfloat gb[3] = { 0.9f, 3.7f, -0.0f };
		GLfloat gc[3] = { 0.6f, 4.5f, -0.3f };
		GLfloat gd[3] = { 0.3f, 4.5f, -0.3f };
		GLfloat ge[3] = { 0.0f, 3.7f, -0.9f };
		GLfloat gf[3] = { 0.9f, 3.7f, -0.9f };
		GLfloat gg[3] = { 0.6f, 4.5f, -0.6f };
		GLfloat gh[3] = { 0.3f, 4.5f, -0.6f };

		glColor3f(1.0f, 1.0f, 1.0f);//podstawa gorna
		glBegin(GL_POLYGON);
		glVertex3fv(pd);
		glVertex3fv(pj1);
		glVertex3fv(pi1);
		glVertex3fv(pk1);
		glVertex3fv(ph);
		glVertex3fv(pt1);
		glVertex3fv(ps1);
		glVertex3fv(pu1);
		glVertex3fv(pg);
		glVertex3fv(pn1);
		glVertex3fv(pl1);
		glVertex3fv(pm1);
		glVertex3fv(pc);
		glVertex3fv(pr1);
		glVertex3fv(po1);
		glVertex3fv(pp1);
		glEnd();

		glColor3f(1.0f, 0.0f, 1.0f);//podstawa dolna
		glBegin(GL_POLYGON);
		glVertex3fv(pa);
		glVertex3fv(pj);
		glVertex3fv(pi);
		glVertex3fv(pk);
		glVertex3fv(pe);
		glVertex3fv(pt);
		glVertex3fv(ps);
		glVertex3fv(pu);
		glVertex3fv(pf);
		glVertex3fv(pn);
		glVertex3fv(pl);
		glVertex3fv(pm);
		glVertex3fv(pb);
		glVertex3fv(pr);
		glVertex3fv(po);
		glVertex3fv(pp);
		glEnd();
		glColor3f(1.0f, 0.0f, 0.0f);//sciana przednia
		glBegin(GL_POLYGON);
		glVertex3fv(pa);
		glVertex3fv(pj);
		glVertex3fv(pj1);
		glVertex3fv(pd);
		glEnd();
		glColor3f(1.0f, 0.0f, 1.0f);//sciana przednia
		glBegin(GL_POLYGON);
		glVertex3fv(pj);
		glVertex3fv(pi);
		glVertex3fv(pi1);
		glVertex3fv(pj1);

		glEnd();
		glColor3f(1.0f, 1.0f, 0.0f);//sciana przednia
		glBegin(GL_POLYGON);
		glVertex3fv(pi);
		glVertex3fv(pk);
		glVertex3fv(pk1);
		glVertex3fv(pi1);

		glEnd();
		glColor3f(0.0f, 0.0f, 1.0f);//sciana przednia
		glBegin(GL_POLYGON);
		glVertex3fv(pk);
		glVertex3fv(pe);
		glVertex3fv(ph);
		glVertex3fv(pk1);

		glEnd();
		glColor3f(1.0f, 0.0f, 0.0f);//sciana przednia
		glBegin(GL_POLYGON);
		glVertex3fv(pe);
		glVertex3fv(pt);
		glVertex3fv(pt1);
		glVertex3fv(ph);

		glEnd();
		glColor3f(1.0f, 0.0f, 1.0f);//sciana przednia
		glBegin(GL_POLYGON);
		glVertex3fv(pt);
		glVertex3fv(ps);
		glVertex3fv(ps1);
		glVertex3fv(pt1);

		glEnd();
		glColor3f(0.0f, 1.0f, 0.0f);//sciana przednia
		glBegin(GL_POLYGON);
		glVertex3fv(ps);
		glVertex3fv(pu);
		glVertex3fv(pu1);
		glVertex3fv(ps1);

		glEnd();
		glColor3f(1.0f, 0.0f, 1.0f);//sciana przednia
		glBegin(GL_POLYGON);
		glVertex3fv(pu);
		glVertex3fv(pf);
		glVertex3fv(pg);
		glVertex3fv(pu1);

		glEnd();
		glColor3f(1.0f, 1.0f, 0.0f);//sciana przednia
		glBegin(GL_POLYGON);
		glVertex3fv(pf);
		glVertex3fv(pn);
		glVertex3fv(pn1);
		glVertex3fv(pg);

		glEnd();
		glColor3f(1.0f, 0.0f, 0.0f);//sciana przednia
		glBegin(GL_POLYGON);
		glVertex3fv(pn);
		glVertex3fv(pl);
		glVertex3fv(pl1);
		glVertex3fv(pn1);

		glEnd();
		glColor3f(1.0f, 1.0f, 0.0f);//sciana przednia
		glBegin(GL_POLYGON);
		glVertex3fv(pl);
		glVertex3fv(pm);
		glVertex3fv(pm1);
		glVertex3fv(pl1);

		glEnd();
		glColor3f(1.0f, 1.0f, 1.0f);//sciana przednia
		glBegin(GL_POLYGON);
		glVertex3fv(pm);
		glVertex3fv(pb);
		glVertex3fv(pc);
		glVertex3fv(pm1);

		glEnd();
		glColor3f(1.0f, 0.0f, 0.0f);//sciana przednia
		glBegin(GL_POLYGON);
		glVertex3fv(pb);
		glVertex3fv(pr);
		glVertex3fv(pr1);
		glVertex3fv(pc);

		glEnd();
		glColor3f(1.0f, 0.0f, 1.0f);//sciana przednia
		glBegin(GL_POLYGON);
		glVertex3fv(pr);
		glVertex3fv(po);
		glVertex3fv(po1);
		glVertex3fv(pr1);

		glEnd();
		glColor3f(1.0f, 1.0f, 0.0f);//sciana przednia
		glBegin(GL_POLYGON);
		glVertex3fv(po);
		glVertex3fv(pp);
		glVertex3fv(pp1);
		glVertex3fv(po1);

		glEnd();
		glColor3f(1.0f, 0.0f, 0.0f);//sciana przednia
		glBegin(GL_POLYGON);
		glVertex3fv(pp);
		glVertex3fv(pa);
		glVertex3fv(pd);
		glVertex3fv(pp1);

		glEnd();
		// Sciany skladowe czesci srodkowej
		glColor3f(1.0f, 1.0f, 0.0f);
		glBegin(GL_POLYGON); //sciana przednia
		glVertex3fv(sa);
		glVertex3fv(sb);
		glVertex3fv(sc);
		glVertex3fv(sd);
		glEnd();

		glColor3f(0.0f, 1.0f, 0.0f);//sciana prawa
		glBegin(GL_POLYGON);
		glVertex3fv(sb);
		glVertex3fv(sf);
		glVertex3fv(sg);
		glVertex3fv(sc);
		glEnd();

		glColor3f(1.0f, 1.0f, 0.0f);//sciana tylna
		glBegin(GL_POLYGON);
		glVertex3fv(sf);
		glVertex3fv(se);
		glVertex3fv(sh);
		glVertex3fv(sg);
		glEnd();

		glColor3f(1.0f, 0.0f, 0.0f);//sciana lewa
		glBegin(GL_POLYGON);
		glVertex3fv(se);
		glVertex3fv(sa);
		glVertex3fv(sd);
		glVertex3fv(sh);
		glEnd();

		glColor3f(0.0f, 0.0f, 1.0f);//podstawa gorna
		glBegin(GL_POLYGON);
		glVertex3fv(sd);
		glVertex3fv(sc);
		glVertex3fv(sg);
		glVertex3fv(sh);
		glEnd();

		glColor3f(1.0f, 1.0f, 1.0f);//podstawa dolna
		glBegin(GL_POLYGON);
		glVertex3fv(sa);
		glVertex3fv(sb);
		glVertex3fv(sf);
		glVertex3fv(se);
		glEnd();
		/*
		// sciany skladowe czesci gornej
		glColor3f(1.0f, 0.0f, 1.0f);//sciana przednia
		glBegin(GL_POLYGON);
		glVertex3fv(ga);
		glVertex3fv(gb);
		glVertex3fv(gc);
		glVertex3fv(gd);
		glEnd();

		glColor3f(1.0f, 0.0f, 0.0f);//sciana prawa
		glBegin(GL_POLYGON);
		glVertex3fv(gb);
		glVertex3fv(gf);
		glVertex3fv(gg);
		glVertex3fv(gc);
		glEnd();

		glColor3f(1.0f, 0.0f, 1.0f);//sciana tylna
		glBegin(GL_POLYGON);
		glVertex3fv(gf);
		glVertex3fv(ge);
		glVertex3fv(gh);
		glVertex3fv(gg);
		glEnd();

		glColor3f(0.0f, 0.0f, 1.0f);//sciana lewa
		glBegin(GL_POLYGON);
		glVertex3fv(ge);
		glVertex3fv(ga);
		glVertex3fv(gd);
		glVertex3fv(gh);
		glEnd();

		glColor3f(0.0f, 1.0f, 1.0f);//podstawa gorna
		glBegin(GL_POLYGON);
		glVertex3fv(gd);
		glVertex3fv(gc);
		glVertex3fv(gg);
		glVertex3fv(gh);
		glEnd();

		glColor3f(1.0f, 0.0f, 0.0f);//podstawa dolna
		glBegin(GL_POLYGON);
		glVertex3fv(ga);
		glVertex3fv(gb);
		glVertex3fv(gf);
		glVertex3fv(ge);
		glEnd();*/
	}
	/*
	glBegin (GL_LINE_LOOP);
	for(i=0;i<circle_points;i++)
	{
	angle = 2*3.1415*i/circle_points;
	glVertex3f(cos(angle)+0.45,sin(angle)+4.6,-0.45f);
	}
	glEnd();
	*/

	//kula na górze
	glPushMatrix();
	glTranslatef(0.5, 4.3, -0.45f);
	GLUquadric *kulka = gluNewQuadric();
	glColor3f(1.0f, 0, 0);
	glBegin(GL_QUAD_STRIP);
	gluSphere(kulka, 0.7, 50, 50);

	glEnd();
	glPopMatrix();
}
void siatka(void)
{
	int i;
	glColor3f(.3, .3, .3);
	glBegin(GL_QUADS);
	glVertex3f(-1, -0.001, 1);
	glVertex3f(-1, -0.001, -17);
	glVertex3f(7, -0.001, -17);
	glVertex3f(7, -0.001, 1);
	glEnd();

	glBegin(GL_LINES);
	for (i = 0; i <= 7; i++) {
		if (i == 0) {
			glColor3f(.8, .3, .3);
			glVertex3f(-1, 0, 1);
			glVertex3f(-1, 0, -17);
			i++;
		}
		if (i>0 && i<7){
			glColor3f(.25, .25, .25);
			glVertex3f(i, 0, 1);
			glVertex3f(i, 0, -17);
		}
		if (i == 7) {
			glColor3f(.8, .3, .3);
			glVertex3f(7, 0, 1);
			glVertex3f(7, 0, -17);
			i++;
		}
		i++;
	}
	for (i = 0; i <= 17; i++) {
		if (i == 0) {
			glColor3f(.8, .3, .3);
			glVertex3f(-1, 0, 1);
			glVertex3f(7, 0, 1);
			i++;
		}
		if (i>0 && i<17){
			glColor3f(.25, .25, .25);
			glVertex3f(-1, 0, -i);
			glVertex3f(7, 0, -i);
		}
		if (i == 17){
			glColor3f(.8, .3, .3);
			glVertex3f(-1, 0, -17);
			glVertex3f(7, 0, -17);
		}
		i++;
	};

	glEnd();
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
		hRC = SetUpOpenGL(hWnd);	// Inicjalizacja OpenGL
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
		case VK_ADD:
			move++;
			break;
		case VK_SUBTRACT:
			move--;
			break;
			// przesuniecia w poziomie
		case VK_NEXT:
			if ((pivot_x + change)<400)
				pivot_x += change;
			InvalidateRect(hWnd, NULL, FALSE);
			break;
		case VK_DELETE:
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
		case VK_PRIOR:
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


	GLfloat position[4] = { 10.0f, 10.0, 100.0f, 0.0f };

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

	//ladowanie tego pierdolonego obj ja pierdole

	std::vector< glm::vec3 > vertices;
	std::vector< glm::vec2 > uvs;
	std::vector< glm::vec3 > normals; // Won't be used at the moment.

	

	// transformacja obserwacji

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// umiejscowienie kamery w odleglym miejscu
	glTranslatef(pivot_x, pivot_y, -z_dist);
	glRotatef(pivot_vert_angle, 1, 0, 0);
	glRotatef(pivot_horiz_angle, 0, 1, 0);


	int test = loadObject("kregiel2.obj");
	int test2 = loadObject("kula2.obj");
	glPushMatrix();
	glCallList(test);
	glPopMatrix();

	glPushMatrix();
	glCallList(test2);
	glTranslatef(2.0, 1.0, move);
	glPopMatrix();

	glPushMatrix();

	//szescian !!!!!!!!!!!!!!!!!!!!!!!!!!
	//szescian();
	
	/*
	glPushMatrix();
		glTranslatef(2.0, 1.0, move);// sterowanie kula + -
		kula();
	glPopMatrix();
	
	kregiel();
	
	//szescian3();
	//szescian4();
	*/
	siatka();
	glPopMatrix();
	
	glFlush();

}
