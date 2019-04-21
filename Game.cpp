/*
 * Traversing a 3D Map.
 *
 * Can also go on Tippy Toes, Look Up/Down, Tilt Neck, etc.
 * Can also Pick Up OBJECTS!!!
 *
 * Also uses normalized calculations to adjust back to homeostasis in ALL angles.
 *
 * TO DEMONSTRATE:
 *
 *
 * MOVES
 * 		Traverse - w, s, a, d
 * 		Rotate Left/Right - n/m
 * 		Look Up/Down - i/j   (move sphere up or down when holding it)
 * 		Tilt Left/Right - k/l
 * 		Go on TippyToes - u/h
 * 		Go back to starting position - Shift + b
 * 		Grab/Drop Objects or Turn ON/OFF Light Switches - 'e'
 * 		Move spheres up and down - i/j (when holding an object)
 * 		Keep blue cone in the air for viewing - Shift + e
 *
 *
 * 	1. Find first light switch (hint: it is to the left of the red ball)
 *	2. The last light switch is to the left behind the green ball.
 * 	3. Interact with Objects
 * 	4. Try placing the blue cone on the table or the floor!
 * 	5. Walking brings your head back to level from Tilt and Look Up/Down functions.
 *
 * DEBUGGING
 * 		Enable/Disable GL_LIGHTING - Toggle '1'
 * 		Change specular reflection from low, medium, high - Toggle '2'
 * 		Change light position from z-axis and y-axis - Toggle '3'
 * 		Traverse Up/Down the y-axis - Shift + {/:
 * 		Unlock Virtual Grid - Shift + C, Shift + O
 *
 *
 * Start demonstration.
 */
#include <GL/glut.h>
#include "Game.h"

void init(void);
void display(void);
void WinReshapeFcn(int, int);
void key(unsigned char, int, int);

void drawWorld1();
void drawWorld2();
void drawVirtualGrid();

/* Interactive Object */
void pickupObject();
void turnOnLightSwitch();

/* Movement for Camera/Person */
void changeOrientation(int);
void lookLeft(); void lookRight();
void lookUpDown(GLint);
void TiltNeck(GLint);
void NormalizeNeck();
void NormalizeLook();

GLsizei winWidth = 1200, winHeight = 800;

/* Initial starting position. */
GLfloat init_posx = 0.0;
GLfloat init_posy = -5.0;
GLfloat init_posz = -105.0;
GLfloat current_posx = 0.0;
GLfloat current_posy = -5.0;  //if changed, also change height_position
GLfloat current_posz = -105.0; //Position of Gamer.


/* Unit per walking distance */
GLfloat movement = 1.0;
/* Coordinates to keep track of degree camera is looking at */
GLint orientation = 0;
/* Unit per head movement (DO NOT CHANGE)*/
GLfloat rotateMovement = 5.0;
/* Used to keep track of position from ground level. (LEAVE SAME AS init_posy) */
GLfloat height_position = -5.0;
/* Used to keep track of eye position from ground level.*/
GLfloat look_position = 0.0;
/* Used to keep track of neck position */
GLfloat neck_position = 0.0;
/* Coordinates used to calculate back to starting position. */
GLint start_posx, start_posy, start_posz;

/* Used for translateForward() function to cover micro-traversing. */
GLint trans_orientation;
GLint d_x, d_z; //for translateForward()


/* KeyCode to Unlock VirtualGrid */
GLint MapKey = 0;
GLint VirtualGrid = 0;

/* ----------  REAL-WORLD INTERACTIVE OBJECTS ----------*/
void init(void)
{
	initializeLights();
	initializeTexturesAndBlending();

	/* Interactive objects */
	initializeObjects();
	glMatrixMode(GL_MODELVIEW);
	glEnable(GL_LIGHT0);
}

/* --------------------  PICKING UP OBJECTS -------------------- */
void pickupObject() {
	if (holdingObject) {
		if (grabbedcone) {
			if (cone1.z <= -13 && cone1.z >= -20 &&cone1.x <= 51 && cone1.x >= 45)
				//Drop the cone on the table.
				grabbedcone = 0;
			else {
				//Drop the cone on the floor.
				cone1.y = 1.0;
				grabbedcone = 0;
			}
		}
		//Dropped the object.
		holdingObject = 0;
	}
	//PICK UP Cone1 SkyBlue
	else if (current_posx >= -cone1.x - 10 && current_posx <= -cone1.x + 10
			&& current_posz <= -cone1.z + 10 && current_posz >= -cone1.z - 10) {
		//HOLD the cone.
		cone1.y = 4.0;
		grabbed = cone1;
		grabbedcone = 1;
		holdingObject = 1;
	}
	else if (current_posx >= -sphere3.x - 10 && current_posx <= -sphere3.x + 10
			&& current_posz <= -sphere3.z + 10
			&& current_posz >= -sphere3.z - 10) {
		//PICK UP SPHERE3 Crimson
		grabbed = sphere3;
		holdingObject = 1;
	}
	//PICK UP SPHERE2 Green
	else if (current_posx >= -sphere2.x - 10 && current_posx <= -sphere2.x + 10
			&& current_posz <= -sphere2.z + 10
			&& current_posz >= -sphere2.z - 10) {
		grabbed = sphere2;
		holdingObject = 1;
	}
	//PICK UP SPHERE1 Sun
	else if (current_posx >= -sphere1.x - 10 && current_posx <= -sphere1.x + 10
			&& current_posz <= -sphere1.z + 10 && current_posz >= -sphere1.z - 10) {
		grabbed = sphere1;
		holdingObject = 1;
	}
	//PICK UP SPHERE0 Black
	else if (current_posx >= -sphere0.x - 10 && current_posx <= -sphere0.x + 10
			&& current_posz <= -sphere0.z + 10
			&& current_posz >= -sphere0.z - 10) {
		grabbed = sphere0;
		holdingObject = 1;
	}
}

/* --------------------  LIGHT SWITCHES -------------------- */
void turnOnLightSwitch() {
	//Turn LIGHT0 ON/OFF
	if (orientation >= 25 && orientation <= 130 && current_posx >= -5.0
			&& current_posx <= 5.0 && current_posy == -5.0
			&& current_posz <= -100.0 && current_posz >= -110.00) {
		if (LIGHT0) {
			glDisable(GL_LIGHT0);
			LIGHT0 = 0;
		} else {
			glEnable(GL_LIGHT0);
			LIGHT0 = 1;
		}
		lightswitchfound = 1;
	}
	//Turn LIGHT1 ON/OFF
	else if (orientation >= 210 && orientation <= 330 && current_posx >= -15.0
			&& current_posx <= 15.0 && current_posy == -5.0
			&& current_posz >= -55.0 && current_posz <= -45.00) {

		if (LIGHT1) {
			glDisable(GL_LIGHT1);
			LIGHT1 = 0;
		} else {
			glEnable(GL_LIGHT1);
			LIGHT1 = 1;
		}
		lightswitchfound = 1;
	}
	//Turn LIGHT0 ON/OFF with different light_position
	else if (orientation >= 210 && orientation <= 330 && current_posx >= -15.0
			&& current_posx <= 15.0 && current_posy == -5.0
			&& current_posz >= -10.0 && current_posz <= 0.00) {
		if (LIGHT0) {
			glLightfv(GL_LIGHT0, GL_POSITION, light2_position);
		} else {
			glLightfv(GL_LIGHT0, GL_POSITION, light2_position);
			lightposition = 0;
			light2position = 1;
			glEnable(GL_LIGHT0);
			LIGHT0 = 1;
			printf("TURN ON LIGHT2\n");
		}
		lightswitchfound = 1;
	}
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glColor3f(1.0, 1.0, 1.0);

    const double t = glutGet(GLUT_ELAPSED_TIME) / 1000.0;
    const double a = t*90.0;

    //glMatrixMode(GL_MODELVIEW);
    	//glLoadIdentity();
    	//glPushMatrix();

	/* location at ORIGIN (Used as a marker for correlation to map.) */
	drawSphere(sphere0);
	drawSphere(sphere1);	//FIRST OBJECT TO DEMONSTRATE
	drawSphere(sphere2);
	drawSphere(sphere3);

	if (!VirtualGrid) {  //UNCOMMENT AND SHIFT+C, SHIFT+O
		drawWorld1();
		drawCone(cone1, a);
		drawDiskWheel(a - 0.5);
	} else {
		drawWorld2();	//NOTHING ADDED TO IT YET!
	}
	drawVirtualGrid();

	glPopMatrix();
	glFlush();
	glutSwapBuffers();

}

void drawHouse() {
	drawWalls();
	drawCylinderWheel();
}
void drawWorld1() {
	drawFloor();
	drawCeiling();
	drawHouse();

	printSecretMessage();
}

void drawWorld2() {
	//TODO: Make another world!
	//drawFloor(); //Make this cement texture
	//drawCeiling();//Make this sky texture or light blue and Bigger
}

void drawVirtualGrid() {
	glLineWidth(1.0);
	//glColor3f(211.0, 211.0, 211.0);
	glPushMatrix();
	glRotatef(90, 1.0, 0.0, 0.0);
	glutWireTorus(50, 350, 10, 20);
	glPopMatrix();

	// Distance Graphics Arena
	glPushMatrix();
	glRotatef(90, 1.0, 0.0, 0.0);
	glRotatef(45, 0.0, 1.0, 0.0);
	glTranslatef(0.0, 200.0, 0.0);
	glutWireTorus(50, 500, 10, 20);
	glPopMatrix();
	glPushMatrix();
	glRotatef(90, 1.0, 0.0, 0.0);
	glRotatef(45, 0.0, -1.0, 0.0);
	glTranslatef(0.0, 200.0, 0.0);
	glutWireTorus(50, 500, 10, 20);
	glPopMatrix();
}

/* -------------------- Key Moves for Traversing -------------------- */

/* *Test Function* for traversing at every degree
 *
 * Heavy Computation.
 * (w.r.t. orientation)
 * */

void translateForward() {
	//TODO: Add Traversing for every degree!!
	if (orientation >= 270 && orientation < 360) {	//First Quadrant.
		d_x = 1;
		d_z = 1;
		trans_orientation = abs(orientation - 360);
	} else if (orientation >= 180) {				//Third Quadrant.
		d_x = 1;
		d_z = -1;
		trans_orientation = abs(orientation - 180);

	} else if (orientation > 90) {					//Fourth Quadrant.
		d_x = -1;
		d_z = -1;
		trans_orientation = abs(orientation - 180 );
	}
	else {											//default values for 0 - 90 degrees. (Second Quadrant.)
		trans_orientation = orientation;
		d_x = -1;
		d_z = 1;
	}
	if (trans_orientation >= 90) {
		glTranslatef(1.0 * d_x, 0.0, 0.0);
		current_posx += 1.0 * d_x;
	} else if (trans_orientation >= 75) {
		glTranslatef(0.0, 0.0, 0.25 * d_z);
		glTranslatef(1.0 * d_x, 0.0, 0.0);
		current_posz += 0.25 * d_z;
		current_posx += 1.0 * d_x;
	} else if (trans_orientation >= 60) {
		glTranslatef(0.0, 0.0, 0.25 * d_z);
		glTranslatef(0.50 * d_x, 0.0, 0.0);
		current_posz += 0.25 * d_z;
		current_posx += 0.50 * d_x;
	} else if (trans_orientation >= 45) {
		glTranslatef(0.0, 0.0, 0.5 * d_z);
		glTranslatef(0.5 * d_x, 0.0, 0.0);
		current_posz += 0.5 * d_z;
		current_posx += 0.5 * d_x;
	} else if (trans_orientation >= 30) {
		glTranslatef(0.0, 0.0, 0.5 * d_z);
		glTranslatef(0.25 * d_x, 0.0, 0.0);
		current_posz += 0.5 * d_z;
		current_posx += 0.25 * d_x;
	} else if (trans_orientation >= 15) {
		glTranslatef(0.0, 0.0, 1 * d_z);
		glTranslatef(0.25 * d_x, 0.0, 0.0);
		current_posz += 1 * d_z;
		current_posx += 0.25 * d_x;
	} else {
		glTranslatef(0.0, 0.0, 1.0 * d_z);
		current_posz += 1.0 * d_z;
	}


}

/* Key moves for Traversing Forward/Backward
 *
 * To Traverse a map, must keep track of orientation in degrees
 *	to traverse properly in World coordinate.
 */
void translateWS90(GLint direction) {
	if (orientation >= 270) {
		glTranslatef(movement * direction, 0.0, 0.0);
		current_posx += movement * direction;
		if (holdingObject) {
			if (grabbed.name == "sphere0")
				sphere0.x -= movement * direction;
			else if (grabbed.name == "sphere1")
				sphere1.x -= movement * direction;
			else if (grabbed.name == "sphere2")
				sphere2.x -= movement * direction;
			else if (grabbed.name == "sphere3")
				sphere3.x -= movement * direction;
			else if (grabbed.name == "cone1")
				cone1.x -= movement * direction;
		}

	}
	else if (orientation >= 180) {
		glTranslatef(0.0, 0.0, -movement * direction);
		current_posz -= movement * direction;
		if (holdingObject) {
			if (grabbed.name == "sphere0")
				sphere0.z += movement * direction;
			else if (grabbed.name == "sphere1")
				sphere1.z += movement * direction;
			else if (grabbed.name == "sphere2")
				sphere2.z += movement * direction;
			else if (grabbed.name == "sphere3")
				sphere3.z += movement * direction;
			else if (grabbed.name == "cone1")
				cone1.z += movement * direction;
		}
	} else if (orientation >= 90) {
		glTranslatef(-movement * direction, 0.0, 0.0);
		current_posx -= movement * direction;
		if (holdingObject) {
			if (grabbed.name == "sphere0")
				sphere0.x += movement * direction;
			else if (grabbed.name == "sphere1")
				sphere1.x += movement * direction;
			else if (grabbed.name == "sphere2")
				sphere2.x += movement * direction;
			else if (grabbed.name == "sphere3")
				sphere3.x += movement * direction;
			else if (grabbed.name == "cone1")
				cone1.x += movement * direction;
		}
	}
	else {
		glTranslatef(0.0, 0.0, movement * direction);
		current_posz += movement * direction;
		if (holdingObject) {
			if (grabbed.name == "sphere0") {
				sphere0.z -= movement * direction;
			} else if (grabbed.name == "sphere1") {
				sphere1.z -= movement * direction;
			} else if (grabbed.name == "sphere2") {
				sphere2.z -= movement * direction;
			} else if (grabbed.name == "sphere3") {
				sphere3.z -= movement * direction;
			} else if (grabbed.name == "cone1")
				cone1.z -= movement * direction;
		}
	}

}
/* Key moves for Traversing Left/Right*/
void translateAD90(GLint direction) {
	if (orientation >= 270) {
			glTranslatef(0.0, 0.0, -movement * direction);
			current_posz -= movement * direction;
			if (holdingObject) {
				if (grabbed.name == "sphere0")
					sphere0.z += movement * direction;
				else if (grabbed.name == "sphere1")
					sphere1.z += movement * direction;
				else if (grabbed.name == "sphere2")
					sphere2.z += movement * direction;
				else if (grabbed.name == "sphere3")
					sphere3.z += movement * direction;
				else if (grabbed.name == "cone1")
					cone1.z += movement * direction;
			}

		}
	else if (orientation >= 180) {
			glTranslatef(-movement * direction, 0.0, 0.0);
			current_posx -= movement * direction;
			if (holdingObject) {
				if (grabbed.name == "sphere0")
					sphere0.x += movement * direction;
				else if (grabbed.name == "sphere1")
					sphere1.x += movement * direction;
				else if (grabbed.name == "sphere2")
					sphere2.x += movement * direction;
				else if (grabbed.name == "sphere3")
					sphere3.x += movement * direction;
				else if (grabbed.name == "cone1")
					cone1.x += movement * direction;
			}

		}
	else if (orientation >= 90) {
		glTranslatef(0.0, 0.0, movement * direction);
		current_posz += movement * direction;
		if (holdingObject) {
			if (grabbed.name == "sphere0")
				sphere0.z -= movement * direction;
			else if (grabbed.name == "sphere1")
				sphere1.z -= movement * direction;
			else if (grabbed.name == "sphere2")
				sphere2.z -= movement * direction;
			else if (grabbed.name == "sphere3")
				sphere3.z -= movement * direction;
			else if (grabbed.name == "cone1")
				cone1.z -= movement * direction;
		}
	}
	else {
		glTranslatef(movement * direction, 0.0, 0.0);
		current_posx += movement * direction;
		if (holdingObject) {
			if (grabbed.name == "sphere0")
				sphere0.x -= movement * direction;
			else if (grabbed.name == "sphere1")
				sphere1.x -= movement * direction;
			else if (grabbed.name == "sphere2")
				sphere2.x -= movement * direction;
			else if (grabbed.name == "sphere3")
				sphere3.x -= movement * direction;
			else if (grabbed.name == "cone1")
				cone1.x -= movement * direction;
		}
	}
}

/* Controls movements */
void key(unsigned char key, int x, int y) {
	switch (key) {
	/* -------------------- TRAVERSE MAP -------------------- */
	case'A':
		movement = 5.0;
		/* no break */
	case 'a':
		NormalizeNeck();
		NormalizeLook();
		translateAD90(1);
		break;
	case'D':
		movement = 5.0;
		/* no break */
	case 'd':
		NormalizeNeck();
		NormalizeLook();
		translateAD90(-1);
		break;
	case'W':
		movement = 5.0;
		/* no break */
	case 'w':
		NormalizeNeck();
		NormalizeLook();
		translateForward();	//Cannot use Objects!
		//translateWS90(1); //Forward and Backward Movements (w.r.t. orientation)
		break;
	case'S':
		movement = 5.0;
		/* no break */
	case 's':
		NormalizeNeck();
		NormalizeLook();
		translateWS90(-1); //Left and Right Movements (w.r.t. orientation)
		break;

	/* ---------- Rotate Camera ---------- */
	case'N':
		rotateMovement = 1.0; //Used to slow down for fine-movement.
		/* no break */
	case 'n':
		lookLeft();
		break;
	case 'M':
		rotateMovement = 1.0;
		/* no break */
	case 'm':
		lookRight();
		break;
	case 'i':
		lookUpDown(-1);
		break;
	case 'j':
		lookUpDown(1);
		break;
	case 'k':
		TiltNeck(-1);
		break;
	case 'l':
		TiltNeck(1);
		break;

	/* ---------- TIPPY TOES ---------- */
	case 'u':
		if (current_posy > height_position - 3.0) {
			glTranslatef(0.0, -movement, 0.0);
			current_posy -= movement;
		}
		break;
	case 'h':
		if (current_posy < height_position) {
			glTranslatef(0.0, movement, 0.0);
			current_posy += movement;
		}
		break;

/* --------------- LIGHTING --------------- */
	case '1':
		if (lightON) {
			glDisable(GL_LIGHTING);
			lightON = 0;
		} else {
			glEnable(GL_LIGHTING);
			lightON = 1;
		}
		break;
	case '2':
		if (specular_reflection == 0) {
			glMaterialfv(GL_FRONT, GL_SHININESS, low_shininess);
			specular_reflection = 1;
		} else if (specular_reflection == 1) {
			glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
			specular_reflection = 2;
		} else {
			glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);
			specular_reflection = 0;
		}
		break;
	case '3':
		if (lightposition) {
			glLightfv(GL_LIGHT0, GL_POSITION, light1_position);
			lightposition = 0;
			light1position = 1;
		}
		else {
			glLightfv(GL_LIGHT0, GL_POSITION, light_position);
			lightposition = 1;
			light1position = 0;
		}
		break;
	case '4':
	    if (localLight) {
	    	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_FALSE);
	    	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);
	    	localLight = 0;
	    } else {
	    	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
			glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
			localLight = 1;
	    }
	    break;

/* --------------- PICKING UP OBJECTS --------------- */
	case 'E':
		if (grabbedcone)
			holdingObject = 0;
		break;
	case 'e':
		turnOnLightSwitch();
		if (!lightswitchfound)
			pickupObject();
		else
			lightswitchfound = 0; //reset
		break;


/* --------------- DEBUGGING --------------- */
	/* Go back to Beginning */
	case 'B':
		start_posx = init_posx - current_posx;
		start_posy = init_posy - current_posy;
		start_posz = init_posz - current_posz;
		glTranslatef(start_posx, start_posy, start_posz);
		//reinitialize the start positions as current positions.
		current_posx += start_posx;
		current_posy += start_posy;
		current_posz += start_posz;
		break;
	case 'p':
		printf("orientation = %d\n", orientation);
		printf("look_position = %f degrees\n", look_position);
		printf("neck_position = %f degrees\n", neck_position);
		printf("position = %f, %f, %f\n", current_posx, current_posy, current_posz);
		break;
	case 'C':
		if (MapKey)
			MapKey = 0;
		else
			MapKey = 1;
		break;
	case 'O':
		if (MapKey) {
			if (VirtualGrid)
				VirtualGrid = 0;
			else
				VirtualGrid = 1;

			MapKey = 0;
		}
		break;
	case '{':
		glTranslatef(0.0, -5.0, 0.0);
		current_posy -= 5.0;
		break;
	case ':':
		if (current_posy < -5.0) {
			glTranslatef(0.0, 5.0, 0.0);
			current_posy += 5.0;
		}
		break;
	case '?':
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		break;
	case VK_ESCAPE:
			exit(0);
	}
	movement = 1;
	rotateMovement = 5.0;
	display();
}

/* --------------- Tools --------------- */

/*	Used to keep track of orientation in degrees.
 *
 *	Keep track of current 3D coordinates for rotate functions,
 *	every rotate must first Traverse to Origin,
 *	Rotate, then Traverse back.
 */
void changeOrientation(int mvmt) {
	orientation += mvmt;
	if (orientation >= 360)
		orientation -= 360;
	else if (orientation < 0)
		orientation += 360;
}
/*	Used to Rotate Camera-View Left and Right.
 *
 * Keep track of current 3D coordinates for rotate functions,
 *	every rotate must first Traverse to Origin,
 *	Rotate, then Traverse back.
 */
void lookLeft() {
	glTranslatef(-current_posx, -current_posy, -current_posz);
	glRotated(rotateMovement, 0, -1, 0);
	changeOrientation(-rotateMovement);
	glTranslatef(current_posx, current_posy, current_posz);
}
void lookRight() {
	glTranslatef(-current_posx, -current_posy, -current_posz);
	glRotated(rotateMovement, 0, 1, 0);
	changeOrientation(rotateMovement);
	glTranslatef(current_posx, current_posy, current_posz);
}

/* Used to Angle Camera-View Up or Down.
 *
 * Look Up, Look Down keeps track of everything above
 * to rotate on the x,y,z coordinates accordingly no matter
 * where you are facing.
 */
void lookUpDown(GLint lookAngle) {
	if (lookAngle < 0) { // If looking Up
		if (holdingObject) {
			if (grabbed.name == "sphere0")
				sphere0.y -= lookAngle;
			else if (grabbed.name == "sphere1")
				sphere1.y -= lookAngle;
			else if (grabbed.name == "sphere2")
				sphere2.y -= lookAngle;
			else if (grabbed.name == "sphere3")
				sphere3.y -= lookAngle;
		} else if (look_position < 90) {
			glTranslatef(-current_posx, -current_posy, -current_posz);
			glRotated(-orientation, 0, 1, 0);
			glRotated(1, lookAngle, 0, 0);
			glRotated(orientation, 0, 1, 0);
			glTranslatef(current_posx, current_posy, current_posz);
			look_position += 1;
		}
	}
	else { // If looking Down
		if (holdingObject) {
			if (grabbed.y >= 1.0) {
				if (grabbed.name == "sphere0") {
					sphere0.y -= lookAngle;
					grabbed.y -= lookAngle;
				} else if (grabbed.name == "sphere1") {
					sphere1.y -= lookAngle;
					grabbed.y -= lookAngle;
				} else if (grabbed.name == "sphere2") {
					sphere2.y -= lookAngle;
					grabbed.y -= lookAngle;
				} else if (grabbed.name == "sphere3") {
					sphere3.y -= lookAngle;
					grabbed.y -= lookAngle;
				}
			}
		} else if (look_position > -40) {
			glTranslatef(-current_posx, -current_posy, -current_posz);
			glRotated(-orientation, 0, 1, 0);
			glRotated(1, lookAngle, 0, 0);
			glRotated(orientation, 0, 1, 0);
			glTranslatef(current_posx, current_posy, current_posz);
			look_position -= 1;

		}
	}

}
/* Used to Tilt Camera-View Left and Right.
 *
 * Tilting your head and going on your tippy toes for higher
 *  angling views takes into account current height OR Y-axis
 *	and neck positions with set boundaries so you don't sprain
 *	your neck.
 */
void TiltNeck(GLint neckAngle) {
	if (neckAngle < 0) { // If Tilt Left
		if (neck_position > -35) {
			glTranslatef(-current_posx, -current_posy, -current_posz);
			glRotated(orientation, 0, -1, 0);
			glRotated(1, 0, 0, neckAngle);
			glRotated(orientation, 0, 1, 0);
			glTranslatef(current_posx, current_posy, current_posz);
			neck_position -= 1;
		}
	}
	else { // If Tilt Right
		if (neck_position < 35) {
			glTranslatef(-current_posx, -current_posy, -current_posz);
			glRotated(orientation, 0, -1, 0);
			glRotated(1, 0, 0, neckAngle);
			glRotated(orientation, 0, 1, 0);
			glTranslatef(current_posx, current_posy, current_posz);
			neck_position += 1;
		}
	}
}

/*
 * Used to Normalize as you Walk.
 *
 *  *** USER-TIMED ANIMATION !!!***
 *	Walking automatically balances your head back to horizontal.
 *	Like a recursive user-function This is done gradually (incrementally)
 *	as you take each step.
 */
void NormalizeNeck() {
	if (neck_position < 0)
		TiltNeck(1);
	else if (neck_position > 0)
		TiltNeck(-1);
}
/*
 * Used to Normalize NeckAngle as you Walk. *** USER-TIMED ANIMATION !!!***
 */
void NormalizeLook() {
	if (!holdingObject) {
		if (current_posy >= -8.0) {
			if (look_position < 0)
				lookUpDown(-1);
			else if (look_position > 0)
				lookUpDown(1);
		}
	}
}

void WinReshapeFcn(int newWidth, int newHeight)
{
	//GLfloat light_position[] = {0.0, 0.0, 0.0, 1.0 };  //Turn these on for Light Source That Moves with the Viewpoint
	if (newHeight == 0) newHeight = 1;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(40.0, (GLfloat)newWidth / (GLfloat)newHeight, 1.0, 600.0);
	glTranslatef(init_posx, init_posy, init_posz);	//set initial position in the VirtualGrid.
	glMatrixMode(GL_MODELVIEW);
	//glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	winWidth = newWidth;
	winHeight = newHeight;
}

static void
idle(void)
{
    glutPostRedisplay();
}
/****************************************************************/
int main (int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(winWidth, winHeight);
	glutInitWindowPosition(40, 40);
	glutCreateWindow("A Game Engine starting from scratch.");
	init();
	glutDisplayFunc(display);
	glutKeyboardFunc(key);
	glutIdleFunc(idle);

	glutReshapeFunc(WinReshapeFcn);

	glutMainLoop();
	return 0;
}
