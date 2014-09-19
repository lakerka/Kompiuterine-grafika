#include <stdlib.h>
#include <stdio.h>
#include <GL/glut.h>	// OpenGL Graphics Utility Library

int currentLevel = 0;
// max recursion level until restart occurs 
const int MAX_LEVEL = 7;

// These variables set the dimensions of the rectanglar region we wish to view.
const double Xmin = -0.5, Xmax = 1.5;
const double Ymin = -0.5, Ymax = 1.5;

void myKeyboardFunc( unsigned char key, int x, int y )
{
	switch ( key ) {
    //when space bar is pressed redraw
	case ' ':
        currentLevel = (currentLevel+1)%MAX_LEVEL;
		glutPostRedisplay();
		break;
    //when esc is presset quit
    case 27:
		exit(1);
        break;
	}
}

void draw(int num)
{
    
 switch(num) {

   case 0:
		glBegin( GL_QUADS);
        ////not symetric
        glVertex3f( 0.5, 1.0, 0.0 );
        glVertex3f( 1.0, 0.0, 0.0 );
        glVertex3f( 0.0, 0.25, 0.0 );		
        glVertex3f( 0.0, 1.0, 0.0 );

        ////square
        //glVertex2f( 0.0, 0.0 );
        //glVertex2f( 0.0, 1.0 );
        //glVertex2f( 1.0, 1.0 );
        //glVertex2f( 1.0, 0.0 );
		glEnd();
        break;

   default:
        glutPostRedisplay(); 
        glPushMatrix();
        glPushMatrix();
        glPushMatrix();
        //upper right
        glScalef(-0.25, 0.25, 0.0);
        glTranslatef(-3, 2, 0.0);
        draw(num-1);
        //upper left
        glPopMatrix();
        //mirror reflection
        glRotatef(180, -1, 1, 0.0);
        glScalef(0.5, 0.5, 0.0);
        glTranslatef(-2, -1, 0.0);
        draw(num-1);
        //lower left
        glPopMatrix();
        glScalef(0.5, 0.5, 0.0);
        draw(num-1);
        //lower right
        glPopMatrix();
        glRotatef(270, 0, 0, 1);
        glScalef(0.5, 0.5, 0.5);
        glTranslatef(-1, 1, 0.0);
        draw(num-1);
        break;   
 }
}

void drawScene(void)
{
	// Clear the rendering window
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//shape to draw color
	glColor3f( 0.0, 0.0, 0.0 );		

    static int white = 0;
    GLfloat colors[][3] = { { 1.0f, 1.0f, 1.0f} };
    //set background color
    glClearColor(colors[white][0], colors[white][1], colors[white][2], 1.0f);


    glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
    draw(currentLevel);

    // Flush the pipeline.  (Not usually necessary.)
	glFlush();
}

// Initialize OpenGL's rendering modes
void initRendering() {

	glEnable ( GL_DEPTH_TEST );
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
	glutInitWindowSize( 360, 360 );
	glutCreateWindow( "SimpleDraw - Press space bar to toggle images" );

	// Initialize OpenGL as we like it..
	initRendering();

	// Set up callback functions for key presses
	glutKeyboardFunc( myKeyboardFunc );			// Handles "normal" ascii symbols
	// Set up the callback function for resizing windows
	glutReshapeFunc( resizeWindow );

	// call this whenever window needs redrawing
	glutDisplayFunc( drawScene );

	fprintf(stdout, "Press space bar to toggle images; escape button to quit.\n");
	
	// Start the main loop.  glutMainLoop never returns.
	glutMainLoop(  );
}


