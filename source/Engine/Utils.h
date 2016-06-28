#pragma once

#include <cstdint>
#include <ostream>
#include <string>
#include <SDL.h>

#define PI 3.14159265

namespace Engine
{
	struct Vector2
	{
		std::int32_t x;
		std::int32_t y;
		inline bool operator==(const Vector2& rhs) const { return ((x == rhs.x) && (y == rhs.y)); }
		inline bool operator!=(const Vector2& rhs) const { return !(*this == rhs); }
	};

	struct Vector2f
	{
		float x;
		float y;
		inline bool operator==(const Vector2f& rhs) const { return ((x == rhs.x) && (y == rhs.y)); }
		inline bool operator!=(const Vector2f& rhs) const { return !(*this == rhs); }
		inline Vector2f& operator+=(const Vector2f& rhs) { 
			x += rhs.x;
			y += rhs.y;
			return *this; 
		}
		inline const Vector2f operator+(const Vector2f& other) const {
			Vector2f copy = *this;
			copy += other;
			return copy;
		}
		inline Vector2f& operator-=(const Vector2f& rhs) {
			x -= rhs.x;
			y -= rhs.y;
			return *this;
		}
		inline const Vector2f operator-(const Vector2f& other) const {
			Vector2f copy = *this;
			copy -= other;
			return copy;
		}	
		inline Vector2f& operator*=(const Vector2f& rhs) {
			x *= rhs.x;
			y *= rhs.y;
			return *this;
		}
		inline const Vector2f operator*(const Vector2f& other) const {
			Vector2f copy = *this;
			copy *= other;
			return copy;
		}
		inline Vector2f& operator*=(float rhs) {
			x *= rhs;
			y *= rhs;
			return *this;
		}
		inline const Vector2f operator*(float other) const {
			Vector2f copy = *this;
			copy *= other;
			return copy;
		}
		inline float Length() const { return sqrt(x * x + y * y); }
		inline Vector2f Normalize() const {
			Vector2f vector(Vector2f{ 0, 0 });
			float length = Length();
			if (length != 0)
			{
				vector.x = x / length;
				vector.y = y / length;
			}
			return vector;
		}
	};

	struct Vector3
	{
		Vector3(std::int32_t x1, std::int32_t y1, std::int32_t z1)
			: x(x1), y(y1), z(z1) { }
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
		static bool Utils::CollisionChecker(const SDL_Rect& a, const SDL_Rect& b)
		{
			// Set the bounds for collider A
			int leftA = a.x;
			int rightA = a.x + a.w;
			int topA = a.y;
			int bottomA = a.y + a.h;

			// Set the bounds for collider B
			int leftB = b.x;
			int rightB = b.x + b.w;
			int topB = b.y;
			int bottomB = b.y + b.h;

			// Check for collision
			// if any of these statements are true we know that there is a gap between 
			// the two colliders on the horizontal or vertical axis
			if ((bottomA <= topB) || (topA >= bottomB) || (rightA <= leftB) || (leftA >= rightB))
			{
				return false;
			}

			// Both the horizontal and vertical coordinates have an overlap
			return true;
		}
	};
}
