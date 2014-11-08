#ifndef __vector3d_h
#define __vector3d_h

#include <cmath>
#include <iostream>

class Vector {
    public:
        float x, y, z;

        Vector () {}
        ~Vector () {}
        Vector (float x, float y, float z) : x(x), y(y), z(z) {}
        Vector (const Vector& v) : x(v.x), y(v.y), z(v.z) {}

        float length () const {
            return sqrt (x*x + y*y + z*z);
        }

        bool isZero () const {
            return x == 0 && y == 0 && z == 0;
        }

        Vector unit () const {
            float len = length ();
            if (len) {
                return Vector (x / len, y / len, z / len);
            } else {
                return Vector (0, 0, 0);
            }
        }

        void normalize () {
            float len = length ();
            if (len) {
                x /= len;
                y /= len;
                z /= len;
            }
        }

        void set(float x, float y, float z) {
            this->x = x;
            this->y = y;
            this->z = z;
        }

        void set(const Vector& v) {
            set(v.x, v.y, v.z);
        }

        void set(Vector& v) {
            set(v.x, v.y, v.z);
        }

        float abs(float f) {
            
            if (f < 0) {
                return -f;    
            }
            return f;
        }

        //returns true if this vector == v1
        bool compare(Vector& v1) {
            
            float error = 0.1;
            float xDiff = abs(this->x - v1.x); 
            float yDiff = abs(this->y - v1.y); 
            float zDiff = abs(this->z - v1.z); 

            return xDiff < error && yDiff < error && zDiff < error;
        }

        Vector operator+ (const Vector& v) const {
            return Vector (x + v.x, y + v.y, z + v.z);
        }

        Vector operator- (const Vector& v) const {
            return Vector (x - v.x, y - v.y, z - v.z);
        }

        Vector operator* (float s) const {
            return Vector (x * s, y * s, z * s);
        }

        float operator* (const Vector& v) const {
            return x * v.x + y * v.y + z * v.z;
        }

        Vector crossProduct (const Vector& v) const {

            return Vector (
                y * v.z - z * v.y,
                v.x * z - x * v.z,
                x * v.y - y * v.x
            );
        }


        Vector operator= (Vector& v) const {
            return Vector(v.x, v.y, v.z);
        }

        bool operator== (const Vector& v) const {
            return x == v.x && y == v.y && z == v.z;
        }

};

std::ostream& operator<< (std::ostream&, const Vector&);

#endif

