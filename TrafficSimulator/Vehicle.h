#pragma once

#include <vector>
#include "IDrawable.h"
#include "RangeFinder.h"
#include "../Engine/Utils.h"

using namespace Engine;

namespace TrafficSimulator
{
	class Vehicle : public IDrawable
	{
	public:
		Vehicle(SDL_Texture* texture, const Vector2f& position, std::int32_t width, std::int32_t height);
		~Vehicle();
		void Update(std::uint32_t delta, const std::vector<Vehicle>& vehicles, const std::vector<Wall>& walls);
		void Draw(SDL_Renderer* renderer);

	private:
		SDL_Texture* mTexture;
		float mRotation;
		float mSpeed;
		float mRotationSpeed;
		SDL_Rect mVisibleRect;
		SDL_Rect mBoundingRect;
		Vector2f mPosition;
		std::int32_t mWidth;
		std::int32_t mHeight;
		RangeFinder mRangeFinderLeft;
		RangeFinder mRangeFinderCenter;
		RangeFinder mRangeFinderRight;
		std::vector<Sensor*> mSensors;
	};
}
