#pragma once

#include "Sensor.h"
#include "../Engine/Utils.h"

using namespace Engine;

namespace TrafficSimulator
{
	class RangeFinder final : public Sensor, public IDrawable
	{
	public:
		RangeFinder(float length, float angle);
		~RangeFinder();
		void Update(const SDL_Rect& position, float rotation);
		void Draw(SDL_Renderer* renderer);

	private:
		Vector2f mStartPosition;
		Vector2f mEndPosition;
		float mLength;
		float mAngleInDegrees;
		bool mIsIntersecting;
	};
}
