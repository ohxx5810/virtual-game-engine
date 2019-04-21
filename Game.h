#include <GL/glut.h>
#include "RGBpixmap.h"

class Object {
public:
	string name;
	GLfloat x, y, z;
	GLfloat r, g, b;
	GLfloat radius;
	GLfloat slices;
	GLfloat stacks;
} obj[10];

Object sphere0;
Object sphere1;
Object sphere2;
Object sphere3;
Object cone1;
GLint numObj = 0;  // Number of interactive objects to pick up.
Object grabbed;
GLint grabbedcone = 0;  //Needed to DROP the cone.
GLint holdingObject = 0;

/* Light Options */
GLfloat mat_specular[] = {1.0, 1.0, 1.0, 1.0 };
GLfloat mat_shininess[] = { 50.0 };
GLfloat low_shininess[] = { 5.0 }; // Used for specular reflection
GLfloat high_shininess[] = { 100.0 };

GLfloat light_ambient[] = { 0.0, 0.0, 0.0, 0.0 };
GLfloat light1_ambient[] = { 0.0, 0.0, 0.1, 1.0 };
GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat light1_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat light1_specular[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat light_position[] = { -1.0, 0.0, 1.0, 0.0 };
GLfloat light1_position[] = { 0.0, 1.0, 0.0, 0.0 };
GLfloat light2_position[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat spot_direction[] = { -1.0, -1.0, 0.0 };

GLint lightposition = 1;
GLint light1position = 0;
GLint light2position = 0;
GLint lightswitchfound = 0;

GLint lightON = 0;
GLint LIGHT0 = 0; //used for demonstration. Press 3 to turn off.
GLint LIGHT1 = 0;
GLint LIGHT2 = 0;
GLint specular_reflection = 0;
GLint localLight = 0;

RGBpixmap pixtextures[3];
GLint pix[3];

float high_x;
float high_z;
float low_x;
float low_z;

void initializeTexturesAndBlending();
void initializeLights();
void initializeObjects();
void drawFloor();
void drawCeiling();
void drawWall(int, int, int, int, int, int, int, int, int); //x, z, height, color
void drawWalls();	//the blueprint for the walls
void drawDiskWheel(const double);
void drawCylinderWheel();
void drawLightSwitch1();
void drawLightSwitch2();
void drawSphere(Object);
void drawCone(Object, const double);
void printSecretMessage();

void initializeTexturesAndBlending()
{

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);		//Used for secretMessage
	glEnable(GL_LINE_SMOOTH);

	pixtextures[0].readBMPFile("wheel.bmp");
	pixtextures[0].setTexture(2000); pix[0] = 2000;

	pixtextures[1].readBMPFile("concretewall2.bmp");
	pixtextures[1].setTexture(2001); pix[1] = 2001;

	pixtextures[2].readBMPFile("woodfloor2.bmp");
	pixtextures[2].setTexture(2002); pix[2] = 2002;

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
}

void initializeLights()
{
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	glLightfv(GL_LIGHT1, GL_AMBIENT, light1_ambient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light1_diffuse);
	glLightfv(GL_LIGHT1, GL_SPECULAR, light1_specular);
	glLightfv(GL_LIGHT1, GL_POSITION, light1_position);
	glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, 1.5);
	glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, 0.5);
	glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, 0.2);

	glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 45.0);
	glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, spot_direction);
	glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 2.0);

	glClearColor(0.0, 0.0, 0.0, 0.0);
	glShadeModel( GL_SMOOTH );

	glLightfv(GL_LIGHT1, GL_POSITION, light1_position);

	glEnable(GL_LIGHTING); //TODO: TURN ON FOR CLASS!!!!
	lightON = 1;


	//Must enable in game! Read Instructions at the top.
		//glEnable(GL_LIGHT0);
		//LIGHT0 = 1;
}

void initializeObjects()
{
	//Sphere at Origin
	sphere0.x = 0.0; sphere0.y = 8.0; sphere0.z = 0.0;
	sphere0.r = 0.0; sphere0.g = 0.0; sphere0.g = 0.0;
	sphere0.radius = 0.5; sphere0.slices = 60; sphere0.stacks = 16;
	sphere0.name = "sphere0";
	numObj++;

	//Add Interactive Objects
	//Goldenrod
	sphere1.x = 0.0; sphere1.y = 5.0; sphere1.z = 90.0;
	sphere1.r = 0.855, sphere1.g = 0.647, sphere1.b = 0.125;
	sphere1.radius = 2.4; sphere1.slices = 60; sphere1.stacks = 16;
	sphere1.name = "sphere1";
	numObj++;

	//Green
	sphere2.x = 0.0; sphere2.y = 5.0; sphere2.z = 20.0;
	sphere2.r = 0.0, sphere2.g = 0.502, sphere2.b = 0.0;
	sphere2.radius = 1.0; sphere2.slices = 60; sphere2.stacks = 16;
	sphere2 .name = "sphere2";
	numObj++;

	//Crimson
	sphere3.x = 0.0; sphere3.y = 5.0; sphere3.z = 50.0;
	sphere3.r = 0.863, sphere3.g = 0.078, sphere3.b = 0.235;
	sphere3.radius = 1.2; sphere3.slices = 60; sphere3.stacks = 16;
	sphere3 .name = "sphere3";
	numObj++;

	cone1.x = 45.0; cone1.y = 4.0; cone1.z = -15.0;
	cone1.r = 0.00, cone1.g = 0.749, cone1.b = 1.0;
	cone1.radius = 1.0; cone1.slices = 20; cone1.stacks = 20;
	cone1 .name = "cone1";
	numObj++;
}

void drawSphere(Object object) {
	glBindTexture(GL_TEXTURE_2D, 0);
	glEnable(GL_COLOR_MATERIAL);
	glColor3f(object.r, object.g, object.b);

	//glShadeModel(GL_SMOOTH);
	glPushMatrix();
	glTranslatef(object.x, object.y, object.z);
	glutSolidSphere(object.radius, object.slices, object.stacks);
	glPopMatrix();
	glDisable(GL_COLOR_MATERIAL);

	/* Set back to neutral ambient setting!! */
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT_AND_BACK, GL_DIFFUSE);
	glColor4f(1.0, 1.0, 1.0, 1.0);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glDisable(GL_COLOR_MATERIAL);
}
void drawCone(Object object, const double a) {
	glEnable(GL_COLOR_MATERIAL);
	glColor3f(object.r, object.g, object.b);//Goldenrod
	glShadeModel(GL_SMOOTH);

	glPushMatrix();
		glTranslatef(object.x, object.y, object.z);
		glRotated(90, -1, 0, 0);
		if (grabbedcone)
			glRotated(a, 0, 0, 1);
		glutSolidCone(object.radius, object.radius, object.slices, object.stacks);
	glPopMatrix();
	glDisable(GL_COLOR_MATERIAL);

	/* Set back to neutral ambient setting!! */
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT, GL_DIFFUSE);
	glColor4f(1.0, 1.0, 1.0, 1.0);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glDisable(GL_COLOR_MATERIAL);
}

/*
 * Wheel of Fortune Disk
 *
 * */
void drawDiskWheel(const double a)
{
	glEnable(GL_COLOR_MATERIAL);
	glColor3f(1.0, 1.0, 1.0);
	GLUquadricObj *myDisk;
	GLdouble inner = 0.0;
	GLdouble outer = 22.0;
	GLint slices = 12;
	GLint loops = 1;
	myDisk = gluNewQuadric();
	gluQuadricDrawStyle(myDisk, GLU_FILL); //GLU_LINE GLU_FILL GLU_SILHOUETTE GLU_POINT
	gluQuadricNormals(myDisk, GLU_SMOOTH);
	gluQuadricTexture(myDisk, GL_TRUE);

	glBindTexture(GL_TEXTURE_2D, pix[0]);

	glPushMatrix();
		glScalef(-0.5, -0.5, -0.5);
		glTranslated(125.0,-10,150);
		glRotated(110,1,0,0);
		glRotated(a, 0, 0, 1);
		gluDisk(myDisk, inner, outer, slices, loops); //24
	glPopMatrix();
	glDisable(GL_COLOR_MATERIAL);

	glShadeModel(GL_SMOOTH);
}//End of Wheel of Fortune Disk

void drawCylinderWheel()
{
	GLUquadricObj *myCyl;
	GLdouble base = 10.0;
	GLdouble top = 5.0;
	GLdouble height = 4.0;
	GLint slices = 16;
	GLint stacks = 16;
	myCyl = gluNewQuadric();
	gluQuadricDrawStyle(myCyl, GLU_FILL);
	gluQuadricNormals(myCyl, GLU_SMOOTH);
	gluQuadricTexture(myCyl, GL_TRUE);

	glBindTexture(GL_TEXTURE_2D, 0);
	glEnable(GL_COLOR_MATERIAL);
	glColor3f(0.0, 0.0, 0.0);
	glPushMatrix();
		glTranslated(-62.0,5.0,-75.0);
		glRotated(110,1,0,0);
		gluCylinder(myCyl, base, top, height, slices, stacks);
	glPopMatrix();
	glDisable(GL_COLOR_MATERIAL);
}


void drawFloor()
{
	glColor3f(1.0, 1.0, 1.0);
	glBindTexture(GL_TEXTURE_2D,0);
	glEnable(GL_TEXTURE_2D);
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, pix[2]);
	glBegin(GL_QUADS);
		glTexCoord3f(-200.0, 0,-200);
		glVertex3f(-200.0, 0.0, -200.0);
		glTexCoord3f(-250.0, 0.0,200);
		glVertex3f(-200.0, 0.0, 200.0);
		glTexCoord3f(250.0, 0.0,200);
		glVertex3f(200.0, 0.0, 200.0);
		glTexCoord3f(250.0, 0.0,-200);
		glVertex3f(200.0, 0.0, -200.0);
	glEnd();
	glPopMatrix();

}
void drawCeiling() {
	glColor3f(1.0, 1.0, 1.0);
	glBindTexture(GL_TEXTURE_2D,0);
	glEnable(GL_TEXTURE_2D);
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, pix[1]);
	glBegin(GL_QUADS);
		glTexCoord2f(-200.0, 0);
		glVertex3f(-200.0, 70.0, -200.0);
		glTexCoord2f(-200.0, 0.0);
		glVertex3f(-200.0, 70.0, 200.0);
		glTexCoord2f(200.0, 0.0);
		glVertex3f(200.0, 70.0, 200.0);
		glTexCoord2f(200.0, 0.0);
		glVertex3f(200.0, 70.0, -200.0);
	glEnd();
		glPopMatrix();
}

void drawLightSwitch1() {
	glPushMatrix();
	glRotated(90, 1, 0, 0);
	glTranslated(0.0, 50.0, 0.0);
	drawWall(-8.0, -5.0, 0, 0, 0, 1, 1, 4, 2001);
	glPopMatrix();
}

void drawLightSwitch2() {
	glPushMatrix();
	glRotated(90, 1, 0, 0);
	drawWall(-8.0, -5.0, 0, 0, 0, 1, 1, 4, 2001);
	glPopMatrix();
}

void drawWalls()
{
	glColor3f(1.0, 1.0, 1.0);

	//Three Cubes Art
	drawWall(-67, -18, 5, 2, 5, 5, 8, 1, 2001);
	drawWall(-79, -24, 5, 5, 5, 5, 10, 2, 2001);
	drawWall(-90, -15, 5, 5, 5, 5, 15, 3, 2001);

	drawWall(50, -50, 5, 5, 5, 5, 10, 1, 2001);
	drawWall(55, -30, 5, 5, 5, 5, 10, 2, 2001);

	drawWall(67, 12, 5, 5, 5, 5, 10, 3, 2001);
	drawWall(57, 82, 5, 5, 5, 5, 10, 3, 2001);

	//drawWall(46, 35, 5, 5, 5, 5, 10, 2, 2001);
	drawWall(37, -2, 5, 100, 5, 5, 10, 3, 2001);

	/* ProtoType */
	/* DO NOT TOUCH */
	//       z_offset    x offset z_width       x_wth ???   hght color texture)
	//drawWall(0,          150,     1,     30,   0,     1,    20,   0,   2001); //PROTOTYPE (DO NOT TOUCH)

	/* Borders */
	drawWall(-100,-95, 0, 180, 5, 5, 60, 0, 2001); //left side wall J1
	drawWall(100,-95,0,200,5,5, 60, 0, 2001); //right side wall J2
	drawWall(-100, 100, 95, 5, 5, 5, 20, 0, 2001);//nearest wall   J3
	drawWall(10, 100, 85, 5, 5, 5, 20, 0, 2001);//nearest wall   J4
	drawWall(50, -100, 50, 1, 150, 1, 60, 0, 2001); //farthest wall  J5

	//First Quadrant (Wheel of Fortune Room)
	drawWall(-60.0, 20.0, 50, 1, 1, 0, 20, 0, 2001);//C2 Middle-Hallway
	drawWall(-10.0, -98.0, 1, 50, 0, 1, 20, 0, 2001);//C3
	drawWall(-10.0, -30.0, 1, 50, 0, 1, 20, 0, 2001);//D3
	//drawWall(-62.0, 21.0, 1, 20, 0, 1, 20, 0, 2001);//C1

	drawWall(-60.0, 60.0, 1, 40, 0, 1, 20, 0, 2001);//G1 Wall
	drawWall(-10.0, 40.0, 1, 20, 0, 1, 20, 0, 2001);//F4 Wall
	drawWall(-59.0, 38.0, 50, 1, 1, 0, 20, 0, 2001);

	//Light Switch 0
	drawLightSwitch1();
	drawLightSwitch2();

	//Draw Sienna Table for the spinning cone
	drawWall(50.0, -15.0, 1, 2, 5, 5, 4, 5, 2001);

}

void drawWall( int x_offset, int z_offset, int length_x, int length_z,
		int width_x, int width_z, int height, int color, int texture)
{
	/* this function draws a cube centered at
	 * (x_offset, z_offset)
	 * high x_z is right and back
	 * low x_z  is left and front */
	 high_x = (float)x_offset + length_x; //length_x
	 high_z = (float)z_offset + length_z; //length_z
	 low_x = (float)x_offset - width_x; //width_x
	 low_z = (float)z_offset - width_z; //width_z
	 glEnable(GL_COLOR_MATERIAL);
	 glBindTexture(GL_TEXTURE_2D, 0);
	 glColorMaterial(GL_FRONT, GL_DIFFUSE);//CORRECT FOR BACK AMBIENT
	 switch(color)
	 {
	 case 1:
		 glColor3f(1.0, 0.0, 0.0);
		 glColor3f(0.804, 0.361, 0.361);//IndianRed
		 break;
	 case 2:
		 //glColor3f(0.0, 1.0, 0.0);
		 glColor3f(0.133, 0.545, 0.133); //ForestGreen
		 break;
	 case 3:
		 //glColor3f(0.0, 0.0, 1.0);
		 glColor3f(0.255, 0.412, 0.882); //RoyalBlue
		 break;
	 case 4:
		 //glColor3f(0.878, 1.0, 1.0); //Light Cyan
		 glColor3f(0.412, 0.412, 0.412);  //Silver
		 break;
	 case 5:
		 glColor3f(0.627, 0.322, 0.176); //Sienna
		 break;
	 default:
		 glColor3f(1.0, 1.0, 1.0);

	 }

	 switch(texture) {
	 case 2001:
		 glBindTexture(GL_TEXTURE_2D, texture);
		 break;
	 }
	 //glShadeModel(GL_FLAT);
	 glBegin(GL_QUADS);
	 glTexCoord2f(0.0, 0.0); glVertex3f(low_x,height,high_z); /* front */
	 glTexCoord2f(1.0, 0.0); glVertex3f(low_x,0.0,high_z);
	 glTexCoord2f(1.0, 1.0); glVertex3f(high_x,0.0,high_z);
	 glTexCoord2f(0.0, 1.0); glVertex3f(high_x,height,high_z);

	 glTexCoord2f(0.0, 0.0); glVertex3f(high_x,height,low_z); /* back */
	 glTexCoord2f(1.0, 0.0); glVertex3f(high_x,0.0,low_z);
	 glTexCoord2f(1.0, 1.0); glVertex3f(low_x,0.0,low_z);
	 glTexCoord2f(0.0, 1.0); glVertex3f(low_x,height,low_z);

	 glTexCoord2f(0.0, 0.0); glVertex3f(high_x,height,high_z); /* right */
	 glTexCoord2f(1.0, 0.0); glVertex3f(high_x,0.0,high_z);
	 glTexCoord2f(1.0, 1.0); glVertex3f(high_x,0.0,low_z);
	 glTexCoord2f(0.0, 1.0); glVertex3f(high_x,height,low_z);

	 glTexCoord2f(0.0, 0.0); glVertex3f(low_x,height,low_z); /* left */
	 glTexCoord2f(1.0, 0.0); glVertex3f(low_x,0.0,low_z);
	 glTexCoord2f(1.0, 1.0); glVertex3f(low_x,0.0,high_z);
	 glTexCoord2f(0.0, 1.0); glVertex3f(low_x,height,high_z);

	 glTexCoord2f(0.0, 0.0); glVertex3f(low_x,height,high_z); /* top */
	 glTexCoord2f(1.0, 0.0); glVertex3f(high_x,height,high_z);
	 glTexCoord2f(1.0, 1.0); glVertex3f(high_x,height,low_z);
	 glTexCoord2f(0.0, 1.0); glVertex3f(low_x,height,low_z);

	 glTexCoord2f(0.0, 0.0); glVertex3f(low_x,0.0,low_z); /* bottom */
	 glTexCoord2f(1.0, 0.0); glVertex3f(high_x,0.0,low_z);
	 glTexCoord2f(1.0, 1.0); glVertex3f(high_x,0.0,high_z);
	 glTexCoord2f(0.0, 1.0); glVertex3f(low_x,0.0,high_z);
	glEnd();
	glDisable(GL_COLOR_MATERIAL);

	//glEnable(GL_COLOR_MATERIAL);
	//glColorMaterial(GL_FRONT_AND_BACK, GL_DIFFUSE);
	//glColor4f(1.0, 1.0, 1.0, 1.0);
	//glDisable(GL_COLOR_MATERIAL);

}
void output(GLfloat x, GLfloat y, char *text)
{
	char *p;
	glPushMatrix();
	glTranslatef(x, y, 0);
	for (p = text; *p; p++)
		glutStrokeCharacter(GLUT_STROKE_ROMAN, *p);
	glPopMatrix();
}
void printSecretMessage() {
	glPushMatrix();
	glTranslated(100.0, 50.0, -400.0);
	glLineWidth(5.0);
	output(5, 5, "THANKYOUFORYOURTIME!");
	glLineWidth(1.0);
	glPopMatrix();
}
