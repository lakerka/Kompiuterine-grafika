/* Klasė surinkta dalimis iš wikipedia.org Quaternion teorijos ir įvarių
 * internete randamų 3D programavimo pamokų.
 */

#ifndef __quaternion_h
#define __quaternion_h

#include <iostream>
#include "vector.h"

class Quaternion
{
    public:
        float w, x, y, z;

        Quaternion () : w(1), x(0), y(0), z(0) {}
        Quaternion (float w, float x, float y, float z)
            : w(w), x(x), y(y), z(z) {}
        Quaternion (float w, Vector v) : w(w), x(v.x), y(v.y), z(v.z) {}
        Quaternion (const Quaternion& q) : w(q.w), x(q.x), y(q.w), z(q.z) {}
        ~Quaternion () {}

        Quaternion conjugate () {
            return Quaternion (w, -x, -y, -z);
        }

        Quaternion negate () {
            return Quaternion (-w, -x, -y, -z);
        }

        float norm () {
            return x*x + y*y + z*z + w*w;
        }

        float length () {
            return sqrt (norm ());
        }

        void normalize ();
        void toMatrix (float* matrix);
        void fromAxisAngle (Vector v, float angle);

        Quaternion operator+ (const Quaternion&) const;
        Quaternion operator- (const Quaternion&) const;
        Quaternion operator* (const Quaternion&) const;
        Quaternion operator= (const Quaternion&);
        Vector operator* (const Vector&);
};

std::ostream& operator<< (std::ostream&, const Quaternion&);

#endif
