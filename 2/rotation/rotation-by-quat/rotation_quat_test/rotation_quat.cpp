#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include <iostream>
#include <cstdlib>
#include <cmath>
#include <cstring>

#include "quaternion.h"
#include "vector.h"

#define MAX_ITER    8

// vienas laipsnis lygus RAD radianu_
#define RAD         0.017453292519943295

using namespace std;


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

// Vektoriai

Vector Vector::operator= (const Vector& v)
{
    x = v.x;
    y = v.y;
    z = v.z;
    return Vector (x, y, z);
}

ostream& operator<< (ostream& os, const Vector& v)
{
    os << "(" << v.x << ", " << v.y << ", " << v.z << ")";
    return os;
}


// Kvaternionai

void Quaternion::normalize ()
{
    float len = length();
    if (len) {
        w /= len;
        x /= len;
        y /= len;
        z /= len;
    }
}

void Quaternion::toMatrix (float matrix[])
{

    cout << *this << endl;

    float x2 = x*x;
    float y2 = y*y;
    float z2 = z*z;
    float xy = x*y;
    float xz = x*z;
    float yz = y*z;
    float wx = w*x;
    float wy = w*y;
    float wz = w*z;

    matrix[0] = 1 - 2*y2 - 2*z2;
    matrix[1] = 2*xy + 2*wz;
    matrix[2] = 2*xz - 2*wy;

    matrix[4] = 2*xy - 2*wz;
    matrix[5] = 1 - 2*x2 - 2*z2;
    matrix[6] = 2*yz + 2*wx;

    matrix[8] = 2*xz + 2*wy;
    matrix[9] = 2*yz - 2*wx;
    matrix[10]= 1 - 2*x2 - 2*y2;

    printMaxtrix(matrix);
}

void Quaternion::fromAxisAngle (Vector v, float angle)
{
    float s = sin((angle *= 0.5));
    v.normalize();

    w = cos(angle);
    x = (v.x * s);
    y = (v.y * s);
    z = (v.z * s);
}

Quaternion Quaternion::operator+ (const Quaternion& q) const {
    return Quaternion(w + q.w, x + q.x, y + q.y, z + q.z);
}

Quaternion Quaternion::operator- (const Quaternion& q) const {
    return Quaternion(w - q.w, x - q.x, y - q.y, z - q.z);
}

Quaternion Quaternion::operator= (const Quaternion& q) {
    w = q.w;
    x = q.x;
    y = q.y;
    z = q.z;
    return *this;
}

Quaternion Quaternion::operator* (const Quaternion& q) const {
    return Quaternion (
        w * q.w - x * q.x - y * q.y - z * q.z,
        w * q.x + x * q.w + y * q.z - z * q.y,
        w * q.y - x * q.z + y * q.w + z * q.x,
        w * q.z + x * q.y - y * q.x + z * q.w
    );
}

Vector Quaternion::operator* (const Vector& vec)
{
    Vector vn(vec);
    vn.normalize();

    Quaternion v, r;
    v.x = vn.x;
    v.y = vn.y;
    v.z = vn.z;
    v.w = 0;

    r = v * conjugate();
    r = *this * r;

    return Vector(r.x, r.y, r.z);
}

ostream& operator<< (ostream& os, const Quaternion& q) {
    os << "(" << q.w
       << " + " << q.x << "i"
       << " + " << q.y << "j"
       << " + " << q.z << "k)";
    return os;
}
// end (kvaternionai)

float aspect = 1.0;
float zoom = 4;

int _x = 0, _y = 0;
int iteration = 2;

Vector xaxis (1, 0, 0);
Vector yaxis (0, 1, 0);
Quaternion rotation;

float matrix[16] = {
    1, 0, 0, 0,
    0, 1, 0, 0,
    0, 0, 1, 0,
    0, 0, 0, 1
};

GLfloat off[] = {0.0, 0.0, 0.0, 0.0};
GLfloat white[] = {1.0, 1.0, 1.0, 1.0};
GLfloat red[] = {1.0, 0.0, 0.0, 1.0};
GLfloat deep_blue[] = {0.1, 0.5, 0.8, 1.0};
GLfloat shiny[] = {50.0};
GLfloat dull[] = {0.0};




float hAngle = 0;

void motion (int x, int y)
{
    // peles poslinkiai x ir y asimi
    int dx = x - _x;
    int dy = y - _y;

    // proporcingai ikoduotam lango dydziui suskaiciuojam posuki.
    // si forumule galetu buti protingesne.
    float angle = dx * 360.0 / 512;
    hAngle += angle;
    hAngle = int(hAngle) %  361;
    Vector v = (yaxis);

    v.normalize();
    // v yra vektorius apie kuri_ suksime objekta_

    Quaternion q;
    q.fromAxisAngle (v, RAD * hAngle);
    q.normalize ();
    cout << "====================" << endl;
    cout << hAngle << endl;
    q.toMatrix (matrix);


    glutPostRedisplay ();

    // xaxis ir yaxis pritaikome atvirkstine transformacija, kad jie
    // visada butu lygiagretus globaliai xy plokstumai
    //Quaternion _q = q.conjugate();
    //xaxis = _q * xaxis; xaxis.normalize();
    //yaxis = _q * yaxis; yaxis.normalize();

    _y = y;
    _x = x;
}

void mouse (int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        _x = x;
        _y = y;
    }
}

void draw ()
{
    glMatrixMode (GL_MODELVIEW);
    glLoadIdentity ();
    gluLookAt (0, 0, zoom, 0, 0, 0, 0, 1, 0);

    glClearColor (0, 0, 0, 0);
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMultMatrixf (matrix);
  
    //glutSolidTorus(0.5, 1.0, 30, 30);
    glBegin(GL_QUADS);           // Begin drawing the pyramid with 4 triangles
    // Front
    glColor3f(1.0f, 0.0f, 1.0f);     // Red
    glVertex3f( -1.0f, -1.0f, 1.0f);
    glVertex3f(-1.0f, 1.0f, 1.0f);
    glVertex3f(1.0f, 1.0f, 1.0f);
    glVertex3f(1.0f, -1.0f, 1.0f);
    glEnd();

    glBegin(GL_QUADS);           // Begin drawing the pyramid with 4 triangles
    // Right
    glColor3f(0.0f, 0.0f, 1.0f);     // Blue
    glVertex3f(1.0f, -1.0f, 1.0f);
    glVertex3f(1.0f, 1.0f, 1.0f);
    glVertex3f(1.0f, 1.0f, -1.0f);
    glVertex3f(1.0f, -1.0f, -1.0f);
    glEnd();

    glBegin(GL_QUADS); 
    // Back
    glColor3f(0.0f, 1.0f, 0.0f);     // Green
    glVertex3f( -1.0f, -1.0f, -1.0f);
    glVertex3f(-1.0f, 1.0f, -1.0f);
    glVertex3f(1.0f, 1.0f, -1.0f);
    glVertex3f(1.0f, -1.0f, -1.0f);
    glEnd();

    glBegin(GL_QUADS); 
    // Left
    glColor3f(1.0f,1.0f,0.0f);       // Red
    glVertex3f(-1.0f, -1.0f, 1.0f);
    glVertex3f(-1.0f, 1.0f, 1.0f);
    glVertex3f(-1.0f, 1.0f, -1.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glEnd();


    glBegin(GL_QUADS); 
    // Top
    glColor3f(0.1f, 0.5f, 0.3f);       // Dark green
    glVertex3f(-1.0f, 1.0f, 1.0f);
    glVertex3f( 1.0f, 1.0f, 1.0f);
    glVertex3f( 1.0f, 1.0f, -1.0f);
    glVertex3f(-1.0f, 1.0f, -1.0f);
    glEnd();

    glBegin(GL_QUADS);
    //Bottom
    glColor3f(0.0f,0.0f,0.0f);       // Black
    glVertex3f(-1.0f, -1.0f, 1.0f);
    glVertex3f( 1.0f, -1.0f, 1.0f);
    glVertex3f( 1.0f, -1.0f, -1.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);

    glEnd();   // Done drawing bottom of the pyramid

    glutSwapBuffers ();
}

void reshape (int w, int h)
{
    glViewport (0, 0, w, h);
    aspect = (float)w / (float)h;
    glutPostRedisplay ();
}

void input (unsigned char key, int x, int y)
{
    switch (key) {
        case 27:
            exit (0);
/*
        case ']':
            iteration = iteration < MAX_ITER ? iteration + 1 : MAX_ITER;
            break;

        case '[':
            iteration = iteration > 0 ? iteration - 1 : 0;
            break;
*/    
      }
    glutPostRedisplay ();
}

void init ()
{
    glMatrixMode (GL_PROJECTION);
    gluPerspective (65, aspect, 1, 100);
    
    glClearColor (0, 0, 0, 0);
    glShadeModel (GL_SMOOTH);

    GLfloat ambient[] = { 0.1, 0.1, 0.1, 1 };
    glLightfv (GL_LIGHT0, GL_AMBIENT, ambient);

    GLfloat specular[] = { .1, .1, .1, 1 };
    glLightfv (GL_LIGHT0, GL_SPECULAR, specular);

    GLfloat diffuse[] = { .3, .3, .3, 1 };
    glLightfv (GL_LIGHT0, GL_DIFFUSE, diffuse);

    GLfloat position[] = { .1, .1, 0, 1 };
    glLightfv (GL_LIGHT0, GL_POSITION, position);
    
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

    GLfloat reflection[] = { .1, .1, .1, 1 };
    glMaterialfv (GL_FRONT_AND_BACK, GL_SPECULAR, reflection);
    glMateriali (GL_FRONT_AND_BACK, GL_SHININESS, 1);
  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
  glMaterialfv(GL_FRONT, GL_SPECULAR, off);
  glMaterialfv(GL_FRONT, GL_SHININESS, dull);   
    glEnable (GL_COLOR_MATERIAL);
    glEnable (GL_LIGHTING);
    glEnable (GL_LIGHT0);
    glEnable (GL_DEPTH_TEST);
}

void initCallbacks ()
{
    glutKeyboardFunc (input);
    glutDisplayFunc (draw);
    glutMotionFunc (motion);
    glutMouseFunc (mouse);
    glutReshapeFunc (reshape);
}

int main (int argc, char *argv[])
{
    glutInit (&argc, argv);
    glutInitDisplayMode (GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize (512, 512);
    glutCreateWindow ("Rotation using quaternions");

    init ();
    initCallbacks ();
    glutMainLoop ();

    return 0;
}
