#pragma once
#include "bmpfuncs.h"
#include <freeglut.h>
#include <GLFW/glfw3.h>
#include <glut.h>
#include <GL/GLU.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "Matrix4.h"
#include "ObjReader.h"
#include "Vector3.h"
#include "Vector4.h"
#include <string>
#include <gl/glext.h>
#include "SoundEngine.h"
#include "colorMenu.h"

#pragma warning(disable:4996)

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define PI 3.14159265
using namespace std;
using namespace jm;

int pos_x=0;
int pos_y = 0;
int pos_z = 0;
double theta = 45.0;
double phi = 45.0;
int radius = 18;
double upVector = cos(phi*PI / 180);

double *angle = new double[7]();
double rotateFactor = 1.0;
bool spinEnable = true;
bool display_axis = false;
bool set_materialProperty = true;
int current_width, current_height;

/* camera location */
double eyex, eyey, eyez;

/* texture mapping set variable */
GLuint texName[6];	// make cube
GLuint texName2[3];	// make cylinder
GLuint *texName3 = new GLuint();
vector<OBJReader*> objreader;
/* quadric object 생성 */
GLUquadricObj* qobj = gluNewQuadric();

void init(void);
void resize(int, int);

void draw(void);
void draw_textureCube(void);
void draw_cylinder(void);
void draw_sphere(void);
void draw_string(void*, const char*, float, float, float, float, float);

void mouse(int, int, int, int);
void mouseWheel(int but, int dir, int x, int y);
void keyboard(unsigned char, int, int);
void specialKeyboard(int, int, int);

void light_default(void);
void printInstruction(void);
void setDefault(void);

void cubeTextureMapping(void);
void cylinderTextureMapping(void);
void sphereTextureMapping(void);

void draw_env(void);
void draw_skybox(void);
void draw_obj(std::string);

GLint submenul;

std::string filepath="01-b.obj";

double objnum = 1.0f;
typedef unsigned char uchar;
int mainwindow;

SoundEngine sound_engine;
OBJReader* objreadr;
int main(int argc, char **argv) {
	/* Initialize window */
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);

	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutInitWindowPosition(400, 300);
	mainwindow=glutCreateWindow("Lego GL Program");
	init();
	/*메뉴 설정*/
	submenul = glutCreateMenu(sub_menu_function);
	//sub메뉴 내용은 여기
	glutAddMenuEntry("Yes!", 10);
	glutAddMenuEntry("No!", 11);
	glutCreateMenu(sub_menu_function);
	//메인메뉴 내용은 여기
	glutAddMenuEntry("Color: red", 1);
	glutAddMenuEntry("Color: blue", 2);
	glutAddMenuEntry("Color: green", 3);
	glutAddMenuEntry("Color: black", 4);
	glutAddMenuEntry("Color: white", 5);
	glutAddMenuEntry("Color: brown", 6);
	glutAddMenuEntry("Color: yellow", 7);
	glutAddMenuEntry("Color: magenta", 8);
	//sub메뉴를 향하는 메인\
	메뉴 내용
	glutAddSubMenu("reset?", submenul);

	glutAttachMenu(GLUT_RIGHT_BUTTON);

	/* quadric object 속성 설정 */
	gluQuadricDrawStyle(qobj, GLU_FILL);
	gluQuadricNormals(qobj, GLU_SMOOTH);

	/* Define Draw Callback */
	glutDisplayFunc(draw);
	/* 키보드 인터럽트 처리 */
	glutKeyboardFunc(keyboard);

	/* 특수 키보드 방향키 처리 */
	glutSpecialFunc(specialKeyboard);

	/* 마우스 처리 */
	glutMouseFunc(mouse);
	glutMouseWheelFunc(mouseWheel);
	/* Main Loop Start! */
	glutMainLoop();
	return 0;
}

void init(void) {
	/* Background color :  Black */
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	/* Default Object Color Set : Black */
	glColor3f(1.0f, 1.0f, 1.0f);

	/* resize callback */
	glutReshapeFunc(resize);

	/* light setting */
	light_default();

	/* 조작법 출력 */
	printInstruction();

	/* TEXTURE MAPPING SET */
	cubeTextureMapping();
	cylinderTextureMapping();
	sphereTextureMapping();
	gluQuadricTexture(qobj, GL_TRUE);

	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);	//polygon의 원래 색상은 무시하고 texture로 덮음
	glEnable(GL_TEXTURE_2D);

	//draw_env();
	/* Front face */
	glFrontFace(GL_CCW);

	/* Enable depth buffer */
	glEnable(GL_DEPTH_TEST);

}
void draw(void) {

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		eyex = radius * sin(theta*PI / 180)*cos(phi*PI / 180);
		eyey = radius * sin(phi*PI / 180);
		eyez = radius * cos(theta*PI / 180)*cos(phi*PI / 180);
		upVector = cos(phi*PI / 180); // simplify upvector correspond to y-axis

		GLfloat light_position[] = { 0.f, 0.f, 0.f, 1.f };
		glLightfv(GL_LIGHT0, GL_POSITION, light_position);
		gluLookAt(eyex, eyey, eyez, 0, 0, 0, 0, upVector, 0);
		//draw_obj("Lego_base.obj");
		//draw_skybox();
		glPushMatrix();
		glTranslatef(pos_x, pos_y, pos_z);
		//1~4구의 레고블럭의 크기 조정
		glScalef(objnum, 1.0f, 1.0f);
		draw_obj(filepath);
		glPopMatrix();
		glFlush();
		glutSwapBuffers();

}

void resize(int width, int height) {
	glViewport(0, 0, width, height);
	printf("[resize] WIDTH : %d,  HEIGHT : %d\n", width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45, (float)width / (float)height, 1, 500);
	glMatrixMode(GL_MODELVIEW);
}

void mouse(int button, int state, int x, int y) {
}

void mouseWheel(int but, int dir, int x, int y)
{
	if (dir > 0) {
		if (radius > 2) radius--;
	}
	else {
		if (radius < 100) radius++;
	}
	glutPostRedisplay();
}

void light_default() {
	glClearColor(0, 0, 0, 1.0f);

	GLfloat ambientLight[] = { 0.2f, 0.2f, 0.3f, 1.0f };
	GLfloat diffuseLight[] = { 0.5f, 0.5f, 0.5f, 1.0f };
	GLfloat specularLight[] = { 1.0f, 1.0f, 1.0f, 1.0f };

	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);

	/************* spot position setting *************/
	GLfloat spot_direction[] = { 0.0, 0.0, -2.0, 1.0 };
	glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 60.0);
	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, spot_direction);
	glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, 5);

	//매질을 표현하기위해 값을 조정
	GLfloat ambientMaterial[] = { 0.2f, 0.2f, 0.2f, 1.0f };
	GLfloat diffuseMaterial[] = { 0.5f, 0.5f, 0.5f, 1.0f };
	GLfloat specularMaterial[] = { 1.0f, 1.0f, 1.0f, 1.0f };

	/************* Material  setting *************/
	glMaterialfv(GL_FRONT, GL_AMBIENT, ambientMaterial);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuseMaterial);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specularMaterial);
	glMaterialf(GL_FRONT, GL_SHININESS, 100);

	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

	/* global light setting */
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
}

void keyboard(unsigned char key, int x, int y) {
	if (key == 100) {
		pos_x++;
		printf("you pressed %c\n", key);
		sound_engine.createSound("lego_sound.wav", "legosound", false);
		sound_engine.playSound("legosound");
	}
	else if (key == 97) {
		pos_x--;
		printf("you pressed %c\n", key);
		sound_engine.createSound("lego_sound.wav", "legosound", false);
		sound_engine.playSound("legosound");
	}
	else if (key == 115) {
		pos_y--;
		printf("you pressed %c\n", key);
		sound_engine.createSound("lego_sound.wav", "legosound", false);
		sound_engine.playSound("legosound");
	}
	else if (key == 119) {
		pos_y++;
		printf("you pressed %c\n", key);
		sound_engine.createSound("lego_sound.wav", "legosound", false);
		sound_engine.playSound("legosound");
	}
	else if (key == 113) {
		pos_z++;
		printf("you pressed %c\n", key);
		sound_engine.createSound("lego_sound.wav", "legosound", false);
		sound_engine.playSound("legosound");
	}
	else if (key == 101) {
		pos_z--;
		printf("you pressed %c\n", key);
		sound_engine.createSound("lego_sound.wav", "legosound", false);
		sound_engine.playSound("legosound");
	}
	else if (key == '1') {
		objnum = 1;
		filepath = "01-b.obj";
		printf("you pressed %c\n", key);
	}
	else if (key == '2') {
		objnum = 2;
		filepath = "02-b.obj";
		printf("you pressed %c\n", key);
	}
	else if (key == '3') {
		objnum = 3;
		filepath = "03-b.obj";
		printf("you pressed %c\n", key);
	}
	else if (key == '4') {
		objnum = 4;
		filepath = "04-b.obj";
		printf("you pressed %c\n", key);
	}
	else if (key == '5') {
		objnum = 1;
		filepath = "01-s.obj";
		printf("you pressed %c\n", key);
	}
	else if (key == '6') {
		objnum = 2;
		filepath = "02-s.obj";
		printf("you pressed %c\n", key);
	}
	else if (key == '7') {
		objnum = 3;
		filepath = "03-s.obj";
		printf("you pressed %c\n", key);
	}
	else if (key == '8') {
		objnum = 4;
		filepath = "04-s.obj";
		printf("you pressed %c\n", key);
	}
	glutPostRedisplay();
}
void specialKeyboard(int key, int x, int y) {
	switch (key) {
	case GLUT_KEY_LEFT:
		theta -= 1;
		if (theta <= -360) theta = 0.0;
		break;
	case GLUT_KEY_RIGHT:
		theta += 1;
		if (theta >= 360) theta = 0.0;
		break;
	case GLUT_KEY_DOWN:
		phi -= 1;
		if (abs((int)phi) % 360 == 0) phi = 0.0;
		break;
	case GLUT_KEY_UP:
		phi += 1;
		if (abs((int)phi) % 360 == 0) phi = 0.0;
		break;
	case 5:
		printInstruction();
		setDefault();
		break;
	default:
		printf("%d is pressed\n", key);
		break;
	}
	glutPostRedisplay();
}

void printInstruction() {
	/* 조작법 console 출력 */
	printf("\n------------keyboard navigation------------\n");
	printf("방향키 : camera 위치\n");
	printf("W,A,S,D: OBJ 좌우상하 이동키\n Q,E: OBJ 앞뒤 이동키\n");
	printf("마우스 우측버튼: 컬러조정\n");
	printf("F5 : 새로고침\n\n");
}

void setDefault() {
	radius = 18;
	theta = 45.0;
	phi = 45.0;
}

void draw_string(void* font, const char* str, float x_position, float y_position, float red, float green, float blue) {
	glPushAttrib(GL_LIGHTING_BIT);
	glDisable(GL_LIGHTING);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(-10, 10, -10, 10);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glColor3f(red, green, blue);
	glRasterPos3f(x_position, y_position, 0);
	for (unsigned int i = 0; i < strlen(str); i++) {
		glutBitmapCharacter(font, str[i]);
	}
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopAttrib();
}

void cubeTextureMapping() {
	glGenTextures(6, texName);
	int imgWidth, imgHeight, channels;
	for (int i = 0; i < 6; i++) {
		glBindTexture(GL_TEXTURE_2D, texName[i]);
		char buf[100];
		sprintf(buf, "TexImage%d.bmp", i);
		buf[strlen(buf)] = 0;
		uchar* img = readImageData(buf, &imgWidth, &imgHeight, &channels);
		//printf("[cube]img%d width : %d, height : %d, ch : %d\n", i, imgWidth, imgHeight, channels);
		glTexImage2D(GL_TEXTURE_2D, 0, /*INPUT CHANNEL*/3, imgWidth, imgHeight, 0, /*TEXEL CHANNEL*/GL_RGB, GL_UNSIGNED_BYTE, img);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	}
}

void draw_textureCube() {
	glColor3f(1.0, 1.0, 1.0);	// white로 color를 set해주어야 texture색상이 제대로 적용 됨!
	//glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE); // polygon의 원래 색상은 무시하고 texture로 덮음
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE); // polygon의 원래 색상과 texture 색상을 곱하여 덮음, texture가 입혀진 표면에 광원 효과 설정 가능
	// glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

	glBindTexture(GL_TEXTURE_2D, texName[0]);
	glBegin(GL_QUADS);
	glNormal3f(-1.0, 0, 0);	// -x axis
	glTexCoord2f(0, 0); glVertex3f(-1.0, 1.0, -1.0);
	glTexCoord2f(1, 0); glVertex3f(-1.0, -1.0, -1.0);
	glTexCoord2f(1, 1); glVertex3f(-1.0, -1.0, 1.0);
	glTexCoord2f(0, 1); glVertex3f(-1.0, 1.0, 1.0);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, texName[1]);
	glBegin(GL_QUADS);
	glNormal3f(1.0, 0, 0);	//x axis
	glTexCoord2f(0, 0); glVertex3f(1.0, 1.0, 1.0);
	glTexCoord2f(1, 0); glVertex3f(1.0, -1.0, 1.0);
	glTexCoord2f(1, 1); glVertex3f(1.0, -1.0, -1.0);
	glTexCoord2f(0, 1); glVertex3f(1.0, 1.0, -1.0);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, texName[2]);
	glBegin(GL_QUADS);
	glNormal3f(0, -1.0, 0);	// -y axis
	glTexCoord2f(0, 0); glVertex3f(-1.0, -1.0, -1.0);
	glTexCoord2f(1, 0); glVertex3f(1.0, -1.0, -1.0);
	glTexCoord2f(1, 1); glVertex3f(1.0, -1.0, 1.0);
	glTexCoord2f(0, 1); glVertex3f(-1.0, -1.0, 1.0);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, texName[3]);
	glBegin(GL_QUADS);
	glNormal3f(0, 1.0, 0);	// y axis
	glTexCoord2f(0, 0); glVertex3f(-1.0, 1.0, 1.0);
	glTexCoord2f(1, 0); glVertex3f(1.0, 1.0, 1.0);
	glTexCoord2f(1, 1); glVertex3f(1.0, 1.0, -1.0);
	glTexCoord2f(0, 1); glVertex3f(-1.0, 1.0, -1.0);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, texName[4]);
	glBegin(GL_QUADS);
	glNormal3f(0, 0, 1.0);	//z axis
	glTexCoord2f(0, 0); glVertex3f(1.0, 1.0, 1.0);
	glTexCoord2f(1, 0); glVertex3f(-1.0, 1.0, 1.0);
	glTexCoord2f(1, 1); glVertex3f(-1.0, -1.0, 1.0);
	glTexCoord2f(0, 1); glVertex3f(1.0, -1.0, 1.0);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, texName[5]);
	glBegin(GL_QUADS);
	glNormal3f(0, 0, -1.0);	//-z축
	glTexCoord2f(0, 0); glVertex3f(1.0, 1.0, -1.0);
	glTexCoord2f(1, 0); glVertex3f(-1.0, 1.0, -1.0);
	glTexCoord2f(1, 1); glVertex3f(-1.0, -1.0, -1.0);
	glTexCoord2f(0, 1); glVertex3f(1.0, -1.0, -1.0);
	glEnd();
}

void draw_cylinder() {
	glRotatef(-90, 1, 0, 0);	// 깡통 세우기
	glTranslatef(0, 0, -1);		// 다른 object랑 y축으로 맞추기
	glColor3f(1.0, 1.0, 1.0);

	/* cylinder top view */
	glPushMatrix();
	glTranslatef(0, 0, 2);
	glBindTexture(GL_TEXTURE_2D, texName2[0]);
	gluPartialDisk(qobj, 0, 0.7, 30, 3, 0, 360);
	glPopMatrix();

	/* cylinder middle */
	glBindTexture(GL_TEXTURE_2D, texName2[1]);
	gluCylinder(qobj, 0.7, 0.7, 2, 20, 20);

	/* cylinder bottom view */
	glPushMatrix();
	glRotatef(180, 1, 0, 0);	// normal vector 설정과 같음
	glBindTexture(GL_TEXTURE_2D, texName2[2]);
	gluPartialDisk(qobj, 0, 0.7, 30, 3, 0, 360);
	glPopMatrix();
}

void draw_sphere() {
	glRotatef(-90, 1, 0, 0);
	glColor3f(1.0, 1.0, 1.0);

	glBindTexture(GL_TEXTURE_2D, *texName3);
	gluSphere(qobj, 0.7, 100, 100);
}

void cylinderTextureMapping() {
	glGenTextures(3, texName2);

	glBindTexture(GL_TEXTURE_2D, texName2[0]);
	int width, height, channels;
	uchar* img = readImageData("CIDER_T.bmp", &width, &height, &channels);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, img);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, texName2[1]);
	img = readImageData("CIDER_S.bmp", &width, &height, &channels);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, img);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, texName2[2]);
	img = readImageData("CIDER_B.bmp", &width, &height, &channels);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, img);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
}

void sphereTextureMapping() {
	glGenTextures(1, texName3);
	uchar* img;
	int width, height, channels;

	glBindTexture(GL_TEXTURE_2D, *texName3);
	img = readImageData("EARTH.bmp", &width, &height, &channels);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, img);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
}
void draw_obj(std::string x) {

		OBJReader obj_reader;
		obj_reader.readObj(x);
		glBegin(GL_TRIANGLES);
		for (int i = 0; i < obj_reader.ix_stack_.size(); i++) {

			const int ix0 = obj_reader.ix_stack_[i].x_;
			const int ix1 = obj_reader.ix_stack_[i].y_;
			const int ix2 = obj_reader.ix_stack_[i].z_;

			const Vector3<float> &v0 = obj_reader.pos_stack_[ix0];
			const Vector3<float> &v1 = obj_reader.pos_stack_[ix1];
			const Vector3<float> &v2 = obj_reader.pos_stack_[ix2];

			glColor3f(1.0, 0.0, 0.0);
			glColor3f(color_x,color_y,color_z);
			glVertex3fv(v0.v_);
			glColor3f(color_x+0.2, color_y+0.2, color_z+0.2);
			glVertex3fv(v1.v_);
			glColor3f(color_x+0.5, color_y+0.5, color_z+0.5);
			glVertex3fv(v2.v_);
		}
		glEnd();
}

unsigned int g_nCubeTex;
void draw_env() {
	glGenTextures(1, &g_nCubeTex);
	int width, height, channels;
	uchar* img0 = readImageData("512px.bmp", &width, &height, &channels);
	uchar* img1 = readImageData("512nx.bmp", &width, &height, &channels);
	uchar* img2 = readImageData("512py.bmp", &width, &height, &channels);
	uchar* img3 = readImageData("512ny.bmp", &width, &height, &channels);
	uchar* img4 = readImageData("512pz.bmp", &width, &height, &channels);
	uchar* img5 = readImageData("512nz.bmp", &width, &height, &channels);

	glBindTexture(GL_TEXTURE_CUBE_MAP, g_nCubeTex);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, img0);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, img1);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, img2);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, img3);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, img4);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, img5);

	glBindTexture(GL_TEXTURE_CUBE_MAP, g_nCubeTex);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP);
	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP);
	glTexGeni(GL_R, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP);

	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);
	glEnable(GL_TEXTURE_GEN_R);
	glEnable(GL_TEXTURE_CUBE_MAP);
	glBindTexture(GL_TEXTURE_CUBE_MAP, g_nCubeTex);


	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);
	glDisable(GL_TEXTURE_GEN_R);
	glDisable(GL_TEXTURE_CUBE_MAP);

}

void draw_skybox()
{
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glDisable(GL_LIGHTING);
	glEnable(GL_TEXTURE_CUBE_MAP);
	glBindTexture(GL_TEXTURE_CUBE_MAP, g_nCubeTex);

	float g_nskysize = 15.0f;

	glBegin(GL_QUADS);

	glTexCoord3d(1.0, -1.0, -1.0); glVertex3f(g_nskysize, -g_nskysize, -g_nskysize);
	glTexCoord3d(1.0, -1.0, 1.0); glVertex3f(g_nskysize, -g_nskysize, g_nskysize);
	glTexCoord3d(1.0, 1.0, 1.0); glVertex3f(g_nskysize, g_nskysize, g_nskysize);
	glTexCoord3d(1.0, 1.0, -1.0); glVertex3f(g_nskysize, g_nskysize, -g_nskysize);

	glTexCoord3d(-1.0, -1.0, -1.0); glVertex3f(-g_nskysize, -g_nskysize, -g_nskysize);
	glTexCoord3d(-1.0, -1.0, 1.0); glVertex3f(-g_nskysize, -g_nskysize, g_nskysize);
	glTexCoord3d(-1.0, 1.0, 1.0); glVertex3f(-g_nskysize, g_nskysize, g_nskysize);
	glTexCoord3d(-1.0, 1.0, -1.0); glVertex3f(-g_nskysize, g_nskysize, -g_nskysize);

	glTexCoord3d(1.0, 1.0, 1.0); glVertex3f(g_nskysize, g_nskysize, g_nskysize);
	glTexCoord3d(-1.0, 1.0, 1.0); glVertex3f(-g_nskysize, g_nskysize, g_nskysize);
	glTexCoord3d(-1.0, 1.0, -1.0); glVertex3f(-g_nskysize, g_nskysize, -g_nskysize);
	glTexCoord3d(1.0, 1.0, -1.0); glVertex3f(g_nskysize, g_nskysize, -g_nskysize);

	glTexCoord3d(1.0, -1.0, 1.0); glVertex3f(g_nskysize, -g_nskysize, g_nskysize);
	glTexCoord3d(-1.0, -1.0, 1.0); glVertex3f(-g_nskysize, -g_nskysize, g_nskysize);
	glTexCoord3d(-1.0, -1.0, -1.0); glVertex3f(-g_nskysize, -g_nskysize, -g_nskysize);
	glTexCoord3d(1.0, -1.0, -1.0); glVertex3f(g_nskysize, -g_nskysize, -g_nskysize);

	glTexCoord3d(1.0, -1.0, 1.0); glVertex3f(g_nskysize, -g_nskysize, g_nskysize);
	glTexCoord3d(-1.0, -1.0, 1.0); glVertex3f(-g_nskysize, -g_nskysize, g_nskysize);
	glTexCoord3d(-1.0, 1.0, 1.0); glVertex3f(-g_nskysize, g_nskysize, g_nskysize);
	glTexCoord3d(1.0, 1.0, 1.0); glVertex3f(g_nskysize, g_nskysize, g_nskysize);

	glTexCoord3d(1.0, -1.0, -1.0); glVertex3f(g_nskysize, -g_nskysize, -g_nskysize);
	glTexCoord3d(-1.0, -1.0, -1.0); glVertex3f(-g_nskysize, -g_nskysize, -g_nskysize);
	glTexCoord3d(-1.0, 1.0, -1.0); glVertex3f(-g_nskysize, g_nskysize, -g_nskysize);
	glTexCoord3d(1.0, 1.0, -1.0); glVertex3f(g_nskysize, g_nskysize, -g_nskysize);
	glEnd();
}

