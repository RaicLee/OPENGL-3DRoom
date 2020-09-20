/*

CODED BY RAIX
2020/05/17

*/


#include <iostream>
#include "freeglut.h"
#include <cmath>
#include <vector>
#include "Camera.hpp"
#include <time.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
using namespace std;
#ifndef TEXTURE
#define TEXTURE
#define FLOOR 0
#define WALL 1
#define BOOK 2
#define DESK 3
#define AIRCON 4
#define CURTAIN 5
#define DESKFRONT 6
#define POSTER 7
#define DOOR 8
#define CLOTHBOXFRONT 9
#define PROCEDURAL 10
#define WINDOW 11
#define CHAIR 12
#define CHAIRHEAD 13
#define BED 14
#define PLASTIC 15
#define POSTER1 16
#define POSTER2 17
#define BOOKCOVER 18
#define DUCK 19
#endif
/*
typedef struct Point {
	GLfloat a[3], b[3], c[3];
	GLfloat *normal;
}P;
*/
#ifndef ADDITIONALPOINT
#define ADDITIONALPOINT
#define    ImageWidth 524
#define    ImageHeight 524
GLubyte MatrixImage[ImageHeight][ImageWidth][3];
float RGB[3];
//static GLint Imageheight;

float InterPolation(float a, float b, float c)
{
	return a + (b - a)*c*c*(3 - 2 * c);

}
float InterLinear(float a, float b, float c)
{
	return a * (1 - c) + b * c;
}
float Noise(int x)
{
	x = (x << 13) ^ x;
	return (((x * (x * x * 15731 + 789221) + 1376312589) & 0x7fffffff) / 2147483648.0);
}
float PerlinNoise(float x, float y, int width, int octaves, int seed, double periode) {

	double a, b, value, freq, tam_pas, zone_x, zone_y;
	int s, box, num, step_x, step_y;
	int amplitude = 120;
	int noisedata;

	freq = 1 / (float)(periode);

	for (s = 0; s < octaves; s++) {
		num = (int)(width*freq);
		step_x = (int)(x*freq);
		step_y = (int)(y*freq);
		zone_x = x * freq - step_x;
		zone_y = y * freq - step_y;
		box = step_x + step_y * num;
		noisedata = (box + seed);
		a = InterPolation(Noise(noisedata), Noise(noisedata + 1), zone_x);
		b = InterPolation(Noise(noisedata + num), Noise(noisedata + 1 + num), zone_x);
		value = InterPolation(a, b, zone_y)*amplitude;

	}
	return value;
}
void colour(float valor) {
	int r = InterLinear(0, valor, valor);
	int g = InterLinear(0, valor, valor);
	int b = InterLinear(0, valor, valor);

	RGB[0] = r;
	RGB[1] = g;
	RGB[2] = b;
}
void makeImage(void)
{

	int x, y;
	float color;
	int seed;
	int width;
	float  disp1, disp2, disp3, disp4, disp5, disp6, scale;
	srand(time(NULL));

	seed = rand();
	for (y = 0; y < ImageWidth; y++) {
		for (x = 0; x < ImageHeight; x++) {
			scale = 1;
			width = 124130;

			disp1 = PerlinNoise(x*scale, y*scale, width, 1, seed, 100);
			disp2 = PerlinNoise(x*scale, y*scale, width, 1, seed, 25);
			disp3 = PerlinNoise(x*scale, y*scale, width, 1, seed, 12.5);
			disp4 = PerlinNoise(x*scale, y*scale, width, 1, seed, 6.25);
			disp5 = PerlinNoise(x*scale, y*scale, width, 1, seed, 3.125);
			disp6 = PerlinNoise(x*scale, y*scale, width, 1, seed, 1.56);

			colour((int)disp1 + (int)(disp2*.25) + (int)(disp3*.125) + (int)(disp4*.0625) + (int)(disp5*.03125) + (int)(disp6*.0156));

			MatrixImage[x][y][0] = RGB[0];
			MatrixImage[x][y][1] = RGB[1];
			MatrixImage[x][y][2] = RGB[2];
		}
	}



}
#endif

typedef struct QUAD {
	GLfloat a[3], b[3], c[3], d[3];
	GLfloat *normal;
	GLfloat text;
}P;

Camera *cam;
//색깔 조절용
float a = 0.6;
float b = 1.6;

//텍스쳐용
unsigned int texture[20];
int width, height, nrChannels;
void normalize(GLfloat *f) {
	GLfloat length = sqrt(f[0] * f[0] + f[1] * f[1] + f[2] * f[2]);

	for (int i = 0; i < 3; i++) {
		f[i] = f[i] / length;
	}
}
GLfloat *crossproduct(GLfloat *a, GLfloat *b) {
	GLfloat result[] = { a[1] * b[2] - a[2] * b[1],
		-(a[0] * b[2] - a[2] * b[0]),
		a[0] * b[1] - a[1] * b[0] };
	normalize(result);
	return result;
}
GLfloat* cNorm(GLfloat *a, GLfloat *b, GLfloat *c) {
	GLfloat x[] = { b[0] - a[0],b[1] - a[1],b[2] - a[2] };
	GLfloat y[] = { c[0] - a[0],c[1] - a[1],c[2] - a[2] };

	GLfloat* result = crossproduct(x, y);
	return result;
}

void cubeinit(float Garo, float Sero, float Nopi, vector<P> &arr) {
	if (arr.size() != 0) return;
	Garo /= 2.0;
	Sero /= 2.0;
	//밑면
	//시계 방향으로 해야지 밖에서 볼때 반시계 방향
	P v;
	v.a[0] = Garo; v.a[1] = 0.1f; v.a[2] = Sero;
	v.b[0] = -Garo; v.b[1] = 0.1f; v.b[2] = Sero;
	v.c[0] = -Garo; v.c[1] = 0.1f; v.c[2] = -Sero;
	v.d[0] = Garo; v.d[1] = 0.1f; v.d[2] = -Sero;
	v.normal = cNorm(v.a, v.b, v.c);
	arr.push_back(v);

	//윗면
	v.a[0] = Garo; v.a[1] = Nopi; v.a[2] = Sero;
	v.b[0] = Garo; v.b[1] = Nopi; v.b[2] = -Sero;
	v.c[0] = -Garo; v.c[1] = Nopi; v.c[2] = -Sero;
	v.d[0] = -Garo; v.d[1] = Nopi; v.d[2] = Sero;
	v.normal = cNorm(v.a, v.b, v.c);
	arr.push_back(v);

	//보는데서 앞면
	/*
	v.a[0] = Garo; v.a[1] = Nopi; v.a[2] = Sero;
	v.b[0] = -Garo; v.b[1] = Nopi; v.b[2] = Sero;
	v.c[0] = -Garo; v.c[1] = 0.1f; v.c[2] = Sero;
	v.d[0] = Garo; v.d[1] = 0.1f; v.d[2] = Sero;
	v.normal = cNorm(v.a, v.b, v.c);
	arr.push_back(v);
	*/
	//앞면다시
	v.a[0] = -Garo; v.a[1] = Nopi; v.a[2] = Sero;
	v.b[0] = -Garo; v.b[1] = 0.1f; v.b[2] = Sero;
	v.c[0] = Garo; v.c[1] = 0.1f; v.c[2] = Sero;
	v.d[0] = Garo; v.d[1] = Nopi; v.d[2] = Sero;
	v.normal = cNorm(v.a, v.b, v.c);
	arr.push_back(v);

	v.a[0] = Garo; v.a[1] = Nopi; v.a[2] = -Sero;
	v.b[0] = Garo; v.b[1] = 0.1f; v.b[2] = -Sero;
	v.c[0] = -Garo; v.c[1] = 0.1f; v.c[2] = -Sero;
	v.d[0] = -Garo; v.d[1] = Nopi; v.d[2] = -Sero;
	v.normal = cNorm(v.a, v.b, v.c);
	arr.push_back(v);

	//옆면
	v.a[0] = Garo; v.a[1] = Nopi; v.a[2] = Sero;
	v.b[0] = Garo; v.b[1] = 0.1f; v.b[2] = Sero;
	v.c[0] = Garo; v.c[1] = 0.1f; v.c[2] = -Sero;
	v.d[0] = Garo; v.d[1] = Nopi; v.d[2] = -Sero;
	v.normal = cNorm(v.a, v.b, v.c);
	arr.push_back(v);

	v.a[0] = -Garo; v.a[1] = Nopi; v.a[2] = -Sero;
	v.b[0] = -Garo; v.b[1] = 0.1f; v.b[2] = -Sero;
	v.c[0] = -Garo; v.c[1] = 0.1f; v.c[2] = Sero;
	v.d[0] = -Garo; v.d[1] = Nopi; v.d[2] = Sero;
	v.normal = cNorm(v.a, v.b, v.c);
	arr.push_back(v);

}
inline void print(vector<P> whattodraw) {
	for (int i = 0; i < whattodraw.size(); i++) {
		glBegin(GL_QUADS);
		glNormal3fv(whattodraw[i].normal);
		glVertex3fv(whattodraw[i].a);
		glVertex3fv(whattodraw[i].b);
		glVertex3fv(whattodraw[i].c);
		glVertex3fv(whattodraw[i].d);
		glEnd();
	}
}
inline void printQ(vector<P> whattodraw,int num,GLfloat point) {
	if(num!=-1)
		glBindTexture(GL_TEXTURE_2D, texture[num]);
	for (int i = 0; i < whattodraw.size(); i++) {
		glBegin(GL_QUADS);

		glNormal3fv(whattodraw[i].normal);
		glTexCoord2f(0.0, point);
		glVertex3fv(whattodraw[i].a);
		glTexCoord2f(0.0, 0.0);
		glVertex3fv(whattodraw[i].b);
		glTexCoord2f(point, 0.0);
		glVertex3fv(whattodraw[i].c);
		glTexCoord2f(point, point);
		glVertex3fv(whattodraw[i].d);
		glEnd();
	}
	glBindTexture(GL_TEXTURE_2D, 0);
}
inline void printQatFace(vector<P> whattodraw, int num, GLfloat point,int face) {
	for (int i = 0; i < whattodraw.size(); i++) {
		if (i == face) {
			glBindTexture(GL_TEXTURE_2D, texture[num]);
		}
		glBegin(GL_QUADS);

		glNormal3fv(whattodraw[i].normal);
		glTexCoord2f(0.0, point);
		glVertex3fv(whattodraw[i].a);
		glTexCoord2f(0.0, 0.0);
		glVertex3fv(whattodraw[i].b);
		glTexCoord2f(point, 0.0);
		glVertex3fv(whattodraw[i].c);
		glTexCoord2f(point, point);
		glVertex3fv(whattodraw[i].d);
		glEnd();
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	glBindTexture(GL_TEXTURE_2D, 0);
}

inline void orangeColor() {
	GLfloat mat_ambient[] = { 1.000*a, 0.271*a, 0.000, 1.0 };
	GLfloat mat_diffuse[] = { 1.000, 0.271, 0.000, 1.0 };
	GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat mat_shininess = 1.0;
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialf(GL_FRONT, GL_SHININESS, mat_shininess);
}
inline void GreenColor() {

	GLfloat mat_ambient[] = { 0.1, 0.3, 0.2, 1.0 };
	GLfloat mat_diffuse[] = { 0.4, 0.8, 0.6, 1.0 };
	GLfloat mat_specular[] = { 0.1, 0.1, 0.1, 1.0 };
	GLfloat mat_shininess = 10.0;
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialf(GL_FRONT, GL_SHININESS, mat_shininess);
}
inline void woodColor() {
	GLfloat mat_ambient[] = { 1.000*a, 0.894*a, 0.710*a,1.0 };
	GLfloat mat_diffuse[] = { 1.000, 0.894, 0.710, 1.0 };
	GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat mat_shininess[] = { 15.0 };
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
}
inline void whiteColor() {
	GLfloat mat_ambient[] = { 1.000*a, 0.980*a, 0.980*a,1.0 };
	GLfloat mat_diffuse[] = { 1.000, 0.980, 0.980,1.0 };
	GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat mat_shininess[] = { 20.0 };
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
}
inline void deskColor() {
	GLfloat matdesk_ambient[] = { 0.871*a, 0.722*a, 0.529*a,1.0 };
	GLfloat matdesk_diffuse[] = { 0.871, 0.722, 0.529,1.0 };
	GLfloat matdesk_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat matdesk_shininess[] = { 25.0 };
	glMaterialfv(GL_FRONT, GL_AMBIENT, matdesk_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, matdesk_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, matdesk_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, matdesk_shininess);
}
inline void outRoomColor() {
	GLfloat matroom_ambient[] = { 1.0*a, 1.0*a, 1.000*a,1.0 };
	GLfloat matroom_diffuse[] = { 2.0, 2.0, 2.0,1.0 };
	GLfloat matroom_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat matroom_shininess[] = { 30.0 };
	glMaterialfv(GL_FRONT, GL_AMBIENT, matroom_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, matroom_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, matroom_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, matroom_shininess);
}
inline void curtainColor() {
	GLfloat mat_ambient[] = { 1.000*a, 0.894*a, 0.710*a,1.0 };
	GLfloat mat_diffuse[] = { 1.000, 0.894, 0.710,1.0 };
	GLfloat mat_specular[] = { 10.0, 10.0, 10.0, 1.0 };
	GLfloat mat_shininess[] = { 1000.0 };
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
}
inline void bookshelfColor() {
	GLfloat matbookshelf_ambient[] = { 0.871*a, 0.722*a, 0.529*a,1.0 };
	GLfloat matbookshelf_diffuse[] = { 0.871, 0.722, 0.529,1.0 };
	GLfloat matbookshelf_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat matbookshelf_shininess[] = { 10.0 };
	glMaterialfv(GL_FRONT, GL_AMBIENT, matbookshelf_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, matbookshelf_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, matbookshelf_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, matbookshelf_shininess);
}
inline void clothboxColor() {
	GLfloat mat_ambient[] = { 0.941, 1.000, 1.000,1.0 };
	GLfloat mat_diffuse[] = { 0.941, 1.000, 1.000,1.0 };
	GLfloat mat_specular[] = { 10.0, 10.0, 10.0, 1.0 };
	GLfloat mat_shininess[] = { 1000.0 };
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
}
inline void bedUpperColor() {
	GLfloat matremain_ambient[] = { 0.992*a, 0.961*a, 0.902*a,1.0 };
	GLfloat mat_diffuse[] = { 0.992, 0.961, 0.902,1.0 };
	GLfloat matremain_specular[] = { 0.8, 0.7, 0.6, 1.0 };
	GLfloat matremain_shininess[] = { 50.0 };
	glMaterialfv(GL_FRONT, GL_AMBIENT, matremain_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, matremain_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, matremain_shininess);
}
inline void matColor() {
	GLfloat mat_ambient[] = { 1.000*a, 0.753*a, 0.796*a,1.0 };
	GLfloat mat_diffuse[] = { 1.000, 0.753, 0.796,1.0 };
	GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat mat_shininess[] = { 10.0 };
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
}
inline void bedUpperColor2() {
	GLfloat matremain_ambient[] = { 1.000*a, 0.894*a, 0.710*a,1.0 };
	GLfloat mat_diffuse[] = { 1.000, 0.894, 0.710,1.0 };
	GLfloat matremain_specular[] = { 0.8, 0.7, 0.6, 1.0 };
	GLfloat matremain_shininess[] = { 50.0 };
	glMaterialfv(GL_FRONT, GL_AMBIENT, matremain_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, matremain_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, matremain_shininess);
}
inline void windowColor() {
	GLfloat matremain_ambient[] = { 0.941*a, 0.973*a, 1.000*a,1.0 };
	GLfloat mat_diffuse[] = { 0.941, 0.973, 1.000,1.0 };
	GLfloat matremain_specular[] = { 0.8, 0.7, 0.6, 1.0 };
	GLfloat matremain_shininess[] = { 10.0 };
	glMaterialfv(GL_FRONT, GL_AMBIENT, matremain_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, matremain_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, matremain_shininess);
}
inline void darkColor() {
	GLfloat matremain_ambient[] = { 0.1, 0.1, 0.1,1.0 };
	GLfloat mat_diffuse[] = { 0.1, 0.1, 0.1,1.0 };
	GLfloat matremain_specular[] = { 0.8, 0.7, 0.6, 1.0 };
	GLfloat matremain_shininess[] = { 10.0 };
	glMaterialfv(GL_FRONT, GL_AMBIENT, matremain_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, matremain_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, matremain_shininess);
}
inline void brownColor() {
	GLfloat matremain_ambient[] = { 0.502*a, 0.000, 0.000,1.0 };
	GLfloat mat_diffuse[] = { 0.502, 0.000, 0.000,1.0 };
	GLfloat matremain_specular[] = { 0.8, 0.7, 0.6, 1.0 };
	GLfloat matremain_shininess[] = { 10.0 };
	glMaterialfv(GL_FRONT, GL_AMBIENT, matremain_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, matremain_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, matremain_shininess);
}
vector<P> bookdown;
vector<P> bookmain;
vector<P> bookside;
bool isBooksetted;
inline void drawBooks(GLfloat x, GLfloat y, GLfloat z) {
	if (!isBooksetted) {
		isBooksetted = true;
		cubeinit(4, 5, 0.2, bookdown);
		cubeinit(0.1, 5, 1, bookside);
		cubeinit(3.5, 4.5, 0.7, bookmain);
	}
	glPushMatrix();
	//입력받은 좌표로 옮겨주는 역할을 한다
	
	glTranslatef(x, y, z);
	glRotatef(90, 0, 1, 0);
	glTranslatef(0, 3.0, 0);
	glRotatef(90, 1, 0, 0);
	glPushMatrix();
	//woodColor();
	printQ(bookdown,BOOKCOVER,1.0);
	glPushMatrix();
	glTranslatef(0, 0.9, 0);
	printQ(bookdown, BOOKCOVER, 1.0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-2, 0, 0);
	printQ(bookside, BOOKCOVER, 1.0);
	glPopMatrix();

	//whiteColor();
	bedUpperColor2();
	glPushMatrix();
	glTranslatef(0, 0.2, 0.0);
	printQ(bookmain,PLASTIC,1.0);
	glPopMatrix();

	glPopMatrix();
	glPopMatrix();
}



vector<P> rooms;
vector<P> quadRooms;
bool isRoomsetted = false;
/*`
void initroom() {
	isRoomsetted = true;
	P v;

	v.a[0] = 52.0f; v.a[1] = 0.0f;  v.a[2] = 48.0f;
	v.b[0] = 52.0f; v.b[1] = 0.0f; v.b[2] = -48.0f;
	v.c[0] = -52.0f; v.c[1] = 0.0f; v.c[2] = -48.0f;
	v.normal = cNorm(v.a, v.b, v.c);
	rooms.push_back(v);
	v.a[0] = -52.0f; v.a[1] = 0.0f; v.a[2] = -48.0f;
	v.b[0] = -52.0f; v.b[1] = 0.0f; v.b[2] = 48.0f;
	v.c[0] = 52.0f; v.c[1] = 0.0f; v.c[2] = 48.0f;
	v.normal = cNorm(v.a, v.b, v.c);
	rooms.push_back(v);
	//bottom


	v.a[0] = 52.0f; v.a[1] = 46.0f; v.a[2] = 48.0f;
	v.b[0] = -52.0f; v.b[1] = 46.0f; v.b[2] = 48.0f;
	v.c[0] = -52.0f; v.c[1] = 46.0f; v.c[2] = -48.0f;
	v.normal = cNorm(v.a, v.b, v.c);
	rooms.push_back(v);
	v.a[0] = -52.0f; v.a[1] = 46.0f; v.a[2] = -48.0f;
	v.b[0] = 52.0f; v.b[1] = 46.0f; v.b[2] = -48.0f;
	v.c[0] = 52.0f; v.c[1] = 46.0f; v.c[2] = 48.0f;
	v.normal = cNorm(v.a, v.b, v.c);
	rooms.push_back(v);
	//top

	//side 1
	v.a[0] = 52.0f; v.a[1] = 0.0f; v.a[2] = 48.0f;
	v.b[0] = 52.0f; v.b[1] = 46.0f; v.b[2] = 48.0f;
	v.c[0] = 52.0f; v.c[1] = 46.0f; v.c[2] = -48.0f;
	v.normal = cNorm(v.a, v.b, v.c);
	rooms.push_back(v);

	v.a[0] = 52.0f; v.a[1] = 46.0f; v.a[2] = -48.0f;
	v.b[0] = 52.0f; v.b[1] = 0.0f; v.b[2] = -48.0f;
	v.c[0] = 52.0f; v.c[1] = 0.0f; v.c[2] = 48.0f;
	v.normal = cNorm(v.a, v.b, v.c);
	rooms.push_back(v);

	//side 2
	v.a[0] = 52.0f; v.a[1] = 46.0f; v.a[2] = 48.0f;
	v.b[0] = 52.0f; v.b[1] = 0.0f; v.b[2] = 48.0f;
	v.c[0] = -52.0f; v.c[1] = 0.0f; v.c[2] = 48.0f;
	v.normal = cNorm(v.a, v.b, v.c);
	rooms.push_back(v);

	v.a[0] = -52.0f; v.a[1] = 0.0f; v.a[2] = 48.0f;
	v.b[0] = -52.0f; v.b[1] = 46.0f; v.b[2] = 48.0f;
	v.c[0] = 52.0f; v.c[1] = 46.0f; v.c[2] = 48.0f;
	v.normal = cNorm(v.a, v.b, v.c);
	rooms.push_back(v);

	//side 3
	v.a[0] = -52.0f; v.a[1] = 0.0f; v.a[2] = 48.0f;
	v.b[0] = -52.0f; v.b[1] = 0.0f; v.b[2] = -48.0f;
	v.c[0] = -52.0f; v.c[1] = 46.0f; v.c[2] = -48.0f;
	v.normal = cNorm(v.a, v.b, v.c);
	rooms.push_back(v);

	v.a[0] = -52.0f; v.a[1] = 46.0f; v.a[2] = 48.0f;
	v.b[0] = -52.0f; v.b[1] = 0.0f; v.b[2] = 48.0f;
	v.c[0] = -52.0f; v.c[1] = 46.0f; v.c[2] = -48.0f;
	v.normal = cNorm(v.a, v.b, v.c);
	rooms.push_back(v);

	//side 4
	v.a[0] = -52.0f; v.a[1] = 46.0f; v.a[2] = -48.0f;
	v.b[0] = -52.0f; v.b[1] = 0.0f; v.b[2] = -48.0f;
	v.c[0] = 52.0f; v.c[1] = 0.0f; v.c[2] = -48.0f;
	v.normal = cNorm(v.a, v.b, v.c);
	rooms.push_back(v);

	v.a[0] = 52.0f; v.a[1] = 0.0f; v.a[2] = -48.0f;
	v.b[0] = 52.0f; v.b[1] = 46.0f; v.b[2] = -48.0f;
	v.c[0] = -52.0f; v.c[1] = 46.0f; v.c[2] = -48.0f;
	v.normal = cNorm(v.a, v.b, v.c);
	rooms.push_back(v);

}
*/
void initroom() {
	isRoomsetted = true;
	P v;
	//밑면

	v.a[0] = 52.0f; v.a[1] = 0.0f; v.a[2] = 48.0f;
	v.b[0] = 52.0f; v.b[1] = 0.0f; v.b[2] = -48.0f;
	v.c[0] = -52.0f; v.c[1] = 0.0f; v.c[2] = -48.0f;
	v.d[0] = -52.0f; v.d[1] = 0.0f; v.d[2] = 48.0f;
	v.normal = cNorm(v.a, v.b, v.c);
	v.text = 0;
	quadRooms.push_back(v);

	//윗면
	v.a[0] = 52.0f; v.a[1] = 46.0f; v.a[2] = 48.0f;
	v.b[0] = -52.0f; v.b[1] = 46.0f; v.b[2] = 48.0f;
	v.c[0] = -52.0f; v.c[1] = 46.0f; v.c[2] = -48.0f;
	v.d[0] = 52.0f; v.d[1] = 46.0f; v.d[2] = -48.0f;
	v.normal = cNorm(v.a, v.b, v.c);
	quadRooms.push_back(v);

	//볼때 왼쪽
	v.a[0] = 52.0f; v.a[1] = 46.0f; v.a[2] = -48.0f;
	v.b[0] = 52.0f; v.b[1] = 0.0f; v.b[2] = -48.0f;
	v.c[0] = 52.0f; v.c[1] = 0.0f; v.c[2] = 48.0f;
	v.d[0] = 52.0f; v.d[1] = 46.0f; v.d[2] = 48.0f;
	v.normal = cNorm(v.a, v.b, v.c);
	quadRooms.push_back(v);

	//볼때 오른쪽
	v.a[0] = -52.0f; v.a[1] = 46.0f; v.a[2] = 48.0f;
	v.b[0] = -52.0f; v.b[1] = 0.0f; v.b[2] = 48.0f;
	v.c[0] = -52.0f; v.c[1] = 0.0f; v.c[2] = -48.0f;
	v.d[0] = -52.0f; v.d[1] = 46.0f; v.d[2] = -48.0f;
	v.normal = cNorm(v.a, v.b, v.c);
	quadRooms.push_back(v);

	//볼때 앞
	v.a[0] = 52.0f; v.a[1] = 46.0f; v.a[2] = 48.0f;
	v.b[0] = 52.0f; v.b[1] = 0.0f; v.b[2] = 48.0f;
	v.c[0] = -52.0f; v.c[1] = 0.0f; v.c[2] = 48.0f;
	v.d[0] = -52.0f; v.d[1] = 46.0f; v.d[2] = 48.0f;
	v.normal = cNorm(v.a, v.b, v.c);
	quadRooms.push_back(v);
	//볼때 뒤
	v.a[0] = 52.0f; v.a[1] = 0.0f; v.a[2] = -48.0f;
	v.b[0] = 52.0f; v.b[1] = 46.0f; v.b[2] = -48.0f;
	v.c[0] = -52.0f; v.c[1] = 46.0f; v.c[2] = -48.0f;
	v.d[0] = -52.0f; v.d[1] = 0.0f; v.d[2] = -48.0f;
	v.normal = cNorm(v.a, v.b, v.c);
	quadRooms.push_back(v);

}
void drawOutRoom() {
	if (!isRoomsetted) {
		initroom();
	}
		
	glPushMatrix();
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glDisable(GL_COLOR_MATERIAL);

	outRoomColor();
	
	for (int i = 0; i < quadRooms.size(); i++) {
		if (i < 1) {
			glBindTexture(GL_TEXTURE_2D, texture[FLOOR]);
			glBegin(GL_QUADS);
			
			glNormal3fv(quadRooms[i].normal);
			glTexCoord2f(0.0, 5.0);
			glVertex3fv(quadRooms[i].a);
			glTexCoord2f(0.0, 0.0);
			glVertex3fv(quadRooms[i].b);
			glTexCoord2f(5.0, 0.0);
			glVertex3fv(quadRooms[i].c);
			glTexCoord2f(5.0, 5.0);
			glVertex3fv(quadRooms[i].d);
			glEnd();
			glBindTexture(GL_TEXTURE_2D, 0);

		}
		else {
			glBindTexture(GL_TEXTURE_2D, texture[WALL]);
			glBegin(GL_QUADS);

			glNormal3fv(quadRooms[i].normal);
			glTexCoord2f(0.0, 1.0);
			glVertex3fv(quadRooms[i].a);
			glTexCoord2f(0.0, 0.0);
			glVertex3fv(quadRooms[i].b);
			glTexCoord2f(1.0, 0.0);
			glVertex3fv(quadRooms[i].c);
			glTexCoord2f(1.0, 1.0);
			glVertex3fv(quadRooms[i].d);
			glEnd();
			glBindTexture(GL_TEXTURE_2D, 0);
		}
	}
	glPopMatrix();
	//glDisable(GL_CULL_FACE);
}

vector<P> deskSIDE;
void drawSIDES() {
	glPushMatrix();
	cubeinit(1, 20, 31, deskSIDE);
	printQ(deskSIDE,3,1.0);
	glPopMatrix();
}
vector<P> deskmain;
vector<P> deskUpper;
vector<P> deskBack;
vector<P> deskDoor;
vector<P> deskLight;
vector<P> deskRealLight;
void drawDesk() {

	glPushMatrix();//draw desk start
				   //glTranslatef(30.0f,15.0f,-38.0f);
				   //glTranslatef(10.0f, 3.0f, -10.0f);

	deskColor();
	//1 중앙 메인 부분을 만든다
	glTranslatef(30.0f, 15.0f, -37.9f);

	glPushMatrix();
	glTranslatef(-15.5f, -14.8f, -0.1);
	drawSIDES();
	glTranslatef(31.0f, 0, 0);
	drawSIDES();
	glPopMatrix();
	//glTranslatef(0.0f, 15.0f, 0.0f);
	glPushMatrix();
	//공부하는 책상 위
	cubeinit(30, 20, 1, deskmain);
	printQ(deskmain,DESK,1.0);
	glPopMatrix();

	//공부 책상 바로위
	glPushMatrix();
	//orangeColor();

	glTranslatef(0.0f, 7.0f, -5.0f);
	cubeinit(30, 10, 1, deskUpper);
	printQ(deskUpper,DESK, 1.0);
	glPopMatrix();

	//책상 빛나오는곳
	glPushMatrix();
	//control Panel
	glTranslatef(0.0f, 6.0f, -0.5f);
	cubeinit(30, 1, 2, deskLight);
	printQ(deskLight,DESK,1.0);
	//Light 나오는곳
	cubeinit(30, 2, 3, deskRealLight);
	whiteColor();
	glTranslatef(0.0f, -2.0f, -3.0f);
	print(deskRealLight);
	glPopMatrix();
	deskColor();

	//공부책상위의위
	deskColor();
	glPushMatrix();
	//orangeColor();
	glTranslatef(0.0f, 15.0f, -5.0f);
	cubeinit(30, 10, 1, deskUpper);
	printQ(deskUpper, DESK, 1.0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(13, 15.0f, -5.0f);
	
	for (int i = 0; i < 19; i++) {
		if (i % 4 == 0)darkColor();
		else if (i % 4 == 1)brownColor();
		else if (i % 4 == 2)whiteColor();
		else matColor();
		glPushMatrix();
		drawBooks(-1.5 * i, 0, 0);
		glPopMatrix();
	}
	
	glPopMatrix();

	deskColor();
	//공부책상 바로 아래 발 놓는곳
	glPushMatrix();
	//orangeColor();
	glTranslatef(0.0f, -10.0f, -5.0f);
	cubeinit(30, 10, 1, deskUpper);
	printQ(deskUpper, DESK, 1.0);
	glPopMatrix();
	//책상 메인 뒷면
	glPushMatrix();
	glTranslatef(0.0, -15.0, -10.1);
	cubeinit(30, 0.1, 35.0, deskBack);
	printQ(deskBack, DESK, 1.0);
	glPopMatrix();






	//front right
	glPushMatrix();
	GreenColor();
	glTranslatef(7.55, 7.9, -0.5);
	glPushMatrix();
	cubeinit(15, 1, 7.2, deskDoor);
	printQ(deskDoor,DESKFRONT,1.0);
	glTranslatef(-5, 3.5, 0);
	orangeColor();
	glutSolidCylinder(0.25, 1, 20, 10);
	glPushMatrix();
	glTranslatef(0, 0, 1);
	glutSolidCylinder(0.5, 1, 20, 10);
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();

	//front left
	glPushMatrix();
	GreenColor();
	glTranslatef(-7.55, 7.9, -0.5);
	glPushMatrix();
	printQ(deskDoor, DESKFRONT, 1.0);
	glTranslatef(5, 3.5, 0);
	orangeColor();
	glutSolidCylinder(0.25, 1, 20, 10);
	glPushMatrix();
	glTranslatef(0, 0, 1);
	glutSolidCylinder(0.5, 1, 20, 10);
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();




	glPopMatrix();//drawDesk 전체 


}


vector<P> bookshelfback;
vector<P> bookshelfright;
vector<P> bookshelfleft;
vector<P> bookshelfdown;
vector<P> bookshelfmiddle;
void drawBookshelf() {
	bookshelfColor();
	glPushMatrix();
	glTranslatef(-5.3f, 0.0f, -43.1f);

	//glTranslatef(0.0f, -0.1f, 0.0f);//전체 해당 위치로 옮기기

	//1 뒷면 만들기
	glPushMatrix();
	cubeinit(30, 1, 36.5, bookshelfback);
	glTranslatef(0.0f, 1.8f, -4.6f);
	printQ(bookshelfback,BOOK, 1.0);
	glPopMatrix();
	glPushMatrix();
	cubeinit(1, 10, 40, bookshelfright);
	glTranslatef(-15.49f, 0.0f, 0.0f);
	printQ(bookshelfright, BOOK, 1.0);
	glPopMatrix();
	glPushMatrix();
	cubeinit(1, 10, 40, bookshelfleft);
	glTranslatef(15.49f, 0.0f, 0.0f);
	printQ(bookshelfleft, BOOK, 1.0);
	glPopMatrix();

	glPushMatrix();
	cubeinit(30, 10, 2, bookshelfdown);
	printQ(bookshelfdown, BOOK, 1.0);
	glPopMatrix();
	glPushMatrix();
	cubeinit(30, 10, 2, bookshelfdown);
	glTranslatef(0.0f, 38.0f, 0.0f);
	printQ(bookshelfdown, BOOK, 1.0);
	glPopMatrix();

	glPushMatrix();
	cubeinit(30, 10, 1, bookshelfmiddle);
	//glRotatef(90, 1, 0, 0);
	//각층
	for (int k = 0; k < 4; k++) {
		glPushMatrix();
		if (k == 0)
			glTranslatef(0.0f, 9.0f, 0.0f);
		else
			glTranslatef(0.0f, 7.0*(k + 1) + 3, 0.0f);
		printQ(bookshelfmiddle,BOOK,1.0);
		glPopMatrix();
	}
	glPopMatrix();
	//books

	glPushMatrix();
	glTranslatef(13.5f, 1, 0);
	for (int i = 0; i < 19; i++) {
		if (i % 4 == 0)darkColor();
		else if (i % 4 == 1)brownColor();
		else if (i % 4 == 2)whiteColor();
		else matColor();
		glPushMatrix();
		drawBooks(-1.5 * i, 0, 0);
		glPopMatrix();
		if (i % 4 == 0)darkColor();
		else if (i % 4 == 1)brownColor();
		else if (i % 4 == 2)whiteColor();
		else matColor();
		glPushMatrix();
		drawBooks(-1.5 * i, 8, 0);
		glPopMatrix();
		if (i % 4 == 0)darkColor();
		else if (i % 4 == 1)brownColor();
		else if (i % 4 == 2)whiteColor();
		else matColor();
		glPushMatrix();
		drawBooks(-1.5 * i, 16, 0);
		glPopMatrix();
		if (i % 4 == 0)darkColor();
		else if (i % 4 == 1)brownColor();
		else if (i % 4 == 2)whiteColor();
		else matColor();
		glPushMatrix();
		drawBooks(-1.5 * i, 24, 0);
		glPopMatrix();
		if (i % 4 == 0)darkColor();
		else if (i % 4 == 1)brownColor();
		else if (i % 4 == 2)whiteColor();
		else matColor();
		glPushMatrix();
		drawBooks(-1.5 * i, 30, 0);
		glPopMatrix();
	}
	glPopMatrix();

	
	/*
	for (int k = 0; k < 5; k++) {
		glPushMatrix();
		glTranslatef(13.5f, 0, 30);
		for (int i = 0; i < 19; i++) {
			if (i % 3 == 0)woodColor();
			else if (i % 3 == 1)whiteColor();
			else orangeColor();
			 if (k == 0) {
				glPushMatrix();
				drawBooks(-1.5 * i, 2, 0);
				glPopMatrix();
			}
			else {
				glPushMatrix();
				drawBooks(-1.5 * i, 7 * (k)+4, 0);
				glPopMatrix();
			}
		}
		glPopMatrix();
	}
	*/

	glPopMatrix();
}



vector<P> clothBox;
vector<P> clothBoxDetail;
vector<P> clothBoxOutSide;
void drawClothBox() {
	cubeinit(13, 13, 5, clothBox);
	cubeinit(0.2, 10, 1.0, clothBoxDetail);
	glPushMatrix();

	for (int k = 0; k < 2; k++) {

		for (int i = 0; i < 5; i++) {
			glPushMatrix();
			whiteColor();
			glTranslatef(0, 4.9* i, 0);
			glPushMatrix();
			if (k == 0)
			{

				glTranslatef(-45.5f, 0.1f, 27.5f);
				printQ(clothBox,CLOTHBOXFRONT,0.5);
				//glRotatef(90, 0, 1, 0);
				glRotatef(-90, 0, 1, 0);
				glTranslatef(-6.6, 4.0, 0);

				clothboxColor();
				printQ(clothBoxDetail,DESK,1.0);
			}
			else {

				glTranslatef(-45.5f, 0.1f, 41.5f);
				printQ(clothBox, CLOTHBOXFRONT, 0.5);
				//glRotatef(90, 0, 1, 0);
				glRotatef(-180, 0, 1, 0);
				glTranslatef(-6.6, 4.0, 0);
				clothboxColor();
				printQ(clothBoxDetail, DESK, 1.0);
			}
			glPopMatrix();
			glPopMatrix();
		}
	}
	glPopMatrix();
}


vector<P> clothHanger;
void drawclothHanger() {
	cubeinit(1, 0.5, 8, clothHanger);
	deskColor();
	glPushMatrix();
	glTranslatef(-51.5f, 23.0f, -20.0f);
	glRotatef(-90, 0, 1, 0);
	glPushMatrix();
	//전체에 대한위치
	for (int i = 0; i < 3; i++) {
		glPushMatrix();//i 누적 초기화용
		glTranslatef(-7 * i, 0, 0);
		//1개에 대한 Push
		glPushMatrix();
		//glRotatef(90, 0, 1, 0);
		glRotatef(-30, 0, 0, 1);
		printQ(clothHanger,DESK,1.0);

		glPushMatrix();
		glTranslatef(-6, 3.5, 0);
		//glRotatef(60, 0, 0, 1);
		printQ(clothHanger, DESK, 1.0);
		glPopMatrix();


		glRotatef(60, 0, 0, 1);
		glPushMatrix();
		glTranslatef(0.5, -0.5, -0.5);
		printQ(clothHanger, DESK, 1.0);
		//높이   오른쪽으로   z쪽으로
		glTranslatef(6.0, 3.5, 0);
		printQ(clothHanger, DESK, 1.0);
		glPopMatrix();
		glPopMatrix();//1개에 대한 POP
		glPopMatrix();
	}
	glPopMatrix();//여러개에 대한 POP

				  //glTranslatef(0, 5, 0);


	woodColor();
	//1층
	glBindTexture(GL_TEXTURE_2D, texture[BOOK]);
	glTranslatef(0, 0.5, -1.5);
	glutSolidCylinder(0.25, 1, 10, 10);
	glTranslatef(0, 0, -0.45);
	glutSolidSphere(0.5, 10, 10);

	glTranslatef(-6.8, 0, 0.45);
	glutSolidCylinder(0.25, 1, 10, 10);
	glTranslatef(0, 0, -0.45);
	glutSolidSphere(0.5, 10, 10);

	glTranslatef(-6.8, 0, 0.45);
	glutSolidCylinder(0.25, 1, 10, 10);
	glTranslatef(0, 0, -0.45);
	glutSolidSphere(0.5, 10, 10);


	//3층
	glTranslatef(13.75, 12, 0.45);
	glutSolidCylinder(0.25, 1, 10, 10);
	glTranslatef(0, 0, -0.45);
	glutSolidSphere(0.5, 10, 10);

	glTranslatef(-6.8, 0, 0.45);
	glutSolidCylinder(0.25, 1, 10, 10);
	glTranslatef(0, 0, -0.45);
	glutSolidSphere(0.5, 10, 10);

	glTranslatef(-6.8, 0, 0.45);
	glutSolidCylinder(0.25, 1, 10, 10);
	glTranslatef(0, 0, -0.45);
	glutSolidSphere(0.5, 10, 10);
	glPushMatrix();

	glPopMatrix();
	//2층 3층에 상대적으로
	glTranslatef(17, -6, +0.45);
	glutSolidCylinder(0.25, 1, 10, 10);
	glTranslatef(0, 0, -0.45);
	glutSolidSphere(0.5, 10, 10);

	glTranslatef(-7, 0, 0.45);
	glutSolidCylinder(0.25, 1, 10, 10);
	glTranslatef(0, 0, -0.45);
	glutSolidSphere(0.5, 10, 10);
	glTranslatef(-7, 0, 0.45);
	glutSolidCylinder(0.25, 1, 10, 10);
	glTranslatef(0, 0, -0.45);
	glutSolidSphere(0.5, 10, 10);
	glTranslatef(-6.5, 0, 0.45);
	glutSolidCylinder(0.25, 1, 10, 10);
	glTranslatef(0, 0, -0.45);
	glutSolidSphere(0.5, 10, 10);
	glPopMatrix();
	glBindTexture(GL_TEXTURE_2D, 0);
}
vector<P> windows;
vector<P> windowsCenter;
vector<P> windowsUpper;
void drawWindow() {
	glPushMatrix();
	glTranslatef(52.0f, 20.0f, -10.0f);
	//glTranslatef(0, 0, -10.0f);
	cubeinit(0.5f, 35, 20, windows);

	//printQ(windows,WINDOW,1.0);
	printQ(windows,WINDOW,1.0);
	/*
	windowColor();
	glPushMatrix();
	cubeinit(0.5f, 2, 20, windowsCenter);
	glTranslatef(-0.5, 0, 0);
	print(windowsCenter);
	glTranslatef(0, 0, -16.5);
	print(windowsCenter);
	glTranslatef(0, 0, 33.0);
	print(windowsCenter);
	glPopMatrix();

	glPushMatrix();
	cubeinit(0.5, 18, 2, windowsUpper);
	glTranslatef(-0.5, 0, -8);
	print(windowsUpper);
	glTranslatef(0, 18, 0);
	print(windowsUpper);
	glTranslatef(0, 0, 16);
	print(windowsUpper);
	glTranslatef(0, -18, 0);
	print(windowsUpper);
	glPopMatrix();
	*/
	glPopMatrix();
}

vector<P> chairLeg;
vector<P> chairMain;
vector<P> chairSide;
vector<P> chairBack;
vector<P> chairbackBone;
vector<P> headPart;
vector<P> chairSideConnect;
void drawChair() {

	glPushMatrix();
	glTranslatef(30, 0.0, -18);
	cubeinit(0.5, 0.5, 8, chairLeg);
	//여기는 의자 밑부분을 만든다
	glPushMatrix();
	glTranslatef(0, 1.0, 0);

	darkColor();
	//바퀴1
	glPushMatrix();
	glTranslatef(8 * cos(45) + 1, 0, 1.1f);
	glutSolidCylinder(1, 1, 10, 10);
	glPopMatrix();
	//바퀴2
	glPushMatrix();
	glTranslatef(-0.5f, 0, 5.5f);
	glRotatef(90, 0, 1, 0);
	glutSolidCylinder(1, 1, 10, 10);
	glPopMatrix();

	//바퀴3
	glPushMatrix();
	glTranslatef(-5.3f, 0, 1.0f);
	glRotatef(35, 0, 1, 0);
	glutSolidCylinder(1, 1, 10, 10);
	glPopMatrix();

	//바퀴4
	glPushMatrix();
	glTranslatef(-3.7f, 0, -4.7f);
	glRotatef(70, 0, 1, 0);
	glutSolidCylinder(1, 1, 10, 10);
	glPopMatrix();

	//바퀴5
	glPushMatrix();
	glTranslatef(3.3f, 0, -5.0f);
	glRotatef(47, 0, 1, 0);
	glutSolidCylinder(1, 1, 10, 10);
	glPopMatrix();


	whiteColor();
	//몸통
	glTranslatef(0, 6.0f, 0.0f);
	glPushMatrix();
	glRotatef(-90, 1, 0, 0);
	glutSolidCylinder(1, 3, 10, 10);
	glPopMatrix();
	//다리 부분
	glPushMatrix();
	glRotatef(135, 1, 0, 0);
	//glRotatef(90, 1, 0, 0);
	for (int i = 0; i < 5; i++) {
		glRotatef(72, 0, 1, 1);
		printQ(chairLeg,PLASTIC,1.0);
	}
	glPopMatrix();

	glPopMatrix();

	//의자 윗부분
	cubeinit(10, 10, 1, chairMain);
	orangeColor();
	glPushMatrix();
	glTranslatef(0, 9.5f, 0);
	printQ(chairMain,CHAIR,1.0);
	glPopMatrix();


	//옆부분 
	cubeinit(1, 5, 1, chairSide);
	glPushMatrix();
	glTranslatef(-8, 14.0f, 0);
	printQ(chairSide,CHAIR,1.0);
	glTranslatef(16, 0, 0);
	printQ(chairSide, CHAIR, 1.0);
	glPopMatrix();


	//의자 뒷부분
	cubeinit(10, 1, 9, chairBack);
	glPushMatrix();
	glTranslatef(0, 14.0f, 5.0f);
	printQ(chairBack,CHAIR,1.0);
	glPopMatrix();


	whiteColor();
	cubeinit(5, 1, 3, headPart);
	glPushMatrix();
	glTranslatef(0, 23.0f, 6.0f);
	printQ(headPart,CHAIRHEAD,3.0);
	glPopMatrix();

	cubeinit(3, 1, 15, chairbackBone);
	whiteColor();
	glPushMatrix();
	glTranslatef(0, 10.0f, 5.0f);
	glRotatef(5, 1, 0, 0);
	print(chairbackBone);
	glPopMatrix();

	cubeinit(1, 2, 5, chairSideConnect);
	glPushMatrix();
	glTranslatef(-5.0f, 10.0f, 0.0f);
	glRotatef(30, 0, 0, 1);
	print(chairSideConnect);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(5.0f, 10.0f, 0.0f);
	glRotatef(-30, 0, 0, 1);
	print(chairSideConnect);
	glPopMatrix();
	glPopMatrix();
}

vector<P> curtains;
void drawcurtain() {
	cubeinit(1, 5, 46, curtains);
	curtainColor();
	glPushMatrix();
	glTranslatef(51.0f, 0.0f, 9.0f);
	for (int i = 0; i < 8; i++) {
		glPushMatrix();
		glTranslatef(0, 0, 5 * i);
		if (i % 2 == 0)
			glRotatef(13, 0, 1, 0);
		else
			glRotatef(-13, 0, 1, 0);
		printQ(curtains,CURTAIN, 0.1);
		
		glPopMatrix();
	}

	glPopMatrix();
}



vector<P> dollHanger;
vector<P> dollHangerBottom;
void drawdollHanger() {

	glPushMatrix();
	brownColor();
	glTranslatef(-49.5f, 34, 34);
	cubeinit(5, 25, 1, dollHanger);
	cubeinit(5, 1, 3, dollHangerBottom);
	printQ(dollHanger,DESK,1.0);
	glTranslatef(0, -2.5, -10);
	printQ(dollHangerBottom, DESK, 1.0);
	glTranslatef(0, 0, 20);
	printQ(dollHangerBottom, DESK, 1.0);

	glPopMatrix();
}

vector<P> A4;
void drawWallClipper() {
	whiteColor();
	cubeinit(4, 0.2,10, A4);
	glPushMatrix();
	//gltransslate
	glTranslatef(10.0f, 18.0f, 47.0f);
	glScalef(2.0, 1.2, 1.2);
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 10; j++) {
			glPushMatrix();
			if (i == 0)
				glRotatef(90, 0, 1, 0);
			else
				glRotatef(-90, 1, 0, 0);
			if (i == 0)
				glTranslatef(0, j, 0);
			else
				glTranslatef(j, 0, 0);
			glutSolidCylinder(0.1, 9, 10, 10);

			glPopMatrix();
		}
	}
	glTranslatef(6.0f, 0, -0.5f);
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, 0);
	for (int i = 0; i < A4.size(); i++) {
		
		if (i == 3) 
			glBindTexture(GL_TEXTURE_2D, texture[POSTER]);
		
		glBegin(GL_QUADS);
			glNormal3fv(A4[i].normal);
			glTexCoord2f(0.0, 1.0);
			glVertex3fv(A4[i].a);
			glTexCoord2f(0.0, 0.0);
			glVertex3fv(A4[i].b);
			glTexCoord2f(1.0, 0.0);
			glVertex3fv(A4[i].c);
			glTexCoord2f(1.0, 1.0);
			glVertex3fv(A4[i].d);
		glEnd();
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	glPopMatrix();

	glPopMatrix();
}
vector<P> doors;


void drawDoor() {
	glPushMatrix();
	glPushMatrix();
	whiteColor();
	cubeinit(20, 0.5, 40, doors);
	glTranslatef(-35.0f, -0.1f, -47.75f);
	printQ(doors,DOOR,1.0);

	curtainColor();
	glTranslatef(8, 20, -1.2);
	glutSolidSphere(1, 50, 50);
	glTranslatef(0, 0, 2.4);
	glutSolidSphere(1, 50, 50);
	glPopMatrix();// door handle


	whiteColor();
	glPushMatrix();
	glTranslatef(-51.75f, 0.0f, 0.0f);
	glRotatef(90.0f, 0, 1, 0);
	printQ(doors, DOOR, 1.0);
	glPopMatrix();//door 2

	glPopMatrix();

}
bool isMainLightsetted = false;
vector<P> mainlights;
void mainLightinit() {
	isMainLightsetted = true;
	P v;

	//형광등 방쪽으로 있는것
	v.a[0] = 6.0f; v.a[1] = 0.1f; v.a[2] = -1.0f;
	v.b[0] = 6.0f; v.b[1] = 0.1f; v.b[2] = 1.0f;
	v.c[0] = -6.0f; v.c[1] = 0.1f; v.c[2] = -1.0f;
	v.normal = cNorm(v.a, v.b, v.c);
	mainlights.push_back(v);

	v.a[0] = 6.0f; v.a[1] = 0.1f; v.a[2] = 1.0f;
	v.b[0] = -6.0f; v.b[1] = 0.1f; v.b[2] = 1.0f;
	v.c[0] = -6.0f; v.c[1] = 0.1f; v.c[2] = -1.0f;
	v.normal = cNorm(v.a, v.b, v.c);
	mainlights.push_back(v);

	//형광등 방에 붙어 있는것
	v.a[0] = 10.0f; v.a[1] = 2.0f; v.a[2] = 2.5f;
	v.b[0] = 10.0f; v.b[1] = 2.0f; v.b[2] = -2.5f;
	v.c[0] = -10.0f; v.c[1] = 2.0f; v.c[2] = -2.5f;
	v.normal = cNorm(v.a, v.b, v.c);
	mainlights.push_back(v);

	v.a[0] = 10.0f; v.a[1] = 2.0f; v.a[2] = 2.5f;
	v.b[0] = -10.0f; v.b[1] = 2.0f; v.b[2] = -2.5f;
	v.c[0] = -10.0f; v.c[1] = 2.0f; v.c[2] = 2.5f;
	v.normal = cNorm(v.a, v.b, v.c);
	mainlights.push_back(v);

	// side
	v.a[0] = 10.0f; v.a[1] = 2.0f; v.a[2] = -2.5f;
	v.b[0] = 6.0f; v.b[1] = 0.1f; v.b[2] = -1.0f;
	v.c[0] = -6.0f; v.c[1] = 0.1f; v.c[2] = -1.0f;
	v.normal = cNorm(v.a, v.b, v.c);
	mainlights.push_back(v);

	v.a[0] = -10.0f; v.a[1] = 2.0f; v.a[2] = -2.5f;
	v.b[0] = 10.0f; v.b[1] = 2.0f; v.b[2] = -2.5f;
	v.c[0] = -6.0f; v.c[1] = 0.1f; v.c[2] = -1.0f;
	v.normal = cNorm(v.a, v.b, v.c);
	mainlights.push_back(v);

	v.a[0] = 10.0f; v.a[1] = 2.0f; v.a[2] = 2.5f;
	v.b[0] = -6.0f; v.b[1] = 0.1f; v.b[2] = 1.0f;
	v.c[0] = 6.0f; v.c[1] = 0.1f; v.c[2] = 1.0f;
	v.normal = cNorm(v.a, v.b, v.c);
	mainlights.push_back(v);


	v.a[0] = 10.0f; v.a[1] = 2.0f; v.a[2] = 2.5f;
	v.b[0] = -10.0f; v.b[1] = 2.0f; v.b[2] = 2.5f;
	v.c[0] = -6.0f; v.c[1] = 0.1f; v.c[2] = 1.0f;
	v.normal = cNorm(v.a, v.b, v.c);
	mainlights.push_back(v);



	v.a[0] = -10.0f; v.a[1] = 2.0f; v.a[2] = 2.5f;
	v.b[0] = -6.0f; v.b[1] = 0.1f; v.b[2] = -1.0f;
	v.c[0] = -6.0f; v.c[1] = 0.1f; v.c[2] = 1.0f;
	v.normal = cNorm(v.a, v.b, v.c);
	mainlights.push_back(v);

	v.a[0] = -10.0f; v.a[1] = 2.0f; v.a[2] = 2.5f;
	v.b[0] = -10.0f; v.b[1] = 2.0f; v.b[2] = -2.5f;
	v.c[0] = -6.0f; v.c[1] = 0.1f; v.c[2] = -1.0f;
	v.normal = cNorm(v.a, v.b, v.c);
	mainlights.push_back(v);


	v.a[0] = 10.0f; v.a[1] = 2.0f; v.a[2] = 2.5f;
	v.b[0] = 6.0f; v.b[1] = 0.1f; v.b[2] = 1.0f;
	v.c[0] = 6.0f; v.c[1] = 0.1f; v.c[2] = -1.0f;
	v.normal = cNorm(v.a, v.b, v.c);
	mainlights.push_back(v);

	v.a[0] = 10.0f; v.a[1] = 2.0f; v.a[2] = 2.5f;
	v.b[0] = 6.0f; v.b[1] = 0.1f; v.b[2] = -1.0f;
	v.c[0] = 10.0f; v.c[1] = 2.0f; v.c[2] = -2.5f;
	v.normal = cNorm(v.a, v.b, v.c);
	mainlights.push_back(v);

}
void drawMainLight() {
	if (!isMainLightsetted)
		mainLightinit();
	glPushMatrix();
	glTranslatef(0.0f, 44.0f, 0.0f);
	glBindTexture(GL_TEXTURE_2D, texture[PLASTIC]);
	for (int i = 0; i < mainlights.size(); i++) {
		
		glBegin(GL_TRIANGLES);
		glNormal3fv(mainlights[i].normal);
		glTexCoord2f(0, 0);
		glVertex3fv(mainlights[i].a);
		glTexCoord2f(1, 0);
		glVertex3fv(mainlights[i].b);
		glTexCoord2f(0, 1);
		glVertex3fv(mainlights[i].c);
		glEnd();
	}
	glBindTexture(GL_TEXTURE_2D, 0);

	glPopMatrix();

}

vector<P> bedframe;
vector<P> bedmat;
vector<P> bedTop, bedBottom;
vector<P> pillow;
vector<P> bedDetails;
vector<P> upperDetails;
void drawbed2() {
	glPushMatrix();
	glTranslatef(14.0f, -0.1f, 28.0f);

	//1 FRAME
	woodColor();
	cubeinit(70, 40, 5, bedframe);
	printQ(bedframe,DESK,1.0);

	//2 MAT
	matColor();
	glPushMatrix();
	cubeinit(70, 39, 8, bedmat);
	glTranslatef(0.0f, 5.1f, 0.0f);
	printQ(bedmat,BED,3.0);
	glPopMatrix();

	///////////////////////////////////////////
	bedUpperColor2();
	glPushMatrix();
	glTranslatef(34.75, 15.0, 0);
	cubeinit(0.5, 40, 5, upperDetails);
	printQ(upperDetails,DESK,1.0);
	glPopMatrix();




	bedUpperColor();

	//3 BED UPPER
	glPushMatrix();
	glTranslatef(35.5f, 0, 0);
	cubeinit(1, 40, 20, bedTop);
	printQ(bedTop,DESK,1.0);
	glPopMatrix();


	//4 BED LOWER
	glPushMatrix();
	glTranslatef(-35.5f, 0, 0);
	cubeinit(1, 40, 9, bedBottom);
	printQ(bedBottom,DESK,1.0);
	glPopMatrix();
	//5 BED DETAILS

	glPushMatrix();
	cubeinit(21.5, 1, 3, bedDetails);
	glTranslatef(22, 1.0, -20);
	printQ(bedDetails,PLASTIC,1.0);
	glTranslatef(-22, 0, 0);
	printQ(bedDetails, PLASTIC, 1.0);
	glTranslatef(-22, 0, 0);
	printQ(bedDetails, PLASTIC, 1.0);
	glPopMatrix();

	//darkColor();
	//6 Pillow
	whiteColor();
	glPushMatrix();
	cubeinit(8, 15, 2, pillow);
	glTranslatef(28, 13, 0);
	printQ(pillow,PROCEDURAL,1.0);
	glPopMatrix();

	//전체 침대에 대한 pop
	glPopMatrix();

}


vector<P> airconbody;
void drawAircon() {
	whiteColor();
	glPushMatrix();
	glTranslatef(40.0f, 36.0f, 45.5);
	cubeinit(20, 5, 10, airconbody);
	for (int i = 0; i < airconbody.size(); i++) {
		if (i == 3)
			glBindTexture(GL_TEXTURE_2D, texture[AIRCON]);
		else 
			glBindTexture(GL_TEXTURE_2D, texture[PLASTIC]);
		glBegin(GL_QUADS);

		glNormal3fv(airconbody[i].normal);
		glTexCoord2f(0.0, 1.0);
		glVertex3fv(airconbody[i].a);
		glTexCoord2f(0.0, 0.0);
		glVertex3fv(airconbody[i].b);
		glTexCoord2f(1.0, 0.0);
		glVertex3fv(airconbody[i].c);
		glTexCoord2f(1.0, 1.0);
		glVertex3fv(airconbody[i].d);
		glEnd();
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	//printQ(airconbody,4);
	
	glPopMatrix();
}

vector<P> Poster;
void drawPoster() {
	cubeinit(10, 0.5, 13, Poster);
}
void lightPosInit() {
	GLfloat light0_position[] = { 0.0f, 46.0f, 0.0f, 1.0 };//메인빛
	GLfloat light1_position[] = { 52.2f, 30.0f, -20.0f, 1.0 };//밖에빛
	GLfloat light2_position[] = { 30.0f, 20.5f, -41.5f,1.0 };//책상빛
	glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
	glLightfv(GL_LIGHT1, GL_POSITION, light1_position);
	glLightfv(GL_LIGHT2, GL_POSITION, light2_position);
	//glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, light_direction);
}
void mydisplay() {
	glClearColor(0.0, 0.0, 0.0, 0.0);
	//카메라 설정
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glShadeModel(GL_SMOOTH);
	gluPerspective(90.0f, 1.0, 0.1f, 1000.0f);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glMultMatrixf(cam->getViewMatrix());

	lightPosInit();

	//1 방외벽
	drawOutRoom();

	//2 침대
	drawbed2();

	//위에 조명 모양
	drawMainLight();
	//에어컨
	drawAircon();
	//창문
	drawWindow();
	//책상
	drawDesk();
	//책장
	drawBookshelf();
	//문
	drawDoor();
	//의자
	drawChair();
	//옷거는거
	drawclothHanger();
	//박스 위
	drawdollHanger();
	//옷넣는 박스
	drawClothBox();
	//방면에 붙어있는 clipper
	drawWallClipper();
	//커텐
	drawcurtain();

	//CALL
	drawPoster();
	

	glPushMatrix();
	glTranslatef(0.0f, 20.0f, 47.0f);
	printQatFace(Poster, POSTER1, 5.0,3);
	glPopMatrix();

	
	glPushMatrix();
	glTranslatef(-20.0f, 20.0f, 47.0f);
	printQatFace(Poster, POSTER2, 2.0,3);

	glPopMatrix();
	
	
	


	glutSwapBuffers();

}

void lightinit() {

	GLfloat light0_ambient[] = { 101.0*b, 101.0*b, 101.0*b, 1.0 };  //메인 빛
	GLfloat light0_diffuse[] = { 101.0*b, 101.0*b, 60.0*b, 1.0 };
	GLfloat light0_specular[] = { 80.0*b, 80.0*b, 80.0*b, 1.0 };

	GLfloat light1_ambient[] = { 60.0, 10.0, 10.0, 1.0 }; //밖에빛
	GLfloat light1_diffuse[] = { 6.0, 1.0, 1.0, 1.0 };
	GLfloat light1_specular[] = { 1.0, 1.0, 1.0, 1.0 };

	GLfloat light2_ambient[] = { 20.0,20.0,40.0, 1.0 };//책상빛
	GLfloat light2_diffuse[] = { 1.0, 1.0, 0.0, 1.0 };
	GLfloat light2_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	glLightfv(GL_LIGHT0, GL_AMBIENT, light0_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light0_specular);


	glLightfv(GL_LIGHT1, GL_AMBIENT, light1_ambient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light1_diffuse);
	glLightfv(GL_LIGHT1, GL_SPECULAR, light1_specular);


	glLightfv(GL_LIGHT2, GL_AMBIENT, light2_ambient);
	glLightfv(GL_LIGHT2, GL_DIFFUSE, light2_diffuse);
	glLightfv(GL_LIGHT2, GL_SPECULAR, light2_specular);


	glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 1.0f);
	glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.25f);
	glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.1f);

	glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, 1.0f);
	glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, 0.25f);
	glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, 0.1f);

	glLightf(GL_LIGHT2, GL_CONSTANT_ATTENUATION, 1.0f);
	glLightf(GL_LIGHT2, GL_LINEAR_ATTENUATION, 0.25f);
	glLightf(GL_LIGHT2, GL_QUADRATIC_ATTENUATION, 0.1f);

	//glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, 80.0f);
	//glLightf(GL_LIGHT2, GL_SPOT_EXPONENT, 80.0f);


	//빛 광량 관련되어있음
	//glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);


}
void enableinit() {

	glShadeModel(GL_SMOOTH);

	//LIGHT 활성화
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	glEnable(GL_LIGHT2);

	//glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	//glColorMaterial(GL_FRONT, GL_SPECULAR);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);
	glFrontFace(GL_CCW);
}
void textureinit() {
	stbi_set_flip_vertically_on_load(true);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(20, texture);
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	unsigned char *FLOORIMAGE = stbi_load("./data/128.jpg", &width, &height, &nrChannels, 0);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, FLOORIMAGE);
	
	stbi_image_free(FLOORIMAGE);

	glBindTexture(GL_TEXTURE_2D, texture[1]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	unsigned char *ws = stbi_load("./data/resized/realwall.jpg", &width, &height, &nrChannels, 0);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, ws);
	stbi_image_free(ws);

	glBindTexture(GL_TEXTURE_2D, texture[2]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	unsigned char *bss = stbi_load("./data/bookshelfside.jpg", &width, &height, &nrChannels, 0);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, bss);
	stbi_image_free(bss);

	glBindTexture(GL_TEXTURE_2D, texture[3]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	unsigned char *desks = stbi_load("./data/resized/deskside.jpg", &width, &height, &nrChannels, 0);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, desks);
	stbi_image_free(desks);

	glBindTexture(GL_TEXTURE_2D, texture[4]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	unsigned char *airconbottom = stbi_load("./data/front.jpg", &width, &height, &nrChannels, 0);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, airconbottom);
	stbi_image_free(airconbottom);


	glBindTexture(GL_TEXTURE_2D, texture[5]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	unsigned char *ct = stbi_load("./data/resized/curtain.jpg", &width, &height, &nrChannels, 0);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, ct);
	stbi_image_free(ct);
	
	glBindTexture(GL_TEXTURE_2D, texture[6]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	unsigned char *deskfront = stbi_load("./data/resized/bookfront.jpg", &width, &height, &nrChannels, 0);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, deskfront);
	stbi_image_free(deskfront);

	
	glBindTexture(GL_TEXTURE_2D, texture[7]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	unsigned char *poem = stbi_load("./data/resized/poem.jpg", &width, &height, &nrChannels, 0);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, poem);
	stbi_image_free(poem);
	

	glBindTexture(GL_TEXTURE_2D, texture[8]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	unsigned char *door = stbi_load("./data/door.jpg", &width, &height, &nrChannels, 0);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, door);
	stbi_image_free(door);
	

	glBindTexture(GL_TEXTURE_2D, texture[9]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	unsigned char *clothboxf = stbi_load("./data/clothbox_front.jpg", &width, &height, &nrChannels, 0);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, clothboxf);
	stbi_image_free(clothboxf);

	glBindTexture(GL_TEXTURE_2D, texture[10]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, ImageWidth, ImageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, MatrixImage);


	glBindTexture(GL_TEXTURE_2D, texture[11]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	unsigned char *WIN = stbi_load("./data/window2.jpg", &width, &height, &nrChannels, 0);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, WIN);
	stbi_image_free(WIN);

	glBindTexture(GL_TEXTURE_2D, texture[12]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	unsigned char *OR = stbi_load("./data/chair.jpg", &width, &height, &nrChannels, 0);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, OR);
	stbi_image_free(OR);



	
	glBindTexture(GL_TEXTURE_2D, texture[13]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	unsigned char *CHH = stbi_load("./data/ch.jpg", &width, &height, &nrChannels, 0);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, CHH);
	stbi_image_free(CHH);

	glBindTexture(GL_TEXTURE_2D, texture[14]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	unsigned char *BDD = stbi_load("./data/bed.jpg", &width, &height, &nrChannels, 0);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, BDD);
	stbi_image_free(BDD);

	glBindTexture(GL_TEXTURE_2D, texture[15]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	unsigned char *PLS = stbi_load("./data/plastic.jpg", &width, &height, &nrChannels, 0);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, PLS);
	stbi_image_free(PLS);


	glBindTexture(GL_TEXTURE_2D, texture[16]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	unsigned char *POS1 = stbi_load("./data/poster1.jpg", &width, &height, &nrChannels, 0);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, POS1);
	stbi_image_free(POS1);

	glBindTexture(GL_TEXTURE_2D, texture[17]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	unsigned char *POS2 = stbi_load("./data/poster2.jpg", &width, &height, &nrChannels, 0);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, POS2);
	stbi_image_free(POS2);

	glBindTexture(GL_TEXTURE_2D, texture[18]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	unsigned char *BC = stbi_load("./data/bookcover.jpg", &width, &height, &nrChannels, 0);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, BC);
	stbi_image_free(BC);

}
void Init() {

	makeImage();
	lightinit();
	enableinit();
	
	//물체마다 다르게 설정
	cam->setWindowSize(800, 800);

	//여기에서 모든 이미지 로딩한다
	glEnable(GL_TEXTURE_2D);
	//glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	textureinit();
}
void mouseStatus(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		cam->setCurrentMousePos(x, y);
		cam->beginRotate();
	}
	else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
		cam->endRotate();
	}
	else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
		cam->setCurrentMousePos(x, y);
		cam->beginZoom();
	}
	else if (button == GLUT_RIGHT_BUTTON && state == GLUT_UP) {
		cam->endZoom();
	}
	else if (button == GLUT_MIDDLE_BUTTON && state == GLUT_DOWN) {
		cam->setCurrentMousePos(x, y);
		cam->beginPan();
	}
	else if (button == GLUT_MIDDLE_BUTTON && state == GLUT_UP) {
		cam->endPan();
	}
	glutPostRedisplay();
}
void mouseMotion(int x, int y) {
	cam->setCurrentMousePos(x, y);
	glutPostRedisplay();
}
void keyboardInput(unsigned char key, int x, int y) {
	if (key == '0') {
		glDisable(GL_LIGHT0);
		glDisable(GL_LIGHT1);
		glDisable(GL_LIGHT2);

	}
	else if (key == '1')//1
	{
		glDisable(GL_LIGHT0);
		glDisable(GL_LIGHT1);//light source 1
		glEnable(GL_LIGHT2);

	}
	else if (key == '2')//2
	{ // main light + desk light
		glEnable(GL_LIGHT0);
		glEnable(GL_LIGHT1);
		glDisable(GL_LIGHT2);

	}
	else if (key == '3') {//3
		glEnable(GL_LIGHT0);
		glEnable(GL_LIGHT1);
		glEnable(GL_LIGHT2);


	}
	else if (key == 'T' || key == 't') {
		glEnable(GL_TEXTURE_2D);
	}
	else if (key == 'N' || key == 'n') {
		glDisable(GL_TEXTURE_2D);
	}
	//여기부터 “Minification - Simple",
	//"Minification - Smooth", "Magnification - Simple", and "Magnification - Smooth".
	//선택하는거 구현
	//1 Minification-Simple Nearest
	else if (key == 'q' || key == 'Q') {
		for (int i = 0; i < 20; i++) {
			glBindTexture(GL_TEXTURE_2D, texture[i]);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glBindTexture(GL_TEXTURE_2D, 0);
		}
	}
	else if (key == 'w' || key == 'W') {
		for (int i = 0; i < 20; i++) {
			glBindTexture(GL_TEXTURE_2D, texture[i]);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glBindTexture(GL_TEXTURE_2D, 0);
		}
	}
	//MAG
	else if (key == 'e' || key == 'E') {
		//NEAR
		for (int i = 0; i < 20; i++) {
			glBindTexture(GL_TEXTURE_2D, texture[i]);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glBindTexture(GL_TEXTURE_2D, 0);
		}
	}
	else if (key == 'r' || key == 'R') {
		//other Nearest Neighbor
		for (int i = 0; i < 20; i++) {
			glBindTexture(GL_TEXTURE_2D, texture[i]);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glBindTexture(GL_TEXTURE_2D, 0);
		}
	}
	else if (key == 'a' || key == 'A') {
		for (int i = 0; i < 20; i++) {
			glBindTexture(GL_TEXTURE_2D, texture[i]);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
			glBindTexture(GL_TEXTURE_2D, 0);
		}
	}
	else if (key == 's' || key == 'S') {
		for (int i = 0; i < 20; i++) {
			glBindTexture(GL_TEXTURE_2D, texture[i]);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
			glBindTexture(GL_TEXTURE_2D, 0);
		}
	}
	//init
	else if (key == 'i' || key == 'I') {

		//camera init
		cam = new Camera;
		cam->setWindowSize(800, 800);
		cam->setEye(0, 20, 0);
		cam->setCenter(10, 23, 10);
		
		//texture ALL LINEAR
		for (int i = 0; i < 20; i++) {
			glBindTexture(GL_TEXTURE_2D, texture[i]);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
			glBindTexture(GL_TEXTURE_2D, 0);
		}
		//LIGHTING INIT
		glEnable(GL_LIGHT0);
		glEnable(GL_LIGHT1);
		glEnable(GL_LIGHT2);


		
	}
	else if (key == 'm' || key == 'M') {
		makeImage();
		glBindTexture(GL_TEXTURE_2D, texture[10]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, ImageWidth, ImageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, MatrixImage);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

int main(int argc, char** argv) {

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	//더블 버퍼링/ RGBA/ DEPTH를 가지는 framebuffer 설계
	glutInitWindowSize(800, 800);
	glutInitWindowPosition(30, 30);
	glutCreateWindow("16011105 이재성");
	//사이즈 및 위치 설정
	cam = new Camera;
	cam->setEye(0, 20, 0);
	cam->setCenter(10, 23, 10);

	glutDisplayFunc(mydisplay);
	glutIdleFunc(mydisplay);
	Init();
	// Projection 관련 설정

	glutMouseFunc(mouseStatus);
	glutMotionFunc(mouseMotion);
	//마우스 관련 콜백

	glutKeyboardFunc(keyboardInput);


	glutMainLoop();
	//무한루프

	return 0;
}






