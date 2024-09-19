#pragma once
#ifndef __MALINMATH_H__
#define __MALINMATH_H__
#include <cmath>
namespace Malin{

	template <class t> 
	class Vec2 {
	private:
		
	public:
		union {
			struct { t u, v; };
			struct { t x, y; };
			t raw[2];
		};
		Vec2():u(0),v(0){}
		Vec2(t _u, t _v) :u(_u), v(_v) {}

		inline Vec2<t> operator+(const Vec2<t>& V) const { return Vec2<t>(u + V.u, v + V.v); }
		inline Vec2<t> operator-(const Vec2<t>& V) const { return Vec2<t>(u - V.u, v - V.v); }
		inline Vec2<t> operator*(float f) const { return Vec2<t>(u * f, v * f); }
		inline t operator[](uint32_t i) { return raw[i]; }

	};

	template <class t> 
	class Vec3 {
	private:
		
	public:
		union {
			struct { t x, y, z; };
			struct { t ivert, iuv, inorm; };
			t raw[3];
		};
		Vec3() : x(0), y(0), z(0) {}
		Vec3(t _x, t _y, t _z) : x(_x), y(_y), z(_z) {}
		inline Vec3<t> operator ^(const Vec3<t>& v) const { return Vec3<t>(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x); }
		inline Vec3<t> operator +(const Vec3<t>& v) const { return Vec3<t>(x + v.x, y + v.y, z + v.z); }
		inline Vec3<t> operator -(const Vec3<t>& v) const { return Vec3<t>(x - v.x, y - v.y, z - v.z); }
		inline Vec3<t> operator *(float f)          const { return Vec3<t>(x * f, y * f, z * f); }
		inline t       operator *(const Vec3<t>& v) const { return x * v.x + y * v.y + z * v.z; }
		inline t operator[](uint32_t i) { return raw[i]; }
		float norm() const { return std::sqrt(x * x + y * y + z * z); }
		Vec3<t>& normalize() { *this =  (*this) * (1 / norm()); return *this; }
	};

	//void swap(int x,int y) {
	//	int temp
	//}
	using Vec2f = Vec2<float>;
	using Vec3f = Vec3<float>;
	using Vec2i = Vec2<int>;
	using Vec3i = Vec3<int>;
}


#endif 