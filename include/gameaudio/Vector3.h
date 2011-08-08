#ifndef GAMEAUDIO_VECTOR3_H_
#define GAMEAUDIO_VECTOR3_H_

namespace gameaudio {
	class Vector3 {
	public:
		float x, y, z;
		Vector3()
			: x(0.0f), y(0.0f), z(0.0f)
		{
		};
		Vector3(float vx, float vy, float vz)
			: x(vx), y(vy), z(vz)
		{
		};
		Vector3 operator+(const Vector3& v) const {
			return Vector3(x + v.x, y + v.y, z + v.z);
		};
		Vector3 operator-(const Vector3& v) const {
			return Vector3(x - v.x, y - v.y, z - v.z);
		};
		Vector3 operator*(float f) const {
			return Vector3(x * f, y * f, z * f);
		};
		Vector3& operator+=(const Vector3& v) {
			x += v.x;
			y += v.y;
			z += v.z;
			return *this;
		};
		Vector3& operator-=(const Vector3& v) {
			x -= v.x;
			y -= v.y;
			z -= v.z;
			return *this;
		};
		Vector3& operator*=(float f) {
			x *= f;
			y *= f;
			z *= f;
			return *this;
		};
	};
};

#endif