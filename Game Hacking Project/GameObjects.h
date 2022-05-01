#pragma once
#include "pch.h"
class Ent;

struct Vec3 {
	float x, y, z;
	Vec3(float x, float y, float z) : x(x), y(y), z(z) {};
	const Vec3 operator-(const Vec3& rhs) {
		Vec3 vec(0, 0, 0);
		vec.x = this->x - rhs.x;
		vec.y = this->y - rhs.y;
		vec.z = this->z - rhs.z;
		return vec;
	};
	float Magnitude() {
		return sqrt(x * x + y * x + z * z);
	}
	std::string toString() {
		return std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z);
	}

};

struct EntList {
	Ent* ents[31];
};

using Vector3 = Vec3;

// Created with ReClass.NET 1.2 by KN4CK3R
class Ent
{
public:
	uint32_t vTable; //0x0000
	Vector3 headPos; //0x0004
	char pad_0010[36]; //0x0010
	Vector3 playerPos; //0x0034
	Vector3 viewAngles; //0x0040
	char pad_004C[54]; //0x004C
	uint8_t playerMode; //0x0082
	char pad_0083[117]; //0x0083
	int16_t health; //0x00F8
	char pad_00FA[298]; //0x00FA
	uint8_t bShooting; //0x0224
	char name[16]; //0x0225
	char pad_0235[319]; //0x0235
	class weapon* currWeapon; //0x0374
	char pad_0378[324]; //0x0378
}; //Size: 0x04BC
// static_assert(sizeof(Ent) == 0x4BC);

class weapon
{
public:
	char pad_0000[8]; //0x0000
	class Ent* owner; //0x0008
	class gunData* gunData; //0x000C
	char pad_0010[52]; //0x0010
}; //Size: 0x0044
// static_assert(sizeof(weapon) == 0x44);

class gunData
{
public:
	char pad_0000[288]; //0x0000
	int16_t recoil1; //0x0120
	int16_t recoil2; //0x0122
	char pad_0124[54]; //0x0124
}; //Size: 0x015A
// static_assert(sizeof(gunData) == 0x15A