#if !defined(MATH_HPP)
#define MATH_HPP

#include <aurora/utils/utils.hpp>

#define Vector2Operators(c,cc)\
	cc& operator=(const c& other){x = other.x; y = other.y; return *this;}\
	cc& operator+=(const c& other){x += other.x;y += other.y;return *this;}\
	cc& operator-=(const c& other) {x -= other.x;y -= other.y;return *this;}\
	cc& operator*=(const c& other){x *= other.x;y *= other.y;return *this;}\
	cc& operator/=(const c& other) {x /= other.x;y /= other.y;return *this;}\
	cc& operator+(const c& other){x += other.x;y += other.y;return *this;}\
	cc& operator-(const c& other) {x -= other.x;y -= other.y;return *this;}\
	cc& operator*(const c& other){x *= other.x;y *= other.y;return *this;}\
	cc& operator/(const c& other) {x /= other.x;y /= other.y;return *this;}\
	bool operator<(const c& v){return x < v.x && y < v.y;}\
	bool operator>(const c& v){return x > v.x && y > v.y;}\
	bool operator<=(const c& v){return x <= v.x && y <= v.y;}\
	bool operator>=(const c& v){return x >= v.x && y >= v.y;}\
	bool operator==(const c& v){return x == v.x && y == v.y;}\
	bool operator!=(const c& v){return x != v.x || y != v.y;}\

#define Vector3Operators(c,cc)\
	cc& operator=(const c& other){x = other.x; y = other.y; z = other.z; return *this;}\
	cc& operator+=(const c& other){x += other.x;y += other.y; z+=other.z; return *this;}\
	cc& operator-=(const c& other) {x -= other.x;y -= other.y; z-=other.z; return *this;}\
	cc& operator*=(const c& other){x *= other.x;y *= other.y; z*=other.z; return *this;}\
	cc& operator/=(const c& other) {x /= other.x;y /= other.y; z/=other.z; return *this;}\
	cc& operator+(const c& other) { x += other.x; y += other.y; z += other.z; return *this; }\
	cc& operator-(const c& other) { x -= other.x; y -= other.y; z -= other.z; return *this; }\
	cc& operator*(const c& other) { x *= other.x; y *= other.y; z *= other.z; return *this; }\
	cc& operator/(const c& other) { x /= other.x; y /= other.y; z /= other.z; return *this; }\
	bool operator<(const c& v){return x < v.x && y < v.y && z < v.z;}\
	bool operator>(const c& v){return x > v.x && y > v.y && z > v.z;}\
	bool operator<=(const c& v){return x <= v.x && y <= v.y && z <= v.z;}\
	bool operator>=(const c& v){return x >= v.x && y >= v.y && z >= v.z;}\
	bool operator==(const c& v){return x == v.x && y == v.y && z == v.z;}\
	bool operator!=(const c& v){return x != v.x || y != v.y && z != v.z;}\

#define Vector4Operators(c,cc)\
	cc& operator=(const c& other){x = other.x; y = other.y; z = other.z; w = other.w; return *this;}\
	cc& operator+=(const c& other){x += other.x;y += other.y; z+=other.z; w+=other.w; return *this;}\
	cc& operator-=(const c& other) {x -= other.x;y -= other.y; z-=other.z; w-=other.w; return *this;}\
	cc& operator*=(const c& other){x *= other.x;y *= other.y; z*=other.z; w*=other.w; return *this;}\
	cc& operator/=(const c& other) {x /= other.x;y /= other.y; z/=other.z; w/=other.w; return *this;}\
	cc& operator+(const c& other){x += other.x;y += other.y; z+=other.z; w+=other.w; return *this;}\
	cc& operator-(const c& other) {x -= other.x;y -= other.y; z-=other.z; w-=other.w; return *this;}\
	cc& operator*(const c& other){x *= other.x;y *= other.y; z*=other.z; w*=other.w; return *this;}\
	cc& operator/(const c& other) {x /= other.x;y /= other.y; z/=other.z; w/=other.w; return *this;}\
	bool operator<(const c& v){return x < v.x && y < v.y && z < v.z && w < v.w;}\
	bool operator>(const c& v){return x > v.x && y > v.y && z > v.z && w > v.w;}\
	bool operator<=(const c& v){return x <= v.x && y <= v.y && z <= v.z && w <= v.w;}\
	bool operator>=(const c& v){return x >= v.x && y >= v.y && z >= v.z && w >= v.w;}\
	bool operator==(const c& v){return x == v.x && y == v.y && z == v.z && w == v.w;}\
	bool operator!=(const c& v){return x != v.x || y != v.y && z != v.z && w != v.w;}\


class Vector2 {
public:
	float x=0.0f, y=0.0f;

	Vector2(float vx, float vy) { x = vx; y = vy; }

	Vector2Operators(glm::vec2,Vector2)

	Vector2Operators(Vector2,Vector2)

	Vector2Operators(glm::vec3,Vector2)

	Vector2Operators(glm::vec4,Vector2)

};

class Vector3 {
public:
	float x = 0.0f, y = 0.0f, z = 0.0f;

	Vector3(float vx, float vy, float vz) { x = vx; y = vy; z = vz; }

	Vector3Operators(Vector3, Vector3)
	Vector3Operators(glm::vec3, Vector3)
	Vector3Operators(glm::vec4, Vector3)

};

class Vector4  {
public:
	float x = 0.0f, y = 0.0f, z = 0.0f, w = 0.0f;

	Vector4(float vx, float vy, float vz, float vw) { x = vx; y = vy; z = vz; w = vw; }

	Vector4Operators(Vector4, Vector4)
	Vector4Operators(glm::vec4, Vector4)

};

class Quaternion : public Vector4
{
public:
	Quaternion(float vx, float vy, float vz) : Vector4(vx,vy,vz,1) {}
};

struct Transform
{
	Vector3 position;
	Quaternion rotation;
	Vector3 scale;

	Transform(nullptr_t): position(0,0,0), rotation(0,0,0), scale(1,1,1)
	{
	}

	Transform() : position(0, 0, 0), rotation(0, 0, 0), scale(1, 1, 1)
	{
	}
};


#endif // MATH_HPP
