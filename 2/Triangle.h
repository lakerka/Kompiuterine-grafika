#ifndef __Triangle_h__
#define __Triangle_h__

#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include "Vector.h"
#include "Colors.h"

using namespace std;

class Triangle {
    
    public:
        Vector lowerLeft;
        Vector upperCenter;
        Vector lowerRight;

    public:
        Triangle () {}

        Triangle (const Triangle& t) {

            lowerLeft.set(t.lowerLeft);
            upperCenter.set(t.upperCenter);
            lowerRight.set(t.lowerRight);
        }

        Triangle (const Vector& lowerLeft,
                const Vector& upperCenter,
                const Vector& lowerRight) {

            this->lowerLeft.set(lowerLeft);
            this->upperCenter.set(upperCenter);
            this->lowerRight.set(lowerRight);
        }

        Triangle (
                float x1, float y1, float z1,
                float x2, float y2, float z2,
                float x3, float y3, float z3
                ){
            lowerLeft.set(x1, y1, z1); 
            upperCenter.set(x2, y2, z2); 
            lowerRight.set(x3, y3, z3); 
        }

        void set(
                float x1, float y1, float z1,
                float x2, float y2, float z2,
                float x3, float y3, float z3
                ){
            lowerLeft.set(x1, y1, z1); 
            upperCenter.set(x2, y2, z2); 
            lowerRight.set(x3, y3, z3); 
        }
    
        void createVertex(Vector& v) {
            glVertex3f(v.x, v.y, v.z);
        }

        void draw() {

            glBegin(GL_TRIANGLES); 
            createVertex(lowerLeft);
            createVertex(upperCenter);
            createVertex(lowerRight);
            glEnd();
        }

        bool compare(Triangle& t) {

            return 
                lowerLeft.compare(t.lowerLeft)
                && upperCenter.compare(t.upperCenter)
                && lowerRight.compare(t.lowerRight);
        }

};

#endif
