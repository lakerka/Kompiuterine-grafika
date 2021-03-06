#include <stdlib.h>
#include <stdio.h>
#include <GL/glut.h>	// OpenGL Graphics Utility Library

#define  white 1.0f, 1.0f, 1.0f
#define  black 0.0f, 0.0f, 0.0f

#define  green 0.0f, 1.0f, 0.0f
#define  red 1.0f, 0.0f, 0.0f
#define  blue 0.0f, 0.0f, 1.0f

#define  cayn  0.0f, 1.0f, 1.0f
#define  magenta 1.0f, 0.0f, 1.0f
#define  darkPurple 0.5, 0.0f, 1.0f
#define  grey 0.68, 0.68f, .68f

#define clearColor white
#define shapeColor grey 

#define upperRightColor black
#define upperLeftColor red
#define lowerLeftColor green
#define lowerRightColor blue

#define COLOR_SQUARES 1

#define DEFINITE_DRAW_METHOD  0
#define PROBABALISTIC_DRAW_METHOD 1
#define SIERPINSKI_DRAW_METHOD 2

int currentDrawMethod = DEFINITE_DRAW_METHOD;
//int currentDrawMethod = SIERPINSKI_DRAW_METHOD;

int currentLevel = 0;
// max recursion level until restart occurs 
const int ITERATION_COUNT = 100;

// These variables set the dimensions of the rectanglar region we wish to view.
const double Xmin = -0.5, Xmax = 1.5;
const double Ymin = -0.5, Ymax = 1.5;

GLfloat color[][3] = {
    {upperRightColor},
    {upperLeftColor},
    {lowerLeftColor},
    {lowerRightColor}
};

bool drawFractal = true;

void clearScreen() {
    drawFractal = false;
    currentLevel = -1;
    glutPostRedisplay();
}

void setDrawMethod(int method) {

    currentDrawMethod = method;
    clearScreen();
}

void myKeyboardFunc( unsigned char key, int x, int y )
{
	switch ( key ) {
    //when space bar is pressed redraw
	case 'd':
        setDrawMethod(DEFINITE_DRAW_METHOD); 
        break;
	case 'p':
        setDrawMethod(PROBABALISTIC_DRAW_METHOD);
        break;
	case ' ':
        currentLevel = (currentLevel+1);
		glutPostRedisplay();
		break;
    //when c is pressed screen is clreared
    case 'c':
        clearScreen();
        break;
    //when esc is presset quit
    case 'e':
		exit(1);
        break;
	}
}

#define DEBUG = 1

void draw(int num)
{
    
 switch(num) {

   case 0:
		glBegin( GL_QUADS);
        //not symetric
        glVertex3f( 0.5, 1.0, 0.0 );
        glVertex3f( 1.0, 0.0, 0.0 );
        glVertex3f( 0.0, 0.25, 0.0 );		
        glVertex3f( 0.0, 1.0, 0.0 );
		glEnd();
        break;

   default:
        glutPostRedisplay(); 
        glPushMatrix();
        glPushMatrix();
        glPushMatrix();
        //upper right
        glScalef(0.25, 0.25, 0.0);
        glRotatef(180, 0, 1, 0.0);
        glTranslatef(-3, 2, 0.0);
#ifdef COLOR_SQUARES
        if (num == currentLevel) {
            glColor3f(upperRightColor);
        }
#endif
        draw(num-1);
        //upper left
        glPopMatrix();
        glScalef(0.5, 0.5, 0);
        glRotatef(90, 0, 0, 1);
        glRotatef(180, 0, 1, 0);
        glTranslatef(-2, -1, 0.0);
#ifdef COLOR_SQUARES
        if (num == currentLevel) {
            glColor3f(upperLeftColor);
        }
#endif
        draw(num-1);
        //lower left
        glPopMatrix();
        glScalef(0.5, 0.5, 0.0);

#ifdef COLOR_SQUARES
        if (num == currentLevel) {
            glColor3f(lowerLeftColor);
        }
#endif
        draw(num-1);
        //lower right
        glPopMatrix();
        glRotatef(270, 0, 0, 1);
        glScalef(0.5, 0.5, 0);
        glTranslatef(-1, 1, 0.0);
#ifdef COLOR_SQUARES
        if (num == currentLevel) {
            glColor3f(lowerRightColor);
        }
#endif
        draw(num-1);
        break;   
 }
}

void sierpinskiDraw() {

    //printf("%s", "labasssssssss");
    int it_count = 1000;
    double x = 0.0;
    double y = 0.0;

    for(int i = 0; i < it_count; i++) {
    
        int r = rand() % 3;
        double tmpX;
        double tmpY;

        if (r == 0) {
            tmpX = x*0.5;
            tmpY = 0.5*y;
        }else if (r == 1) {
            tmpX = x*0.5 + 0.5;
            tmpY = 0.5*y;
        }else {
            tmpX = x*0.5;
            tmpY = 0.5*y + 0.5;
        }
        x = tmpX;
        y = tmpY;
        glBegin( GL_POINTS);
        glVertex3f( x, y, 0.0 );
        glEnd();
    }

}

void printMaxtrix(GLfloat mat[]) {

    for(int i = 0; i < 4; i++) {
        for(int j = 0; j < 4; j++) {
            
            int ind = j*4 + i;
            printf("%f ", mat[ind]); 
            //printf("%d ", ind);
        }
        printf("%s", "\n");
    }
}

void printTransform(int transFormIndex) {

    switch(transFormIndex) {

        case 0:
            glScalef(0.25, 0.25, 0.0);
            glRotatef(180, 0, 1, 0.0);
            glTranslatef(-3, 2, 0.0);
            break;

        case 1:
            glScalef(0.5, 0.5, 0);
            glRotatef(90, 0, 0, 1);
            glRotatef(180, 0, 1, 0);
            glTranslatef(-2, -1, 0.0);
            break;
        case 2:
            glScalef(0.5, 0.5, 0.0);
            break;
        case 3:
            glScalef(0.5, 0.5, 0);
            glRotatef(270, 0, 0, 1);
            glTranslatef(-1, 1, 0.0);
            break;
    }
    GLfloat matrix[16]; 
    glGetFloatv (GL_MODELVIEW_MATRIX, matrix);

    printMaxtrix(matrix);
}

double probX = 0.0;
double probY = 0.0;

int getSquareIndex(double x, double y) {

    if (x >= 0.5 && y >= 0.5) {
        return 0;
    }

    if (x <= 0.5 && y >= 0.5) {
        return 1;
    }

    if (x <= 0.5 && y <= 0.5) {
        return 2;
    }

    return 3;

}

void probDraw() {


    int it_count = 1000;

    double tmpX;
    double tmpY;

    for(int i = 0; i < it_count; i++) {

        int r = rand()%100;

        //upper right
        if (r < 25) {
            tmpX = -0.25*probX + 0.75;
            tmpY = 0.25*probY + 0.5;
        //upper left
        }else if (r < 50) {
            tmpX = -0.5*probY + 0.5;
            tmpY = -0.5*probX + 1;
            //lower left
        }else if (r < 75) {
            tmpX = 0.5*probX;
            tmpY = 0.5*probY;
        //lower right
        }else {
            tmpX = 0.5*probY + 0.5;
            tmpY = -0.5*probX + 0.5;
        }

        probX = tmpX;
        probY = tmpY;

        int index = getSquareIndex(probX, probY);
        glColor3f(color[index][0], color[index][1], color[index][2]);

        glBegin( GL_POINTS);
        glVertex3f( probX, probY, 0);
        glEnd();
    }
}


void drawScene(void)
{

    //set shape to draw color
    glColor3f(shapeColor);
    //set clear color
    glClearColor(clearColor, 1.0f);

    if ((drawFractal &&  currentDrawMethod == DEFINITE_DRAW_METHOD)
            || !drawFractal) {
        // Clear the rendering window
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    if (drawFractal) {
        if (currentDrawMethod == PROBABALISTIC_DRAW_METHOD) {
            probDraw();
        }else if (currentDrawMethod == DEFINITE_DRAW_METHOD) {
            draw(currentLevel);
        }else {
            sierpinskiDraw();
        }
    }

    // Flush the pipeline.  (Not usually necessary.)
    glFlush();
    drawFractal = true;
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

        int transformToPrint = -1;

        //transformToPrint = 3;
        // Start the main loop.  glutMainLoop never returns.
        if (transformToPrint < 0) {
            glutMainLoop();
        }else {
            printTransform(transformToPrint);
        }
}


