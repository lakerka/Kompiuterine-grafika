#ifndef __vector3d_h
#define __vector3d_h

#include <cmath>
#include <iostream>

class Vector
{
    public:
        float x, y, z;

        Vector () {}
        ~Vector () {}
        Vector (float x, float y, float z) : x(x), y(y), z(z) {}
        Vector (const Vector& v) : x(v.x), y(v.y), z(v.z) {}

        Vector operator= (const Vector& v);

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

        Vector operator^ (const Vector& v) const {
            return Vector (
                y * v.z - z * v.y,
                v.x * z - x * v.z,
                x * v.y - y * v.x
            );
        }

        bool operator== (const Vector& v) const {
            return x == v.x && y == v.y && z == v.z;
        }
};

std::ostream& operator<< (std::ostream&, const Vector&);

#endif
