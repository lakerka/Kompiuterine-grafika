#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>   
#include "Point.h"
#include "Colors.h"
#include "Quaternion.h"
#include <GL/glut.h>	// OpenGL Graphics Utility Library

#define clearColor white
#define shapeColor grey 

#define upperRightColor black
#define upperLeftColor red
#define lowerLeftColor green
#define lowerRightColor blue

#define PI 3.14159265
#define COLOR_SQUARES 1
#define DEBUG = 1

using namespace std;

const float ROTATE_CAMERA_BY = 3.0f;
const float ZOOM_CAMERA_BY = 0.3f;

enum Direction { LEFT, RIGHT, UP, DOWN};

float aspect = 1.0;


static void print(const char* str) {
    fprintf(stderr, "%s\n", str);
}

static void print(char* str) {
    fprintf(stderr, "%s\n", str);
}

static void makeIdentity(float matrix[]) {

    for(int i = 0; i < 4; i++) {
        for(int j = 0; j < 4; j++) {

            int curInd = i*4 + j;

            if (i == j) {
                matrix[curInd] = 1;
            }else {
                matrix[curInd] = 0;
            }
        }
    }
}

static void printMaxtrix(float mat[]) {

    for(int i = 0; i < 4; i++) {
        for(int j = 0; j < 4; j++) {

            int ind = i*4 + j;
            if (j  != 0) {
                cout << ",";
            }
            cout << " " << mat[ind]; 
            //printf("%d ", ind);
        }
        cout << endl;
    }
}

static float sqr(float f) {
    return f*f;
}

static float degToRad(float f) {
    return f*PI/180.0;
}

ostream& operator<< (ostream& os, const Quaternion& q) {
    os << "===== Quaternion =====\n"
        << "(" << q.r
        << " + " << q.x << "i"
        << " + " << q.y << "j"
        << " + " << q.z << "k)";
    return os;
}

ostream& operator<< (ostream& os, const Vector& v) {
    os << "===== Vector =====\n"
        << "(" << v.x << ", " << v.y << ", " << v.z << ")";
    return os;
}

class Camera {

    private:
        static const GLfloat DEFAULT_H_ANGLE = 0.0f;
        static const GLfloat DEFAULT_V_ANGLE = 0.0f;
        static const GLfloat DEFAULT_ZOOM_LEVEL = 0.0f;
        static const GLfloat DEFAULT_MIN_ZOOM_LEVEL = -100.0f;
        static const GLfloat DEFAULT_MAX_ZOOM_LEVEL = 100.0f;

        Vector* inicialOrigin;
        Vector* inicialCenter;
        Vector* inicialUp;

        Quaternion rotor;

        GLfloat minZoomLevel, maxZoomLevel;

        GLfloat hAngle;
        GLfloat vAngle;

        Vector YAxis;
        Vector XAxis;

        GLfloat zoomLevel;

    public:

        Camera(const Vector* inicialOrigin, 
                const Vector* inicialCenter, const Vector* inicialUp) {

            init(inicialOrigin, inicialCenter, inicialUp, 
                    DEFAULT_MIN_ZOOM_LEVEL,
                    DEFAULT_MAX_ZOOM_LEVEL,
                    DEFAULT_ZOOM_LEVEL,
                    DEFAULT_H_ANGLE,
                    DEFAULT_V_ANGLE
                );
        }

        void init(
                const Vector* inicialOrigin, 
                const Vector* inicialCenter, 
                const Vector* inicialUp,
                GLfloat minZoomLevel,
                GLfloat maxZoomLevel,
                GLfloat zoomLevel,
                GLfloat hAngle,
                GLfloat vAngle
                ) {

            this->inicialOrigin= new Vector(*inicialOrigin);
            this->inicialCenter= new Vector(*inicialCenter);
            this->inicialUp = new Vector(*inicialUp);

            this->minZoomLevel = minZoomLevel;
            this->maxZoomLevel = maxZoomLevel;
            this->zoomLevel =  zoomLevel;

            this->hAngle = hAngle;
            this->vAngle = vAngle;

            YAxis.set(0.0, 1.0, 0.0);
            XAxis.set(1.0, 0.0, 0.0);

            makeIdentity(rotationMatrix);
        }



        float rotationMatrix[16];

        void toMatrix (float matrix[], Quaternion& q) {

            float r = q.r;
            float x = q.x;
            float y = q.y;
            float z = q.z;

            float x2 = x*x;
            float y2 = y*y;
            float z2 = z*z;
            float xy = x*y;
            float xz = x*z;
            float yz = y*z;
            float wx = r*x;
            float wy = r*y;
            float wz = r*z;

            matrix[0] = 1 - 2*y2 - 2*z2;
            matrix[1] = 2*xy + 2*wz;
            matrix[2] = 2*xz - 2*wy;

            matrix[4] = 2*xy - 2*wz;
            matrix[5] = 1 - 2*x2 - 2*z2;
            matrix[6] = 2*yz + 2*wx;

            matrix[8] = 2*xz + 2*wy;
            matrix[9] = 2*yz - 2*wx;
            matrix[10]= 1 - 2*x2 - 2*y2;

            //printMaxtrix(rotationMatrix);
        }



        static Quaternion fromAxisAndAngle (Vector v, float angleInDegrees) {

            angleInDegrees = angleInDegrees * 0.5f;
            float angleInRadians = degToRad(angleInDegrees);
            v.normalize();
            float r = cos(angleInRadians);
            float s = sin(angleInRadians);
            float x = (v.x * s);
            float y = (v.y * s);
            float z = (v.z * s);

            return Quaternion(r, x, y, z);
        }



        Vector multiply(Quaternion& q, const Vector& vec) {
            Vector vn(vec);
            vn.normalize();

            Quaternion v, r;
            v.x = vn.x;
            v.y = vn.y;
            v.z = vn.z;
            v.r = 0;

            Quaternion conj = q.conjugate();

            r = v * conj;
            r = q * r;

            return Vector(r.x, r.y, r.z);
        }


        void rotateVerticallyBy(GLfloat angleInDeg) {
            this->vAngle = angleInDeg;
            normalizeAngle(vAngle);
        }

        void rotateHorizontallyBy(GLfloat angleInDeg) {
            this->hAngle = angleInDeg;
            normalizeAngle(hAngle);
        }

        void normalizeAngle(GLfloat& angle) {
            if (angle > 360) {
                angle -= 360.0;
            } else if (angle < 0) {
                angle += 360.0;
            } 
        }



        void performTransformations() {

            //-------------------- Zoom ----------------------//
            //origin->z =  origin->z - zoomLevel;


            //-------------------- Rotation ----------------------//
            // Pirma Buvo meginta pilnai pasukti sukauptu kampu, taciau
            // posukis turi buti reliatyvus su 'stebetojo' posizija.
            // Turedami dabartini sukaupta kampa ir pozicija nuo, kurios
            // reikia sukti nepanaudosime sukaupto kampo, nes posukis bus per
            // didelis. Posukius turime atlikti reliatyviai su dabartine
            // naudojo pozicija nes priesingu atveju gali nutikti tokia
            // situacija: pasukame, kad matytume objekto virsu, meginame
            // pasukti i kaire(turetume judeti zemyn), taciau objektas 
            // sukasi apie savo centra, nejudame nei i apacia nei i virsu.



            //Sukame reliatyviai su dabartine 'stebetojo' pozicija.
            //Jeigu norime kad figura judetu horizontaliai turime 
            //sukti apie y asi, po posukio turi pasikeisti x axis, 
            //keiciasi x, y, z.
            //Noredami teisingai pakeisti x asi turime ja sukti 
            //apie y asi, per hAngle laipsniu, tam gali vel panaudoti 
            //kvaternijonus.


            XAxis.normalize();
            YAxis.normalize();

            //gauname posukius
            Quaternion q1 = fromAxisAndAngle (YAxis, hAngle);
            q1.normalize();

            Quaternion q2 = fromAxisAndAngle (XAxis, vAngle);
            q2.normalize();

            //pasinaudojame tuo, kad musu programa 
            //vienu metu galime pasukti tik apie x arba y axi


            ////Vector rotate(Vector axis, float angle, Vector vectorToRotate) {
            //if (hAngle > 0) {
            //XAxis.set(rotate(YAxis, hAngle, XAxis));
            //}else if (vAngle > 0) {
            //YAxis.set(rotate(XAxis, vAngle, YAxis));
            //}

            cout << "====================" << endl;
            cout << "X asis: " << XAxis << endl;
            cout << "Y asis: " << YAxis << endl;
            //posukius galime sujungti tiesiog sudaungindami kvaternijonus
            Quaternion q = q1*q2;
            q.normalize();
            rotor = rotor * q;
            rotor = rotor.normalize();
            toMatrix (rotationMatrix, rotor);

            Quaternion _q = q.conjugate();
            XAxis.set(multiply(_q, XAxis));
            XAxis.normalize();
            YAxis.set(multiply(_q, YAxis));
            YAxis.normalize();


            this->hAngle = 0;
            this->vAngle= 0;

            glutPostRedisplay ();
        }


        Vector rotate(Vector axis, float angle, Vector vectorToRotate) {

            //rotation is perfomed twice bigger than provided angle
            angle = angle / 2.0;
            float angleInRad = degToRad(angle);
            //generate quaternion q from axis and angle
            axis.normalize(); 
            float realPart = cos(angleInRad);
            Vector imgPart = axis*sin(angleInRad);
            Quaternion q(realPart, imgPart);

            //cout << "<<<<<<<<<<<<<<<<<>>>>>>>>>>>>>>>>>>>>>>" << endl;

            //cout << "imgPart: " << imgPart << endl;
            //cout << "Quaternion: q " << q << endl;

            //multiply axis by quaternion, d = qp
            Quaternion d = q*vectorToRotate;

            //cout << "vectToRotate: " << vectorToRotate << endl;
            //cout << "Quaternion d: " << d << endl;

            //find inverse q
            Quaternion invQ = q.conjugate()/sqr(q.length());

            //cout << "Quaternion q: " << q << endl;
            //cout << "Quaternion invQ: " << invQ << endl;

            //multiply d by inverse of q
            Quaternion rez = d*invQ;

            //cout << "Quaternion rez: " << rez << endl;

            Vector rezVect(rez.x, rez.y, rez.z);

            return rezVect;
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

            zoomLevel += zoomBy;
            if (zoomLevel > maxZoomLevel) {
                zoomLevel = maxZoomLevel;
            }else if (zoomLevel < minZoomLevel) {
                zoomLevel = minZoomLevel;
            }
            performTransformations();
        }
};


static Camera* camera = new Camera(
        new Vector(0.0, 0.0, 6.0),
        new Vector(0.0, 0.0, 0.0),
        new Vector(0.0, 1.0, 0.0));


void keyboardEventsHandler( unsigned char key, int x, int y ) {

    switch ( key ) {

        //------------ ZOOM ----------------//
        case '+':
            camera->zoomBy(ZOOM_CAMERA_BY);
            break;
        case '-':
            camera->zoomBy(-ZOOM_CAMERA_BY);
            break;

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
        case 'e':
        case '\e':
            exit(1);
            break;
    }
}

void mouseEventsHandler(int button, int state, int x, int y) {
   
    if ((button == 3) || (button == 4)) {

        // Disregard redundant GLUT_UP events
        if (state == GLUT_UP) {
            return;
        }

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
    }else{  
        // normal button event
        //printf("Button %s At %d %d\n", (state == GLUT_DOWN) ? "Down" : "Up", x, y);
    }
}


void display() {

    glMatrixMode (GL_MODELVIEW);
    glLoadIdentity ();
    gluLookAt (0, 0, -6, 0, 0, 0, 0, 1, 0);

    glClearColor (white,  0);
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMultMatrixf (camera->rotationMatrix);


    // Front
    glBegin(GL_QUADS); 
    glColor3f(1.0f, 0.0f, 1.0f);     // Red
    glVertex3f( -1.0f, -1.0f, 1.0f);
    glVertex3f(-1.0f, 1.0f, 1.0f);
    glVertex3f(1.0f, 1.0f, 1.0f);
    glVertex3f(1.0f, -1.0f, 1.0f);
    glEnd();

    // Right
    glBegin(GL_QUADS); 
    glColor3f(0.0f, 0.0f, 1.0f);     // Blue
    glVertex3f(1.0f, -1.0f, 1.0f);
    glVertex3f(1.0f, 1.0f, 1.0f);
    glVertex3f(1.0f, 1.0f, -1.0f);
    glVertex3f(1.0f, -1.0f, -1.0f);
    glEnd();

    // Back
    glBegin(GL_QUADS); 
    glColor3f(0.0f, 1.0f, 0.0f);     // Green
    glVertex3f( -1.0f, -1.0f, -1.0f);
    glVertex3f(-1.0f, 1.0f, -1.0f);
    glVertex3f(1.0f, 1.0f, -1.0f);
    glVertex3f(1.0f, -1.0f, -1.0f);
    glEnd();

    // Left
    glBegin(GL_QUADS); 
    glColor3f(1.0f,1.0f,0.0f);       // Red
    glVertex3f(-1.0f, -1.0f, 1.0f);
    glVertex3f(-1.0f, 1.0f, 1.0f);
    glVertex3f(-1.0f, 1.0f, -1.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glEnd();

    // Top
    glBegin(GL_QUADS); 
    glColor3f(0.1f, 0.5f, 0.3f);       // Dark green
    glVertex3f(-1.0f, 1.0f, 1.0f);
    glVertex3f( 1.0f, 1.0f, 1.0f);
    glVertex3f( 1.0f, 1.0f, -1.0f);
    glVertex3f(-1.0f, 1.0f, -1.0f);
    glEnd();

    //Bottom
    glBegin(GL_QUADS);
    glColor3f(0.0f,0.0f,0.0f);       // Black
    glVertex3f(-1.0f, -1.0f, 1.0f);
    glVertex3f( 1.0f, -1.0f, 1.0f);
    glVertex3f( 1.0f, -1.0f, -1.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glEnd();  

    // Swap the front and back frame buffers (double buffering)
    glutSwapBuffers();  
}


void reshape(GLsizei w, GLsizei h) {  
    glViewport (0, 0, w, h);
    aspect = (float)w / (float)h;
    glutPostRedisplay ();
}

void initGL() {
    glMatrixMode (GL_PROJECTION);
    gluPerspective (65, aspect, 1, 100);
    glClearColor (1, 1, 1, 0);
    glEnable (GL_COLOR_MATERIAL);
    glEnable (GL_DEPTH_TEST);
}

int main( int argc, char** argv ) {

    glutInit(&argc, argv);            // Initialize GLUT
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGB); 
    glutInitWindowSize(512, 512);   
    glutInitWindowPosition(700, 50); 
    glutCreateWindow("Rhombicosidodecahedron");          

    glutMouseFunc(mouseEventsHandler);
    glutKeyboardFunc(keyboardEventsHandler);
    glutDisplayFunc(display);       
    glutReshapeFunc(reshape);
    initGL();                       
    glutMainLoop();                 
    return 0;
}