#pragma once
struct Vector2
{
	float x, y;
};
struct Vector3 : Vector2
{
	float z;
};
struct Vector4 : Vector3
{
	float w;
};