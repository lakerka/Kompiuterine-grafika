#include "Point.h"

Point::Point(const Point& p) {
    init(p.x, p.y, p.z);
}

Point::Point(GLfloat x, GLfloat y, GLfloat z) {
            init(x, y, z);
}

void Point::set(GLfloat x, GLfloat y, GLfloat z) {
    this->x = x;
    this->y = y;
    this->z = z;
}

void Point::set(const Point& p) {
    set(p.x, p.y, p.z);
}

void Point::set(Point& p) {
    set(p.x, p.y, p.z);
}

void Point::init(GLfloat x, GLfloat y, GLfloat z) {
            this->x = x;
            this->y = y;
            this->z = z;
}
