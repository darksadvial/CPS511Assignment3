/*******************************************************************
           Assignment 3
********************************************************************/


/*
Karl Dadivas
500 345 383
Assignment 3 CPS 511

I hereby acknowledge that this is my own original work without the assistance of any other person 

*/

#include <stdio.h>
#include <string.h>
#include <math.h>
#include <gl/glut.h>
#include "Vector3D.h"
#include "QuadMesh.h"


#define PI 3.14159265


const int meshSize = 16;    // Default Mesh Size
const int vWidth = 650;     // Viewport width in pixels
const int vHeight = 500;    // Viewport height in pixels
const float floorHeight = 0.5;
static int currentButton;
static unsigned char currentKey;



// Light properties
static GLfloat light_position0[] = { -6.0F, 12.0F, 0.0F, 1.0F };
static GLfloat light_position1[] = { 6.0F, 12.0F, 0.0F, 1.0F };
static GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
static GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
static GLfloat light_ambient[] = { 0.2F, 0.2F, 0.2F, 1.0F };

// Material properties
static GLfloat drone_mat_ambient[] = { 0.4F, 0.2F, 0.0F, 1.0F };
static GLfloat drone_mat_specular[] = { 0.1F, 0.1F, 0.0F, 1.0F };
static GLfloat drone_mat_diffuse[] = { 0.9F, 0.5F, 0.0F, 1.0F };
static GLfloat drone_mat_shininess[] = { 0.0F };

// A quad mesh representing the ground
static QuadMesh groundMesh;



float xpos, ypos, zpos, p1x, p2x, p3x, p1y, p2y, p3y, p1z, p2z, p3z, dy, dz = 0;
struct DroneCoordinates {
	float x, y, z, p1x, p2x, p3x, p1y, p2y, p3y, p1z, p2z, p3z;
	GLfloat theta;
}DPos;

struct TorpedoCoordinates {
	float x, y, z, fuel;
	GLfloat theta;
}TPos;

struct CameraCoordinates {
	float x, y, z;
	GLfloat theta;
}CPos;

static GLfloat spin = 0.0;
static GLfloat theta = 0.0;



float toRadians = PI/180;

int spinOn = 0;
int droneOn = 0;
int fired = 0;
int cameraOn = 0;


// Prototypes for functions in this module
void initOpenGL(int w, int h);
void display(void);
void reshape(int w, int h);
void mouse(int button, int state, int x, int y);
void mouseMotionHandler(int xMouse, int yMouse);
void keyboard(unsigned char key, int x, int y);
void functionKeys(int key, int x, int y);
Vector3D ScreenToWorld(int x, int y);


// Drawing drone
void drawDrone(void);
void drawPropeller(void);
void destroyDrone(void);

// Autonomous drone
void drawAutoDrone(int droneOn);
void drawAutoPropeller(void);
void moveAuto(droneOn);
void moveAutoForward(void);
void moveAutoTurn(void);
void destoryAutoDrone(void);
// Moving drone
void spinPropeller(int spinOn);
void fireTorpedo(void);
void moveForward(float dz);
void moveUp(float dy);
void moveTurn(float theta);
void cameraDrone(cameraOn);

void helpKey(void);

// Drawing city
void drawBuilding(void);


//Collision detection
void collisionDetection(void);

//draw Torpedo
void drawTorpedo(void);


int main(int argc, char **argv)
{
    // Initialize GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(vWidth, vHeight);
    glutInitWindowPosition(200, 30);
    glutCreateWindow("Assignment 3");

    // Initialize GL
    initOpenGL(vWidth, vHeight);

    // Register callbacks
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMouseFunc(mouse);
    glutMotionFunc(mouseMotionHandler);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(functionKeys);


    // Start event loop, never returns
    glutMainLoop();

    return 0;
}


// Set up OpenGL. For viewport and projection setup see reshape(). */
void initOpenGL(int w, int h)
{
    // Set up and enable lighting
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular);

    glLightfv(GL_LIGHT0, GL_POSITION, light_position0);
    glLightfv(GL_LIGHT1, GL_POSITION, light_position1);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    //glEnable(GL_LIGHT1);   // This light is currently off

    // Other OpenGL setup
    glEnable(GL_DEPTH_TEST);   // Remove hidded surfaces
    glShadeModel(GL_SMOOTH);   // Use smooth shading, makes boundaries between polygons harder to see 
    glClearColor(0.6F, 0.6F, 0.6F, 0.0F);  // Color and depth for glClear
    glClearDepth(1.0f);
    glEnable(GL_NORMALIZE);    // Renormalize normal vectors 
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);   // Nicer perspective

    // Set up ground quad mesh
    Vector3D origin = NewVector3D(-8.0f, 0.0f, 8.0f);
    Vector3D dir1v = NewVector3D(1.0f, 0.0f, 0.0f);
    Vector3D dir2v = NewVector3D(0.0f, 0.0f, -1.0f);
    groundMesh = NewQuadMesh(meshSize);
    InitMeshQM(&groundMesh, meshSize, origin, 50.0, 50.0, dir1v, dir2v);

    Vector3D ambient = NewVector3D(0.0f, 0.05f, 0.0f);
    Vector3D diffuse = NewVector3D(0.4f, 0.8f, 0.4f);
    Vector3D specular = NewVector3D(0.04f, 0.04f, 0.04f);
    SetMaterialQM(&groundMesh, ambient, diffuse, specular, 0.2);

	ypos = 4;
	DPos.y = 4;
	DPos.theta = 0.0;
	DPos.x = 0;
	DPos.z = 0;
	DPos.p1x = 0;
	DPos.p2y = 0;
	DPos.p3z = 0;
	DPos.p1x = 0;
	DPos.p2y = 0;
	DPos.p3z = 0;
	DPos.p1x = 0;
	DPos.p2y = 0;
	DPos.p3z = 0;
	CPos.x = 15;
	CPos.y = 21;
	CPos.z = 15;

	TPos.x = 0;
	TPos.y = 4;
	TPos.z = 0;
	TPos.theta = 0.0;
	TPos.fuel = 100;
    // Set up the bounding box of the scene
    // Currently unused. You could set up bounding boxes for your objects eventually.
 
}


// Callback, called whenever GLUT determines that the window should be redisplayed
// or glutPostRedisplay() has been called.
void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Draw Drone

    // Set drone material properties
    glMaterialfv(GL_FRONT, GL_AMBIENT, drone_mat_ambient);
    glMaterialfv(GL_FRONT, GL_SPECULAR, drone_mat_specular);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, drone_mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SHININESS, drone_mat_shininess);


	drawDrone();
	collisionDetection();
	cameraDrone(cameraOn);
	
	moveAuto(droneOn);
	drawAutoDrone(droneOn);

	fireTorpedo(fired);
	drawTorpedo(fired);
	
	
	spinPropeller(spinOn);
	drawBuilding();

	// Set up the camera at position (0, 6, 22) looking at the origin, up along positive y axis
	


	
	
    // Draw ground mesh
    DrawMeshQM(&groundMesh, meshSize);

    glutSwapBuffers();   // Double buffering, swap buffers
}


// Callback, called at initialization and whenever user resizes the window.
void reshape(int w, int h)
{
    // Set up viewport, projection, then change to modelview matrix mode - 
    // display function will then set up camera and do modeling transforms.
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(80.0, (GLdouble)w / h, 0.2, 40.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
	gluLookAt(CPos.x,CPos.y,CPos.z,CPos.x,CPos.y-15,CPos.z-20, 0.0, 1.0, 0);
	glOrtho(1, 1, 1, 1, 1, 1);
    // Set up the camera at position (0, 6, 22) looking at the origin, up along positive y axis   		
}

// Callback, handles input from the keyboard, non-arrow keys
void keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
	case 's':
		if (spinOn == 0)
		{
			spinOn = 1;
		}
		else
		{
			spinOn = 0;
		};
		break;

	case 'f':
		TPos.x = xpos;
		TPos.y = ypos;
		TPos.z = zpos;
		TPos.theta = theta;
		fired = 1;
		break;

	case 'u':
		dy = 0.1;
		moveUp(dy);
		break;

	case 'd':
		dy = -0.1;
		moveUp(dy);
		break;



// Calculations for new forward vector after rotation

	case 'b':
		dz = -0.1;
		xpos += dz * sin(theta*toRadians);
		zpos += dz * cos(theta*toRadians);
		p1x += dz * sin(theta*toRadians);
		p1z += dz * cos(theta*toRadians);
		p2x += dz * sin(theta*toRadians);
		p2z += dz * cos(theta*toRadians);
		p3x += dz * sin(theta*toRadians);
		p3z += dz * cos(theta*toRadians);
	
		break;

	case 'o':
		if (droneOn == 0) {
			droneOn = 1;
		}
		else { droneOn = 0; }
		
		break;
	


	case 'l':
		theta += 2;
		
		break;

	case 'r':
		theta -= 2;
	
		break;
	
	case 'c':
		if (cameraOn == 1) {
			cameraOn = 0;
		}
		else { cameraOn = 1; }
		

        break;
	
		
    }

    glutPostRedisplay();   // Trigger a window redisplay
}

// Callback, handles input from the keyboard, function and arrow keys
void functionKeys(int key, int x, int y)
{
    // Help key
    if (key == GLUT_KEY_F1)
    {
		helpKey();
    }
    // Do transformations with arrow keys
	else if (key == GLUT_KEY_DOWN) 
	{
		moveForward(-0.2);

		

	}
	else if (key == GLUT_KEY_UP) 
	{
		moveForward(0.2);

	}
	else if (key == GLUT_KEY_RIGHT) 
	{
		moveTurn(-2);
		
	}
	else if (key == GLUT_KEY_LEFT)
	{
		moveTurn(2);

	}
  
	
    glutPostRedisplay();   // Trigger a window redisplay
}


// Mouse button callback - use only if you want to 
void mouse(int button, int state, int x, int y)
{
    currentButton = button;

    switch (button)
    {
    case GLUT_LEFT_BUTTON:
        if (state == GLUT_DOWN)
        {
            ;

        }
        break;
    case GLUT_RIGHT_BUTTON:
        if (state == GLUT_DOWN)
        {
            ;
        }
        break;
    default:
        break;
    }

    glutPostRedisplay();   // Trigger a window redisplay
}


// Mouse motion callback - use only if you want to 
void mouseMotionHandler(int xMouse, int yMouse)
{
    if (currentButton == GLUT_LEFT_BUTTON)
    {
        ;
    }

    glutPostRedisplay();   // Trigger a window redisplay
}


Vector3D ScreenToWorld(int x, int y)
{
    // you will need to finish this if you use the mouse
    return NewVector3D(0, 0, 0);
}

//requirement 1, static building 
void drawBuilding(void)
{
	glPushMatrix();
	glTranslatef(32, 1.5, 0);
	glScalef(1, 3, 1);
	glutSolidCube(1);
	glPopMatrix();
	
	glPushMatrix();
	glTranslatef(24, 3.5, 4);
	glScalef(1, 7, 1);
	glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(37, 4, -7);
	glScalef(1, 8, 1);
	glutSolidCube(1);
	glPopMatrix();

}

void drawTorpedo(int on) {
	
	if (on == 1) {
		glPushMatrix();
		glTranslatef(TPos.x, TPos.y, TPos.z);
		glRotatef(TPos.theta, 0, 1, 0);
		glScalef(0.5, 0.5, 1);
		glutSolidCone(2, 3, 4, 4);
		glPopMatrix();
	}
}

void fireTorpedo(int on) {

	if (on == 1) {
		if (TPos.fuel > 0) {
			TPos.x += 0.1 * sin(TPos.theta*toRadians);
			TPos.z += 0.1 * cos(TPos.theta*toRadians);
			TPos.fuel -= 0.01;
			if ((TPos.x >= DPos.x - 3) && (TPos.z >= DPos.z - 3) && (TPos.x < DPos.x + 3) && (TPos.z <= DPos.z + 3) && (TPos.y <= DPos.y)) {
				fired = 0;
				printf("Destroyed \n");
			}
		}
		else {
			TPos.fuel = 100;
			TPos.x = xpos;
			TPos.y = ypos;
			TPos.z = zpos;
			TPos.theta = theta;
			fired = 0;
		}
	}
}

void drawAutoDrone(int droneOn) 
{
	if (droneOn == 1) {
		glPushMatrix();
		glTranslatef(DPos.x, DPos.y, DPos.z); // M' = M*T1*R1*T2 translate back to local position
		glRotatef(DPos.theta, 0, 1, 0); // M' = M*T1*R1 rotate along origin
		glTranslatef(-DPos.x, -DPos.y, -DPos.z); // M' = M*T1 translate to origin
		glTranslatef(DPos.x, DPos.y, DPos.z); // M = CTM*T*S create object
		glScalef(1, 1, 6);
		glutSolidCube(1);//include coordinates for transformation
		glPopMatrix();
		glPushMatrix();
		glTranslatef(DPos.x, DPos.y, DPos.z); // M' = M*T1*R1*T2 translate back to local position at first object
		glRotatef(DPos.theta, 0, 1, 0); // M' = M*T1*R2 rotated object along origin Y axis
		glTranslatef(-DPos.x, -DPos.y, -DPos.z); // M' = M*T1 translate new object to local(first object) origin
		glTranslatef(DPos.x, DPos.y, (DPos.z + 3.5)); // M' = CTM*T*S another object created with scale
		glScalef(6, 1, 1);
		glutSolidCube(1);
		glPopMatrix();
		drawAutoPropeller();
	}
}

void drawDrone(void) 
{	
	glPushMatrix();
	glTranslatef(xpos, ypos, zpos); // M' = M*T1*R1*T2 translate back to local position
	glRotatef(theta, 0, 1, 0); // M' = M*T1*R1 rotate along origin
	glTranslatef(-xpos, -ypos, -zpos); // M' = M*T1 translate to origin
	glTranslatef(xpos, ypos, zpos); // M = CTM*T*S create object
	glScalef(1, 1, 6);
	glutSolidCube(1);//include coordinates for transformation
	glPopMatrix();
	glPushMatrix();
	glTranslatef(xpos, ypos, zpos); // M' = M*T1*R1*T2 translate back to local position at first object
	glRotatef(theta, 0, 1, 0); // M' = M*T1*R2 rotated object along origin Y axis
	glTranslatef(-xpos, -ypos, -zpos); // M' = M*T1 translate new object to local(first object) origin
	glTranslatef(xpos, ypos, (zpos + 3.5)); // M' = CTM*T*S another object created with scale
	glScalef(6, 1, 1);
	glutSolidCube(1);
	glPopMatrix();
	drawPropeller();
}


void drawAutoPropeller(void) 
{
	glPushMatrix();
	glTranslatef(DPos.x, DPos.y, DPos.z); // M'= M*T1*R1*T2 
	glRotatef(DPos.theta, 0, 1, 0); // M' = M*T1*R1
	glTranslatef(-DPos.x, -DPos.y, -DPos.z); // M' = M*T1
	glTranslatef(DPos.p1x + 2.5, DPos.p1y + 4.6, DPos.p1z + 3.8); // for each propeller translate to local objects origin point, then rotate along that axis, and translate back to local position
	glScalef(0.9, 0.9, 1.8);
	glRotatef(270, 1, 0, 0);
	glutSolidCone(0.5, 0.8, 10, 10);
	glRotatef(spin, 0, 0, 1);
	glScalef(2.5, 0.3, 0.1);
	glutSolidCube(1);
	glRotatef(spin, 0, 0, 1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(DPos.x, DPos.y, DPos.z);
	glRotatef(DPos.theta, 0, 1, 0);
	glTranslatef(-DPos.x, -DPos.y, -DPos.z);
	glTranslatef(DPos.p2x - 2.5, DPos.p2y + 4.6, DPos.p2z + 3.8);
	glScalef(0.9, 0.9, 1.8);
	glRotatef(270, 1, 0, 0);
	glutSolidCone(0.5, 0.8, 10, 10);
	glRotatef(-spin, 0, 0, 1);
	glScalef(2.5, 0.3, 0.1);
	glutSolidCube(1);
	glRotatef(-spin, 0, 0, 1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(DPos.x, DPos.y, DPos.z);
	glRotatef(DPos.theta, 0, 1, 0);
	glTranslatef(-DPos.x, -DPos.y, -DPos.z);
	glTranslatef(DPos.p3x + 0, DPos.p3y + 4.6, DPos.p3z - 2.2); //include p1,p2,p3 coordinates for transformations
	glScalef(0.9, 0.9, 1.8);
	glRotatef(270, 1, 0, 0);
	glutSolidCone(0.5, 0.8, 10, 10);
	glRotatef(spin, 0, 0, 1);
	glScalef(2.5, 0.3, 0.1);
	glutSolidCube(1);
	glRotatef(spin, 0, 0, 1);
	glPopMatrix();
}

//requirement 1, 3 spinning propellers, key 's' to turn on/off spinPropeller method
void drawPropeller(void)
{
	glPushMatrix();
	glTranslatef(xpos, ypos, zpos); // M'= M*T1*R1*T2 
	glRotatef(theta, 0, 1, 0); // M' = M*T1*R1
	glTranslatef(-xpos, -ypos, -zpos); // M' = M*T1
	glTranslatef(p1x + 2.5, p1y + 4.6, p1z + 3.8); // for each propeller translate to local objects origin point, then rotate along that axis, and translate back to local position
	glScalef(0.9, 0.9, 1.8);
	glRotatef(270, 1, 0, 0);
	glutSolidCone(0.5, 0.8, 10, 10);
	glRotatef(spin, 0, 0, 1);
	glScalef(2.5, 0.3, 0.1);
	glutSolidCube(1);
	glRotatef(spin, 0, 0, 1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(xpos, ypos, zpos);
	glRotatef(theta, 0, 1, 0);
	glTranslatef(-xpos, -ypos, -zpos);
	glTranslatef(p2x-2.5, p2y+4.6, p2z+3.8);
	glScalef(0.9, 0.9, 1.8);
	glRotatef(270, 1, 0, 0);
	glutSolidCone(0.5, 0.8, 10, 10);
	glRotatef(-spin,0,0,1);
	glScalef(2.5, 0.3, 0.1);
	glutSolidCube(1);
	glRotatef(-spin, 0, 0, 1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(xpos, ypos, zpos);
	glRotatef(theta, 0, 1, 0);
	glTranslatef(-xpos, -ypos, -zpos);
	glTranslatef(p3x+0, p3y+4.6, p3z-2.2); //include p1,p2,p3 coordinates for transformations
	glScalef(0.9, 0.9, 1.8);
	glRotatef(270, 1, 0, 0);
	glutSolidCone(0.5, 0.8, 10, 10);
	glRotatef(spin,0,0,1);
	glScalef(2.5, 0.3, 0.1);
	glutSolidCube(1);
	glRotatef(spin, 0, 0, 1);
	glPopMatrix();
}

// Apply transformations to move drone
// ...

void spinPropeller(int spinOn) 
{
	if(spinOn==1)
	{
		spin += 0.5;
		if (spin > 360.0)
			spin -= 360.0;
		glutPostRedisplay();
	};
}

void moveForward(float dz) {
	xpos += dz * sin(theta*toRadians);
	zpos += dz * cos(theta*toRadians);
	p1x += dz * sin(theta*toRadians);
	p1z += dz * cos(theta*toRadians);
	p2x += dz * sin(theta*toRadians);
	p2z += dz * cos(theta*toRadians);
	p3x += dz * sin(theta*toRadians);
	p3z += dz * cos(theta*toRadians);
}

void moveUp(float dy) 
{
	ypos += dy;
	p1y += dy;
	p2y += dy;
	p3y += dy;


}

void moveTurn(float turn)
{
	theta += turn;
	CPos.x = xpos * sin(theta*toRadians);
	CPos.z = zpos * cos(theta*toRadians);
}


void moveAuto(int droneOn)
{
	if (droneOn) {

		if (DPos.z > -20) {
			moveAutoForward();
		}
		else {
			if (DPos.theta > 180) {
				moveAutoTurn(1);
			}
			else if (DPos.z > -20) {
				moveAutoForward();
			}
		}

		if (DPos.z < 3) {
			moveAutoForward();
		}
		else {
			if (DPos.theta <= 180) {
				moveAutoTurn(1);
			}
			else if (DPos.theta > 180) {
				moveAutoTurn(1);
			}
			else {
				moveAutoForward();
			}
		}



		glutPostRedisplay();
	}
}
void moveAutoForward(void) 
{
	float dz;
	dz = 0.01;
	DPos.x += dz * sin(DPos.theta*toRadians);
	DPos.z += dz * cos(DPos.theta*toRadians);
	DPos.p1x += dz * sin(DPos.theta*toRadians);
	DPos.p1z += dz * cos(DPos.theta*toRadians);
	DPos.p2x += dz * sin(DPos.theta*toRadians);
	DPos.p2z += dz * cos(DPos.theta*toRadians);
	DPos.p3x += dz * sin(DPos.theta*toRadians);
	DPos.p3z += dz * cos(DPos.theta*toRadians);

}

void moveAutoTurn(int left)
{
	
	float turn;
	if (DPos.theta > 360) {
		DPos.theta -= 360;
	}
	if (left == 0) {
		turn = -0.2;
	}
	else { turn = 0.2; }
	DPos.theta += turn;
	

}

void helpKey(void) 
{
	printf("Assignment 3 \n Controls: \n UP, DOWN forward, backward \n LEFT RIGHT rotate left, right \n u, d raise height, lower height \n \"o\" spawn second drone \n \"f\" fire missile \n \"c\" camera mode note: currently not optimized but is located underside of player drone \n");
}

void destoryDrone(void)
{
	
	ypos = 4;
	xpos = 0;
	zpos = 0;
	p1x = 0;
	p2x = 0;
	p3x = 0;
	p1y = 0;
	p2y = 0;
	p3y = 0;
	p1z= 0;
	p2z = 0;
	p3z = 0;
	theta = 0;
	printf("Destroyed");
	glutPostRedisplay();
}

void destoryAutoDrone(void) {

	DPos.y = 4;
	DPos.x = 0;
	DPos.z = 0;
	DPos.p1x = 0;
	DPos.p2x = 0;
	DPos.p3x = 0;
	DPos.p1y = 0;
	DPos.p2y = 0;
	DPos.p3y = 0;
	DPos.p1z = 0;
	DPos.p2z = 0;
	DPos.p3z = 0;
	DPos.theta = 0;
	
	

	glutPostRedisplay();
}

void collisionDetection(void)
{


	

	if ((xpos >= 27.5) && (zpos >= -2.5) && (xpos < 36.5) && (zpos <= 2.5) && (ypos < 3)){

		destoryDrone();

	}

	if ((xpos >= 20.5) && (zpos >= 0.5) && (xpos < 27.5) && (zpos <= 7.5) && (ypos < 7 )){

		destoryDrone();

	}
	if ((xpos >= 33.5) && (zpos >= -10.5) && (xpos < 40.5) && (zpos <= -5.5) && (ypos < 8)){
	
	
		destoryDrone();

	}
	if (ypos < floorHeight) {
		destoryDrone();
	}
	if ((TPos.x >= DPos.x - 3) && (TPos.z >= DPos.z - 3) && (TPos.x < DPos.x + 3) && (TPos.z <= DPos.z + 3) && (TPos.y <= DPos.y)) {
		destoryAutoDrone();
	}
}

void cameraDrone(int cameraOn) {
	
	if (cameraOn == 1) {
		CPos.x = xpos;
		CPos.y = ypos;
		CPos.z = zpos;


		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

	
		glRotatef(180, 0, 1, 0);
		gluLookAt(xpos, ypos-1, zpos, xpos-0.1, ypos, zpos-0.1, 0.0, 1.0, 0);
		glOrtho(1, 1, 1, 1, 1, 1);

	}
	else {
		CPos.x = 15;
		CPos.y = 21;
		CPos.z = 15;
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		gluLookAt(CPos.x, CPos.y, CPos.z, CPos.x, CPos.y - 20, CPos.z - 20, 0.0, 1.0, 0);
		glOrtho(1, 1, 1, 1, 1, 1);
	}
}
	







