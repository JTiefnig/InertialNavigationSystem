#ifndef QUATERNION_H
#define QUATERNION_H


// credits to https://github.com/carrino 
//
//


struct EulerAngles {
    double roll, pitch, yaw;
};


class Quaternion {
public:
    double a;
    double b;
    double c;
    double d;

    Quaternion() {a = 1; b = c = d = 0;}


    Quaternion(double x, double y, double z) {a = 0; b = x; c = y; d = z;}

    static const Quaternion from_euler_rotation(double x, double y, double z);

    static const Quaternion from_euler_rotation_approx(double x, double y, double z);

    Quaternion & operator=(const Quaternion &rhs) {
        a = rhs.a;
        b = rhs.b;
        c = rhs.c;
        d = rhs.d;
        return *this;
    }

    Quaternion & operator*=(const Quaternion &q);
    const Quaternion operator* (const Quaternion& q) const { return Quaternion(*this) *= q; }
    Quaternion & operator+=(const Quaternion &q);
    const Quaternion operator+(const Quaternion& q) const { return Quaternion(*this) += q; }
    Quaternion & operator*=(double scale);
    const Quaternion operator*(double scale) const { return Quaternion(*this) *= scale; }
    double norm() const;
    Quaternion & normalize();
    const Quaternion conj() const;
    

    const Quaternion rotation_between_vectors(const Quaternion& v) const;
    double dot_product(const Quaternion& q) const;

    const Quaternion rotate(const Quaternion& q) const;
    Quaternion & fractional(double f);


    EulerAngles ToEulerAngles() const;
};

#endif