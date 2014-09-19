/*
 * Draw_triangles.c
 */

#include <stdlib.h>
#include <stdio.h>
#include <GL/glut.h>	// OpenGL Graphics Utility Library
//#include "SimpleDraw.h"

// These variables control the current mode
int CurrentMode = 0;
const int NumModes = 4;

// These variables set the dimensions of the rectanglar region we wish to view.
const double Xmin = -5.0, Xmax = 5.0;
const double Ymin = -5.0, Ymax = 5.0;

// glutKeyboardFunc is called below to set this function to handle
//		all "normal" ascii key presses.
// Only space bar and escape key have an effect.
void myKeyboardFunc( unsigned char key, int x, int y )
{
	switch ( key ) {

	case ' ':									// Space bar
		// Increment the current mode, and tell operating system screen needs redrawing
		CurrentMode = (CurrentMode+1)%NumModes;
		glutPostRedisplay();
		break;

	case 27:									// "27" is theEscape key
		exit(1);

	}
}


/*
 * drawScene() handles the animation and the redrawing of the
 *		graphics window contents.
 */

void drawTriangle(void)
{
		glBegin( GL_POINTS );
		glVertex3f( 1.0, 0.0, 0.0 );
		glVertex3f( 0.0, 2.0, 0.0 );
		glVertex3f( -1.0, 0.0, 0.0 );
		glEnd();
		glBegin( GL_LINE_LOOP );
		glVertex3f( 1.0, 0.0, 0.0 );
		glVertex3f( 0.0, 2.0, 0.0 );
		glVertex3f( -1.0, 0.0, 0.0 );
		glEnd();
}

void drawAxis(void)
{
    glBegin( GL_LINES );
	glColor3f( 0.7, 0.7, 0.7 );		
		glVertex2f( -3.0, 0.0 );
		glVertex2f( 3.0, 0.0 );
		glVertex2f( 0.0, -3.0 );
		glVertex2f( 0.0, 3.0 );
    glEnd();
}


void drawScene(void)
{
	// Clear the rendering window

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Set drawing color to white
	glColor3f( 1.0, 1.0, 1.0 );		

    switch (CurrentMode)
	{

    case 0:
		// step 0
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	drawAxis();
    break;
	
	case 1:
		// step 1
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glColor3f( 0, 1, 0 );		
	drawTriangle();
	drawAxis();
    break;
	
    case 2:
		// step 2
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glRotatef(45,0,0,1);
	glTranslatef(2,0,0);
    glPushMatrix();
	glTranslatef(0,-2,0);
	glColor3f( 1, 0, 0 );		
	drawTriangle();
	glLoadIdentity();
	drawAxis();
    break;

    case 3:
		// step 3
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glRotatef(45,0,0,1);
	glTranslatef(2,0,0);
	glColor3f( 0, 1, 0 );		
	glPushMatrix();
	glTranslatef(0,-2,0);
	glColor3f( 1, 0, 0 );		
	drawTriangle();
	glPopMatrix();
	glRotatef(180,0,0,1);
	glTranslatef(0,-2,0);	
	glColor3f( 0, 0, 1 );		
	drawTriangle();
	glLoadIdentity();
	drawAxis();
    case 4:
		// step 4
    break;
    default:    
    break;
	}

	// Flush the pipeline.  (Not usually necessary.)
	glFlush();

}

// Initialize OpenGL's rendering modes
void initRendering()
{

	glEnable ( GL_DEPTH_TEST );

	// Uncomment out the first block of code below, and then the second block,
	//		to see how they affect line and point drawing.

	// The following commands should cause points and line to be drawn larger
	//	than a single pixel width.
	glPointSize(8);
	glLineWidth(5);



	// The following commands should induce OpenGL to create round points and 
	//	antialias points and lines.  (This is implementation dependent unfortunately).
	glEnable(GL_POINT_SMOOTH);
	glEnable(GL_LINE_SMOOTH);
	glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);	// Make round points, not square points
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);		// Antialias the lines
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

}

// Called when the window is resized
//		w, h - width and height of the window in pixels.
void resizeWindow(int w, int h)
{
	double scale, center;
	double windowXmin, windowXmax, windowYmin, windowYmax;

	// Define the portion of the window used for OpenGL rendering.
	glViewport( 0, 0, w, h );	// View port uses whole window

	// Set up the projection view matrix: orthographic projection
	// Determine the min and max values for x and y that should appear in the window.
	// The complication is that the aspect ratio of the window may not match the
	//		aspect ratio of the scene we want to view.
	w = (w==0) ? 1 : w;
	h = (h==0) ? 1 : h;
	if ( (Xmax-Xmin)/w < (Ymax-Ymin)/h ) {
		scale = ((Ymax-Ymin)/h)/((Xmax-Xmin)/w);
		center = (Xmax+Xmin)/2;
		windowXmin = center - (center-Xmin)*scale;
		windowXmax = center + (Xmax-center)*scale;
		windowYmin = Ymin;
		windowYmax = Ymax;
	}
	else {
		scale = ((Xmax-Xmin)/w)/((Ymax-Ymin)/h);
		center = (Ymax+Ymin)/2;
		windowYmin = center - (center-Ymin)*scale;
		windowYmax = center + (Ymax-center)*scale;
		windowXmin = Xmin;
		windowXmax = Xmax;
	}
	
	// Now that we know the max & min values for x & y that should be visible in the window,
	//		we set up the orthographic projection.
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	glOrtho( windowXmin, windowXmax, windowYmin, windowYmax, -1, 1 );

}


// Main routine
// Set up OpenGL, define the callbacks and start the main loop
int main( int argc, char** argv )
{
	glutInit(&argc,argv);

	// The image is not animated so single buffering is OK. 
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH );

	// Window position (from top corner), and size (width and hieght)
	glutInitWindowPosition( 20, 60 );
	glutInitWindowSize( 560, 560 );
	glutCreateWindow( "SimpleDraw - Press space bar to toggle images" );

	// Initialize OpenGL as we like it..
	initRendering();

	// Set up callback functions for key presses
	glutKeyboardFunc( myKeyboardFunc );			// Handles "normal" ascii symbols
	// glutSpecialFunc( mySpecialKeyFunc );		// Handles "special" keyboard keys

	// Set up the callback function for resizing windows
	glutReshapeFunc( resizeWindow );

	// Call this for background processing
	// glutIdleFunc( myIdleFunction );

	// call this whenever window needs redrawing
	glutDisplayFunc( drawScene );

	fprintf(stdout, "Press space bar to toggle images; escape button to quit.\n");
	
	// Start the main loop.  glutMainLoop never returns.
	glutMainLoop(  );

	return(0);	// This line is never reached.
}

