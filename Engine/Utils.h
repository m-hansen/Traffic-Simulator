#pragma once

#include <cstdint>

#define PI 3.14159265

namespace Engine
{
	struct Vector2
	{
		std::int32_t x;
		std::int32_t y;
	};

	struct Vector2f
	{
		Vector2f(float x1, float y1) : x(x1), y(y1) { }
		float x;
		float y;
	};

	struct Vector3
	{
		std::int32_t x;
		std::int32_t y;
		std::int32_t z;
	};

	struct Vector3f
	{
		float x;
		float y;
		float z;
	};

	class Utils
	{
	public:
		Utils();
		~Utils();
	};
}
