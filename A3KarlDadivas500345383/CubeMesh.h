#include <stdbool.h>
#include "Vector3D.h"
#include <gl/glut.h>

typedef struct CubeMesh
{
	Vector3D center;
	Vector3D dim;
	
	GLfloat tx, ty, tz;		// Translatation Deltas
	GLfloat sfx, sfy, sfz;	// Scale Factors
	GLfloat angle;			// Angle around y-axis of cube coordinate system
	
	bool selected;

	// Material properties for drawing
	GLfloat mat_ambient[4];
    GLfloat mat_specular[4];
    GLfloat mat_diffuse[4];
	GLfloat mat_shininess[1];

	// Material properties if selected
	GLfloat highlightMat_ambient[4];
    GLfloat highlightMat_specular[4];
    GLfloat highlightMat_diffuse[4];
	GLfloat highlightMat_shininess[1];

} CubeMesh;
