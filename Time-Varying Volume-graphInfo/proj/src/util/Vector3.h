#ifndef VECTOR3_H
#define VECTOR3_H

#include <cmath>
#include <cassert>
#include <iostream>

/**
* Vector3
**/
template <class T>
struct Vector3
{
	// create a vector
	Vector3 (T x_=0, T y_=0, T z_=0) : x(x_), y(y_), z(z_) {}

	// set coordinates
	void set (T x_, T y_, T z_) { x=x_; y=y_; z=z_; }

	// norm
	Vector3 normalize  () const { return (*this) / norm();    }
	double norm        () const { return sqrt(double(normSquared())); }
	T      normSquared () const { return x*x+y*y+z*z;			}

	/// array reference operator
	const T& operator [] (size_t i) const { assert(i < 3); return i == 0 ? x : ( i == 1 ? y : z); }
	/// array reference operator
	T&       operator [] (size_t i)       { assert(i < 3); return i == 0 ? x : ( i == 1 ? y : z); }

	// boolean operators
	bool operator == (const Vector3& v) const { return x==v.x && y==v.y && z==v.z; }
	bool operator != (const Vector3& v) const { return x!=v.x || y!=v.y || z!=v.z; }
	bool operator <  (const Vector3& v) const { return x < v.x || (x == v.x && (y < v.y || (y == v.y && z < v.z))); }

	// operators
	Vector3  operator +  (const Vector3 &v) const { return Vector3(x+v.x, y+v.y, z+v.z); }
	Vector3& operator += (const Vector3 &v)       { x+=v.x; y+=v.y; z+=v.z; return *this; }
	Vector3  operator -  () const                 { return Vector3(-x, -y, -z); }
	Vector3  operator -  (const Vector3 &v) const { return Vector3(x-v.x, y-v.y, z-v.z); }
	Vector3& operator -= (const Vector3 &v)       { x-=v.x; y-=v.y; z-=v.z; return *this; }
	Vector3  operator *  (T s) const              { return Vector3(x*s, y*s, z*s); }
	Vector3& operator *= (float s)                { x*=s; y*=s; z*=s; return *this; }
	Vector3  operator /  (float s) const          { assert(s); return (*this)* (1/s); }
	Vector3& operator /= (float s)                { assert(s); return (*this)*=(1/s); }

	// coordinates
	T x, y, z;
};

// dot product
template <class T> inline
T Dot (const Vector3<T>& l, const Vector3<T>& r)
{
	return l.x*r.x + l.y*r.y + l.z*r.z;
}

// cross product
template <class T> inline
Vector3<T> Cross (const Vector3<T>& l, const Vector3<T>& r)
{
	return Vector3<T>(
		l.y*r.z - l.z*r.y,
		l.z*r.x - l.x*r.z,
		l.x*r.y - l.y*r.x );
}

// rotate vector
template <class T> inline
Vector3<T> Rotate(const Vector3<T>& v, const Vector3<T>& axis, double theta)
{
	// Rotate vector counterclockwise around axis (looking at axis end-on) (rz(xaxis) = yaxis)
	// From Goldstein: v' = v cos t + a (v . a) [1 - cos t] - (v x a) sin t 
	const double cos_theta = cos(theta);
	const double dot = Dot(v, axis);
	Vector3<T> cross = Cross(v, axis);
	Vector3d result = v * cos_theta;
	result += axis * dot * (1.0 - cos_theta);
	result -= cross * sin(theta);
	return result;
}

template <class T> inline
Vector3<T> Rotate(const Vector3<T>& v, const Vector3<T>& origin, const Vector3<T>& axis, double theta)
{
	// Translate axis to origin
	Vector3<T> tv = v - origin;

	// Rotate point counterclockwise around axis through origin by radians ???
	Vector3<T> result = Rotate(tv, axis, theta);

	// Translate axis back from origin
	result += origin;
	return result;
}

// min/max functions
template <class T>
inline T Max  (T x, T y) { return x>=y ? x :  y; }
template <class T>
inline T Min  (T x, T y) { return x<=y ? x :  y; }

template <class T> inline
Vector3<T> Min (const Vector3<T> &l, const Vector3<T> &r)
{
	return Vector3<T>(Min(l.x,r.x), Min(l.y,r.y), Min(l.z,r.z));
}

template <class T> inline
Vector3<T> Max (const Vector3<T> &l, const Vector3<T> &r)
{
	return Vector3<T>(Max(l.x,r.x), Max(l.y,r.y), Max(l.z,r.z));
}

// dubug
template <class T> inline
std::ostream& operator << (std::ostream &out, const Vector3<T> &r)
{
	return out << '[' << r.x << ',' << r.y << ',' << r.z << ']';
}

template <class T> inline
Vector3<T> lerp(const Vector3<T> &v0, const Vector3<T> &v1, float t)
{
	return Vector3<T>(v0.x + (v1.x - v0.x) * t, v0.y + (v1.y - v0.y) * t, v0.z + (v1.z - v0.z) * t);
}

/**
* types
**/
typedef Vector3<char>   Vector3c;
typedef Vector3<int>    Vector3i;
typedef Vector3<float>  Vector3f;
typedef Vector3<double> Vector3d;

#endif
