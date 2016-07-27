#pragma once

#include <list>
#include <SDL.h>
#include "Sensor.h"
#include "IDrawable.h"
#include "Wall.h"
#include "IntersectionManager.h"
#include "../Engine/Utils.h"

using namespace Engine;

namespace TrafficSimulator
{
	class Vehicle;
	class RangeFinder final : public Sensor, public IDrawable
	{
	public:
		RangeFinder(const Vehicle& owner, float length, float angle);
		~RangeFinder();
		void Update(const SDL_Rect& position, float rotation, 
			const std::vector<IntersectionManager>& intersectionManagers, 
			const std::vector<Wall>& walls, const std::list<Vehicle>& vehicles);
		void Draw(SDL_Renderer* renderer);
		bool IsIntersecting() const { return mIsIntersecting; }

	private:
		const Vehicle& mOwner;
		Vector2f mStartPosition;
		Vector2f mEndPosition;
		SDL_Color mColor;
		float mLength;
		float mAngleInDegrees;
		bool mIsIntersecting;
	};
}
