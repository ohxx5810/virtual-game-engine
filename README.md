# virtual-game-engine
A physical game environment created completely from scratch in OpenGL. 

Traverse a 3D map with objects and hidden easter eggs.
Activate certain light switches to experiment with different lighting and positions.
You can look in all directions and even tilt your neck to the left and right, or go on your tippy toes.
Manipulate objects by picking them up and moving them!
Also uses 'normnalization calculations' to adjust the neck back to homeostasis in ALL angles.

The goal of this project was to be able to create an environment in OpenGL, where not only would you be able to look at an object such as a building from the outside, but actually be able to traverse into and around the building and hallways itself.  That is very hard to produce as OpenGL uses certain glRotate and glTranslate functions to manipulate the focal point, but is not intended as a video game with a first-person perspective.

In order to truly understand the complexity in traversing a first-person perspective 3D environment, one must take into consideration creating classes and functions to keep track of orientation based around 360 degrees, and position relative to origin of the environment (in this case pixel-per-square).  OpenGL's translate and rotate functions were methodically calculated into functions using formulas for varying degrees.  

To give an example:

  Just to turn your head side to side.. the code is
  
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

I have even incorporated a system that involves controlling other objects in the environment.
You can not only move the positions of floating spheres, but you can pick up objects and drop them.
You can drop them on tables, or drop them on the floor, the program tracks position of objects relative to other objects such as tables!

There is also another interesting and important feature to consider.  To traverse in the map I have made it so that the directional of the arrows keys will change every 90 degrees.  This was something very interesting to create and tinker with, as it made sense and made traversing the map suprisingly enjoyable.  Upon random testing, I came across a way to traverse the map with consideration to every degree of orientation.  The function is called translateForward() and so far it is capped at every 15 degrees which I found to be sufficient enough.  

  This is the code for this function, just to show how I manipulated numbers with pixel positionings.
  
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

I accomplished all of this in about a weeks time, and the first thing I learned was how to make squares and circles just like everybody else!
