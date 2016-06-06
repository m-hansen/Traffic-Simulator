#pragma once

#include "Sensor.h"
#include "Wall.h"
#include "../Engine/Utils.h"

using namespace Engine;

namespace TrafficSimulator
{
	class RangeFinder final : public Sensor, public IDrawable
	{
	public:
		RangeFinder(float length, float angle);
		~RangeFinder();
		void Update(std::uint32_t delta, SDL_Rect& position, float rotation, const std::vector<Wall>& walls);
		void Draw(SDL_Renderer* renderer);

	private:
		Vector2f mStartPosition;
		Vector2f mEndPosition;
		SDL_Color mColor;
		float mLength;
		float mAngleInDegrees;
		bool mIsIntersecting;
	};
}
