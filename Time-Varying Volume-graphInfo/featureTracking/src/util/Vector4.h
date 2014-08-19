#ifndef VECTOR4_H
#define VECTOR4_H

#include <cmath>
#include <iostream>

/**
 * 4D vector class
 */
template <class T>
struct Vector4 {
	/// default constructor
	Vector4(T x_ = 0, T y_ = 0, T z_ = 0, T w_ = 0) : x(x_), y(y_), z(z_), w(w_) {}
	/// copy constructor
	Vector4(const Vector4<T> &v) : x(v.x), y(v.y), z(v.z), w(v.w) {}

	/// set coordinates
	void set (T x_, T y_, T z_, T w_) { x = x_; y = y_; z = z_; w = w_; }

	/// array reference operator
	const T& operator [] (size_t i) const;
	/// array reference operator
	T&       operator [] (size_t i);

	/// assignment operator
	Vector4& operator = (const Vector4 &v) { 
		x = v.x; y = v.y; z = v.z; w = v.w; 
		return *this; 
	}

	/// boolean operator ==
	bool operator == (const Vector4 &v) const { return x == v.x && y == v.y && z == v.z && w == v.w; }
	/// boolean operator !=
	bool operator != (const Vector4 &v) const { return x != v.x || y != v.y || z != v.z || w != v.w; }
	/// boolean operator < - compare coordinates in x, y, z, and w order
	bool operator <  (const Vector4 &v) const { 
		return x < v.x || (x == v.x && (y < v.y || (y == v.y && (z < v.z || (z == v.z && w < v.w))))); 
	}

	/// operator +
	Vector4  operator +  (const Vector4 &v) const { return Vector4(x+v.x, y+v.y, z+v.z, w+v.w);   }
	/// operator +=
	Vector4& operator += (const Vector4 &v)       { x+=v.x; y+=v.y; z+=v.z; w+=v.w; return *this; }
	/// operator -
	Vector4  operator -  () const                 { return Vector4(-x, -y, -z, -w);               }
	/// operator -
	Vector4  operator -  (const Vector4 &v) const { return Vector4(x-v.x, y-v.y, z-v.z, w-v.w);   }
	/// operator -=
	Vector4& operator -= (const Vector4 &v)       { x-=v.x; y-=v.y; z-=v.z; w-=v.w; return *this; }
	/// operator *
	Vector4  operator *  (T s) const              { return Vector4(x*s, y*s, z*s, w*s);           }
	/// operator *=
	Vector4& operator *= (T s)                    { x*=s; y*=s; z*=s; w*=s; return *this;         }
	/// operator /
	Vector4  operator /  (T s) const              { return (*this) *  (1/s); }
	/// operator /=
	Vector4& operator /= (T s)                    { return (*this) *= (1/s); }

	/// normalize
	Vector4 normalize   () const { return (*this) / norm();      }
	/// norm / length
	T       norm        () const { return sqrt(normSquared());   }
	/// squared norm
	T       normSquared () const { return x*x + y*y + z*z + w*w; }


	enum {
		size = 4
	};

	/// 4D coordinates
	union {
		struct {
			T x;
			T y;
			T z;
			T w;
		};
		struct {
			T s;
			T t;
			T p;
			T q;
		};
		struct {
			T r;
			T g;
			T b;
			T a;
		};
		// workaround to prevent "dereferencing type-punned pointer will break strict-aliasing
		// rules" warning with gcc and -fstrict-aliasing.
		struct {
			T _xyz[size-1];
			T _w;
		};
		T elem[size];
	};
};

/**
 * array reference operator - with bounds checking
 */
template<class T> 
inline const T& Vector4<T>::operator [] (size_t i) const
{
	return i == 0 ? x : (i == 1 ? y : (i == 2 ? z : w));
}

/**
 * array reference operator - with bounds checking
 */
template<class T> 
inline T& Vector4<T>::operator [] (size_t i)
{
	return i == 0 ? x : (i == 1 ? y : (i == 2 ? z : w));
}

/**
 * scalar * Vector4<T>
 */
template <class T> 
inline const Vector4<T> operator * (const T &s, const Vector4<T> &v)
{
	return Vector4<T>(v.x*s, v.y*s, v.z*s, v.w*s);
}

/**
 * dot product
 */
template <class T>
inline T dot(const Vector4<T> &l, const Vector4<T> &r)
{
	return l.x*r.x + l.y*r.y + l.z*r.z + l.w*r.w;
}

/**
 * stream operator
 */
template <class T>
inline std::ostream& operator << (std::ostream &out, const Vector4<T> &r)
{
	return out << '[' << r.x << ", " << r.y << ", " << r.z << ", " << r.w << ']';
}

template <class T>
inline std::istream& operator >> (std::istream &in, Vector4<T> &r)
{
	return (((in.ignore() >> r.x).ignore(2) >> r.y).ignore(2) >> r.z).ignore(2) >> r.w;
}

/**
 * types
 */
typedef Vector4<int>     Vector4i;
typedef Vector4<float>   Vector4f;
typedef Vector4<double>  Vector4d;

typedef Vector4f        Color;
typedef Vector4<unsigned char> Color4;

inline unsigned int color2uint(const Color4& c)
{
	unsigned int result = 0;
	return ((((((result | c.a) << 8) | c.r) << 8) | c.g) << 8) | c.b;
}
#endif // VECTOR4_H
