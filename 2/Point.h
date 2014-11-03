#ifndef __Point_H___  
#define __Point_H___

#include <GL/glut.h>	// OpenGL Graphics Utility Library


class Point {
    public:
        GLfloat x;
        GLfloat y;
        GLfloat z;
        Point(const Point& p);
        Point(GLfloat x, GLfloat y, GLfloat z);
        void set(GLfloat x, GLfloat y, GLfloat z);
        void set(const Point& p);
        void set(Point& p);
    private:
        void init(GLfloat x, GLfloat y, GLfloat z);
};

#endif 
