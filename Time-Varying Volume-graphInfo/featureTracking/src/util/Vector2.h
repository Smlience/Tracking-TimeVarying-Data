#ifndef VECTOR2_H
#define VECTOR2_H

#include <cmath>
#include <iostream>
#include <assert.h>
/**
 * 2D vector class
 */
template <class T>
struct Vector2 {
	/// default constructor
	Vector2(T x_ = 0, T y_ = 0) : x(x_), y(y_) {}

	/// copy constructor
	Vector2(const Vector2<T> &v) : x(v.x), y(v.y) {}

	/// set coordinates
	void set(T x_, T y_) { x = x_; y = y_; }

	/// array reference operator
	const T& operator [] (size_t i) const { assert(i < 2); return i == 1 ? y : x; }
	/// array reference operator
	T&       operator [] (size_t i)       { assert(i < 2); return i == 1 ? y : x; }

	/// assignment operator
	Vector2& operator = (const Vector2 &v) { x = v.x; y = v.y; return *this; }

	/// boolean operator ==
	bool operator == (const Vector2 &v) const { return x == v.x && y == v.y; }
	/// boolean operator !=
	bool operator != (const Vector2 &v) const { return x != v.x || y != v.y; }
	/// boolean operator < - compare coordinates in x and y order
	bool operator <  (const Vector2 &v) const { return x < v.x || (x == v.x && y < v.y);  }

	/// operator +
	Vector2  operator +  (const Vector2 &v) const { return Vector2(x + v.x, y + v.y);     }
	/// operator +=
	Vector2& operator += (const Vector2 &v)       { x += v.x; y += v.y; return *this;     }
	/// operator -
	Vector2  operator -  () const                 { return Vector2(-x, -y);               }
	/// operator -
	Vector2  operator -  (const Vector2 &v) const { return Vector2(x - v.x, y - v.y);     }
	/// operator -=
	Vector2& operator -= (const Vector2 &v)       { x -= v.x; y -= v.y; return *this;     }
	/// operator *
	Vector2  operator *  (T s) const              { return Vector2(x * s, y * s);         }
	/// operator *=
	Vector2& operator *= (T s)                    { x *= s; y *= s; return *this;         }
	/// operator /
	Vector2  operator /  (T s) const              { assert(s != T(0)); return (*this) *  (1 / s); }
	/// operator /=
	Vector2& operator /= (T s)                    { assert(s != T(0)); return (*this) *= (1 / s); }

	/// normalize
	Vector2 normalize   () const { return (*this) / norm();    }
	/// norm / length
	T       norm        () const { return sqrt(normSquared()); }
	/// squared norm
	T       normSquared () const { return x * x + y * y;       }

	/// 2D coordinates
	T x, y;
};

/**
 * scalar * Vector2<T>
 */
template <class T>
inline const Vector2<T> operator * (const T &s, const Vector2<T> &v)
{
	return Vector2<T>(v.x * s, v.y * s);
}

/**
 * dot product
 */
template <class T> 
inline T dot (const Vector2<T> &l, const Vector2<T> &r)
{
	return l.x * r.x + l.y * r.y;
}

/**
 * stream operator
 */
template <class T>
inline std::ostream& operator << (std::ostream &out, const Vector2<T> &r)
{
	return out << '[' << r.x << ", " << r.y << ']';
}

template <class T>
inline std::istream& operator >> (std::istream &in, Vector2<T> &r)
{
	return (in.ignore() >> r.x).ignore(2) >> r.y;
}

/**
 * types
 */
typedef Vector2<int>     Vector2i;
typedef Vector2<float>   Vector2f;
typedef Vector2<double>  Vector2d;
typedef Vector2<int>     Point2i;
typedef Vector2<float>   Point2f;
typedef Vector2<double>  Point2d;

#endif // VECTOR2_H
