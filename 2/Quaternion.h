#ifndef __Quaternion_H__  
#define __Quaternion_H__

#include "Vector.h"
#include <iostream>

class Quaternion {
    public:
        float r, x, y, z;

        Quaternion () : r(1), x(0), y(0), z(0) {}
        Quaternion (float r, float x, float y, float z)
            : r(r), x(x), y(y), z(z) {}
        Quaternion (float r, Vector v) : r(r), x(v.x), y(v.y), z(v.z) {}
        Quaternion (const Quaternion& q) : r(q.r), x(q.x), y(q.y), z(q.z) {}
        ~Quaternion () {}

        Quaternion conjugate () {
            return Quaternion (r, -x, -y, -z);
        }

        Quaternion negate () {
            return Quaternion (-r, -x, -y, -z);
        }

        float norm () {
            return x*x + y*y + z*z + r*r;
        }

        Quaternion normalize () {

            float len = length();
            if (len) {
                return Quaternion(r/len, x/len, y/len, z/len);
            }
            return Quaternion(r, x, y, z);
        }

        float length () {
            return sqrt (norm ());
        }

        Quaternion operator+ (const Quaternion& q) const {
            return Quaternion(r + q.r, x + q.x, y + q.y, z + q.z);
        }

        Quaternion operator- (const Quaternion& q) const {
            return Quaternion(r - q.r, x - q.x, y - q.y, z - q.z);
        }

        Quaternion operator= (const Quaternion& q) {
            r = q.r;
            x = q.x;
            y = q.y;
            z = q.z;
            return *this;
        }


        Quaternion operator/ (const float f) const {
            return Quaternion(r / f, x / f, y / f, z / f);
        }

        Quaternion operator* (const Vector& v) const {
            return Quaternion (
                    -x * v.x - y * v.y - z * v.z,
                    r * v.x + y * v.z - z * v.y,
                    r * v.y - x * v.z + z * v.x,
                    r * v.z + x * v.y - y * v.x
                    );
        }

        Quaternion operator* (const Quaternion& q) const {
            return Quaternion (
                    r * q.r - x * q.x - y * q.y - z * q.z,
                    r * q.x + x * q.r + y * q.z - z * q.y,
                    r * q.y - x * q.z + y * q.r + z * q.x,
                    r * q.z + x * q.y - y * q.x + z * q.r
                    );
        }
};

std::ostream& operator<< (std::ostream&, const Quaternion&);

#endif

