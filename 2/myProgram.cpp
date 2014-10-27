#include <stdlib.h>
#include <stdio.h>
#include <math.h>   
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
#define DEBUG = 1

// These variables set the dimensions of the rectanglar region we wish to view.
const double Xmin = -0.5, Xmax = 0.5;
const double Ymin = -0.5, Ymax = 0.5;
const double Zmin = -0.5, Zmax = 0.5;

const float MOVE_CAMERA_BY = 0.5f;
const float ROTATE_CAMERA_BY = 0.05f;
const float ZOOM_CAMERA_BY = 1.0f;

enum Direction { LEFT, RIGHT, UP, DOWN};

void print(const char* str);
void print(char* str);

class Point {
    public:
        GLfloat x;
        GLfloat y;
        GLfloat z;
    public:
        Point(GLfloat x, GLfloat y, GLfloat z) {
            this->x = x;
            this->y = y;
            this->z = z;
        }
};


class Camera {
    private:
        //vertical angle
        GLfloat vAngle;
        //horizontal angle
        GLfloat hAngle;
    public:
        Point* inicialOrigin;
        Point* inicialCenter;
        Point* inicialUp;

        //transformations result is stored in these values
        Point* origin;
        Point* center;
        Point* up;

        Point* pan;

        GLfloat zoomLevel;
        

        bool isRotationHorizontal;

    public:
        Camera(Point* inicialOrigin, Point* inicialCenter, Point* inicialUp) {


            this->inicialOrigin= new Point(inicialOrigin->x, inicialOrigin->y, inicialOrigin->z);
            this->inicialCenter= new Point(inicialCenter->x, inicialCenter->y, inicialCenter->z);
            this->inicialUp = new Point(inicialUp->x, inicialUp->y, inicialUp->z);

            this->origin = new Point(inicialOrigin->x, inicialOrigin->y, inicialOrigin->z);
            this->center = new Point(inicialCenter->x, inicialCenter->y, inicialCenter->z);
            this->up = new Point(inicialUp->x, inicialUp->y, inicialUp->z);

            this->pan = new Point(0.0f, 0.0f, 0.0f);

            this->zoomLevel =  0.0f;

            this->vAngle = 0.0f;
            this->hAngle = 0.0f;
        }

        void rotateVerticallyBy(GLfloat angle) {
            isRotationHorizontal = false;
            vAngle += angle;
        }

        void rotateHorizontallyBy(GLfloat angle) {
            isRotationHorizontal = true;
            hAngle += angle;
        }

        void performTransformations() {


            origin->x =  inicialOrigin->x;
            origin->y =  inicialOrigin->y;
            origin->z =  inicialOrigin->z;

            center->x =  inicialCenter->x;
            center->y =  inicialCenter->y;
            center->z =  inicialCenter->z;

            up->x =  inicialUp->x;
            up->y =  inicialUp->y;
            up->z =  inicialUp->z;

            //zoom
            origin->z =  origin->z + zoomLevel;


            GLfloat z1 = origin->z*cos(hAngle) - origin->x*sin(hAngle);
            GLfloat x1 = origin->z*sin(hAngle) + origin->x*cos(hAngle);

            GLfloat z2 = origin->z*cos(vAngle) - origin->y*sin(vAngle);
            GLfloat y2 = origin->z*sin(vAngle) + origin->y*cos(vAngle);

            
            GLfloat t;

            if (isRotationHorizontal) {
                t = z1/z2;
                origin->z = z1;
                y2 *= t;
            }else {
                t = z2/z1;
                origin->z = z2;
                x1 *= t;
            }

            origin->x = x1;
            origin->y = y2;

            //rotateValues(origin->z, origin->x, hAngle, true);
            //rotateValues(origin->x, origin->y, vAngle, false);


            //if (origin->z < 0) {
                ////up->y = -1;
                //origin->z = -origin->z;
            //}else {
                ////up->y = 1;
            //}

            fprintf(stderr, "z1: %f\n", z1);
            fprintf(stderr, "z2: %f\n", z2);
            fprintf(stderr, "y: %f\n", origin->y);
            fprintf(stderr, "z: %f\n", origin->z);
            fprintf(stderr, "x: %f\n", origin->x);
            fprintf(stderr, "y: %f\n", origin->y);


            //Point* tmp2 = new Point(origin->x, origin->z, 0.0f);
            //rotateValues(tmp2->y, tmp2->z, vAngle);

            //origin->x = tmp1->x + tmp2->x + origin->x;
            //origin->z = tmp1->z + tmp2->z + origin->z;

            //glRotatef(hAngle, 0, 1, 0);
            //glRotatef(vAngle, 0, 0, 1);


            //pan
            //origin->x =  origin->x + pan->x;
            //origin->y =  origin->y + pan->y;
            //origin->z =  origin->z + pan->z;

            //center->x =  center->x + pan->x;
            //center->y =  center->y + pan->y;
            //center->z =  center->z + pan->z;


            //rotation
            //Point* tmp1 = new Point(0.707f, 0.707f, 0.0f);
            //rotateValues(tmp1->x, tmp1->z, hAngle);

            //rotateValues(origin->x, origin->z, hAngle);
            //origin->x =  origin->x + tmp1->x;
            //origin->y =  origin->y + tmp1->y;
            //origin->z =  origin->z + tmp1->z;

            glutPostRedisplay();
        }

        void rotateValues(GLfloat& firstVal, GLfloat& secondVal, GLfloat angle, bool changeFirstValue) {

            GLfloat curFirst = firstVal;
            //GLfloat curSecond = abs(secondVal);
            GLfloat curSecond = secondVal;

            //fprintf(stderr, "Current first: %f\n", curFirst);
            //fprintf(stderr, "Current second: %f\n", curSecond);

            GLfloat newFirstVal = curFirst*cos(angle) - curSecond*sin(angle);
            GLfloat newSecondVal = curFirst*sin(angle) + curSecond*cos(angle);
            if (changeFirstValue) {
                firstVal = newFirstVal;
            }
            secondVal = newSecondVal;

            //fprintf(stderr, "New first: %f\n", firstVal);
            //fprintf(stderr, "New second: %f\n", secondVal);
        }

        void rotateBy(Direction dir, GLfloat angle) {

            switch(dir) {
                case LEFT: 
                    rotateHorizontallyBy(-angle);
                    break;
                case RIGHT: 
                    rotateHorizontallyBy(angle);
                    break;
                case UP: 
                    rotateVerticallyBy(angle);
                    break;
                case DOWN: 
                    rotateVerticallyBy(-angle);
                    break;
            } 
            performTransformations();
        }

        void zoomBy(GLfloat zoomBy) {

            this->zoomLevel += zoomBy;
            performTransformations();
        }

        void moveBy(Direction dir, GLfloat moveBy) {

            switch(dir) {
                case LEFT: 
                    this->pan->x -= moveBy;
                    break;
                case RIGHT: 
                    this->pan->x += moveBy;
                    break;
                case UP: 
                    this->pan->y += moveBy;
                    break;
                case DOWN: 
                    this->pan->y -= moveBy;
                    break;
            } 
            performTransformations();
        }

        void lookAt() {

            gluLookAt(	
                    this->origin->x,
                    this->origin->y,
                    this->origin->z,

                    this->center->x,
                    this->center->y,
                    this->center->z,

                    this->up->x,
                    this->up->y,
                    this->up->z
                    );
        }
};


    Camera* camera = new Camera(
            new Point(0.0, 0.0, 6.0),
            new Point(0.0, 0.0, 0.0),
            new Point(0.0f, 1.0f, 0.0f)
            );

    void drawAxes () {


float w, h, tip = 0, turn = 0;

    float ORG[3] = {0,0,0};

    float XP[3] = {1,0,0}, XN[3] = {-1,0,0},
    YP[3] = {0,1,0}, YN[3] = {0,-1,0},
    ZP[3] = {0,0,1}, ZN[3] = {0,0,-1};

        glPushMatrix ();

        //glTranslatef (-2.4, -1.5, -5);
        //glRotatef (tip , 1,0,0);
        //glRotatef (turn, 0,1,0);
        //glScalef (0.25, 0.25, 0.25);

        glLineWidth (2.0);

        glBegin (GL_LINES);
        glColor3f (1,0,0); // X axis is red.
        glVertex3fv (ORG);
        glVertex3fv (XP );
        glColor3f (0,1,0); // Y axis is green.
        glVertex3fv (ORG);
        glVertex3fv (YP );
        glColor3f (0,0,1); // z axis is blue.
        glVertex3fv (ORG);
        glVertex3fv (ZP );
        glEnd();

        glPopMatrix ();
    }

void print(const char* str) {
    fprintf(stderr, "%s\n", str);
}

void print(char* str) {
    fprintf(stderr, "%s\n", str);
}

void zoom(float zoomLevel) {

}

void keyboardEventsHandler( unsigned char key, int x, int y ) {


    //fprintf(stderr,"EVENT WITH KEY: %c\n", key);

    switch ( key ) {
        //------------ ROTATION ----------------//
        case '4':
            camera->rotateBy(LEFT, ROTATE_CAMERA_BY);
            break;
        case '6':
            camera->rotateBy(RIGHT, ROTATE_CAMERA_BY);
            break;
        case '8':
            camera->rotateBy(UP, ROTATE_CAMERA_BY);
            break;
        case '2':
            camera->rotateBy(DOWN, ROTATE_CAMERA_BY);
            break;

        //------------ PAN ----------------//
        case 'a':
            camera->moveBy(LEFT, MOVE_CAMERA_BY);
            break;
        case 'd':
            camera->moveBy(RIGHT, MOVE_CAMERA_BY);
            break;
        case 'w':
            camera->moveBy(UP, MOVE_CAMERA_BY);
            break;
        case 's':
            camera->moveBy(DOWN, MOVE_CAMERA_BY);
            break;
        case 'c':
            break;
        case 'e':
            exit(1);
            break;
        case '\e':
            exit(1);
            break;
    }
}

void mouseEventsHandler(int button, int state, int x, int y)
{
   // Wheel reports as button 3(scroll up) and button 4(scroll down)
   if ((button == 3) || (button == 4)) // It's a wheel event
   {
       // Each wheel event reports like a button click, GLUT_DOWN then GLUT_UP
       if (state == GLUT_UP) return; // Disregard redundant GLUT_UP events

       switch ( button ) {
           //scroll up
           case 3:
               camera->zoomBy(ZOOM_CAMERA_BY);
               break;
           //scroll down
           case 4:
               camera->zoomBy(-ZOOM_CAMERA_BY);
               break;
       }
       //printf("Scroll %s At %d %d\n", (button == 3) ? "Up" : "Down", x, y);
   }else{  // normal button event
       printf("Button %s At %d %d\n", (state == GLUT_DOWN) ? "Down" : "Up", x, y);
   }
}

/* Handler for window-repaint event. Called back when the window first appears and
   whenever the window needs to be re-painted. */
void display() {

   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear color and depth buffers
   glMatrixMode(GL_MODELVIEW);     // To operate on model-view matrix
 
   // Render a pyramid consists of 4 triangles
   glLoadIdentity();                  // Reset the model-view matrix

   camera->lookAt();

   glTranslatef(-0.0f, 0.0f, 0.0f);  // Move left and into the screen

   //glRotatef(camera->vAngle*100.0, 1.0, 0.0, 0.0);

   glBegin(GL_TRIANGLES);           // Begin drawing the pyramid with 4 triangles
   // Front
   glColor3f(1.0f, 0.0f, 0.0f);     // Red
   glVertex3f( 0.0f, 1.0f, 0.0f);
   //glColor3f(0.0f, 1.0f, 0.0f);     // Green
   glVertex3f(-1.0f, -1.0f, 1.0f);
   //glColor3f(0.0f, 0.0f, 1.0f);     // Blue
   glVertex3f(1.0f, -1.0f, 1.0f);

   // Right
   glColor3f(0.0f, 0.0f, 1.0f);     // Blue
   //glColor3f(1.0f, 1.0f, 0.0f);     // Red
   glVertex3f(0.0f, 1.0f, 0.0f);
   //glColor3f(0.0f, 0.0f, 1.0f);     // Blue
   glVertex3f(1.0f, -1.0f, 1.0f);
   //glColor3f(0.0f, 1.0f, 0.0f);     // Green
   glVertex3f(1.0f, -1.0f, -1.0f);

   // Back
   glColor3f(0.0f, 1.0f, 0.0f);     // Green
   //glColor3f(1.0f, 0.0f, 0.0f);     // Red
   glVertex3f(0.0f, 1.0f, 0.0f);
   //glColor3f(0.0f, 1.0f, 0.0f);     // Green
   glVertex3f(1.0f, -1.0f, -1.0f);
   //glColor3f(0.0f, 0.0f, 1.0f);     // Blue
   glVertex3f(-1.0f, -1.0f, -1.0f);

   // Left
   glColor3f(1.0f,1.0f,0.0f);       // Red
   //glColor3f(1.0f,0.0f,0.0f);       // Red
   glVertex3f( 0.0f, 1.0f, 0.0f);
   //glColor3f(0.0f,0.0f,1.0f);       // Blue
   glVertex3f(-1.0f,-1.0f,-1.0f);
   //glColor3f(0.0f,1.0f,0.0f);       // Green
   glVertex3f(-1.0f,-1.0f, 1.0f);
   glEnd();   // Done drawing the pyramid

   
   //drawAxes();
   
   glutSwapBuffers();  // Swap the front and back frame buffers (double buffering)
}



/* Handler for window re-size event. Called back when the window first appears and
   whenever the window is re-sized with its new width and height */
void reshape(GLsizei width, GLsizei height) {  // GLsizei for non-negative integer
    // Compute aspect ratio of the new window
    if (height == 0) height = 1;                // To prevent divide by 0
    GLfloat aspect = (GLfloat)width / (GLfloat)height;

    // Set the viewport to cover the new window
    glViewport(0, 0, width, height);

   // Set the aspect ratio of the clipping volume to match the viewport
   glMatrixMode(GL_PROJECTION);  // To operate on the Projection matrix
   glLoadIdentity();             // Reset
   // Enable perspective projection with fovy, aspect, zNear and zFar
   gluPerspective(45.0f, aspect, 0.1f, 100.0f);

	// Get Back to the Modelview
	glMatrixMode(GL_MODELVIEW);
}

/* Initialize OpenGL Graphics */
void initGL() {
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // Set background color to black and opaque
    glClearDepth(1.0f);                   // Set background depth to farthest
    glEnable(GL_DEPTH_TEST);   // Enable depth testing for z-culling
    // Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LESS);
    glShadeModel(GL_FLAT);   // Enable smooth shading
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);  // Nice perspective corrections
}

// Main routine
// Set up OpenGL, define the callbacks and start the main loop
int main( int argc, char** argv ) {

   glutInit(&argc, argv);            // Initialize GLUT
   glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGBA); // Enable double buffered mode
   glutInitWindowSize(640, 480);   // Set the window's initial width & height
   glutInitWindowPosition(700, 50); // Position the window's initial top-left corner
   glutCreateWindow("Rhombicosidodecahedron");          // Create window with the given title
   glutMouseFunc(mouseEventsHandler);
   glutKeyboardFunc(keyboardEventsHandler);			// Handles "normal" ascii symbols
   glutDisplayFunc(display);       // Register callback handler for window re-paint event
   glutReshapeFunc(reshape);       // Register callback handler for window re-size event
   initGL();                       // Our own OpenGL initialization
   glutMainLoop();                 // Enter the infinite event-processing loop
   return 0;
}


