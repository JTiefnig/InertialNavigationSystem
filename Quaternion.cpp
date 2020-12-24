// credits to https://github.com/carrino 
// 
//


#include "Quaternion.h"
#include <math.h>



Quaternion & Quaternion::operator*=(const Quaternion &q) {
    Quaternion ret;
    ret.a = a*q.a - b*q.b - c*q.c - d*q.d;
    ret.b = b*q.a + a*q.b + c*q.d - d*q.c;
    ret.c = a*q.c - b*q.d + c*q.a + d*q.b;
    ret.d = a*q.d + b*q.c - c*q.b + d*q.a;
    return (*this = ret);
}

Quaternion & Quaternion::operator+=(const Quaternion &q) {
    a += q.a;
    b += q.b;
    c += q.c;
    d += q.d;
    return *this;
}

Quaternion & Quaternion::operator*=(double scale) {
    a *= scale;
    b *= scale;
    c *= scale;
    d *= scale;
    return *this;
}

double Quaternion::norm() const {
    double norm2 = a*a + b*b + c*c + d*d;
    return sqrt(norm2);
}


Quaternion & Quaternion::normalize() {
    double n = norm();
    a /= n;
    b /= n;
    c /= n;
    d /= n;
    return *this;
}


const Quaternion Quaternion::from_euler_rotation(double x, double y, double z) {
    double c1 = cos(y/2);
    double c2 = cos(z/2);
    double c3 = cos(x/2);

    double s1 = sin(y/2);
    double s2 = sin(z/2);
    double s3 = sin(x/2);
    Quaternion ret;
    ret.a = c1 * c2 * c3 - s1 * s2 * s3;
    ret.b = s1 * s2 * c3 + c1 * c2 * s3;
    ret.c = s1 * c2 * c3 + c1 * s2 * s3;
    ret.d = c1 * s2 * c3 - s1 * c2 * s3;
    return ret;
}

const Quaternion Quaternion::from_euler_rotation_approx(double x, double y, double z) {

    double c1 = 1 - (y * y / 8);
    double c2 = 1 - (z * z / 8);
    double c3 = 1 - (x * x / 8);

    double s1 = y/2;
    double s2 = z/2;
    double s3 = x/2;
    Quaternion ret;
    ret.a = c1 * c2 * c3 - s1 * s2 * s3;
    ret.b = s1 * s2 * c3 + c1 * c2 * s3;
    ret.c = s1 * c2 * c3 + c1 * s2 * s3;
    ret.d = c1 * s2 * c3 - s1 * c2 * s3;
    return ret;
}

const Quaternion Quaternion::conj() const {
    Quaternion ret(*this);
    ret.b *= -1;
    ret.c *= -1;
    ret.d *= -1;
    return ret;
}


const Quaternion Quaternion::rotation_between_vectors(const Quaternion& q) const 
{
    Quaternion ret = (*this) * q;
    ret.a = 1 - ret.a;
    ret.normalize();
    return ret;
}

double Quaternion::dot_product(const Quaternion& q) const 
{
    return a * q.a + b * q.b + c * q.c + d * q.d;
}

const Quaternion Quaternion::rotate(const Quaternion& q) const 
{
    return (*this) * q * conj();
}


Quaternion & Quaternion::fractional(double f) 
{
    a = 1-f + f*a;
    b *= f;
    c *= f;
    d *= f;
    return normalize();
}


EulerAngles Quaternion::ToEulerAngles() const 
{
     EulerAngles angles;

    // roll (x-axis rotation)
    double sinr_cosp = 2 * (a * b + c * d);
    double cosr_cosp = 1 - 2 * (b * b + c * c);
    angles.roll = std::atan2(sinr_cosp, cosr_cosp);

    // pitch (y-axis rotation)
    double sinp = 2 * (a * c - d * b);
    if (std::abs(sinp) >= 1)
        angles.pitch = std::copysign(M_PI / 2, sinp); // use 90 degrees if out of range
    else
        angles.pitch = std::asin(sinp);

    // yaw (z-axis rotation)
    double siny_cosp = 2 * (a * d + b * c);
    double cosy_cosp = 1 - 2 * (c * c + d * d);
    angles.yaw = std::atan2(siny_cosp, cosy_cosp);

    return angles;
}