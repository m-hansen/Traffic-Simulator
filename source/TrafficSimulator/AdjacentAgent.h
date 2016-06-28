#pragma once

#include <SDL.h>
#include <list>
#include "Sensor.h"
#include "../Engine/Utils.h"

using namespace Engine;

namespace TrafficSimulator
{
	class AdjacentAgent final : public Sensor, public IDrawable
	{
	public:
		AdjacentAgent(const Vector2f& position, std::int32_t parentWidth, std::int32_t parentHeight, float radius);
		~AdjacentAgent() = default;
		void Update(const Vector2f& position);
		void Draw(SDL_Renderer* renderer);
		//std::list<Vehicle*> AdjacentAgents(const Vehicle& self, const std::list<Vehicle>& vehicles);

	private:
		float mRadius;
		SDL_Rect mBoundingRect;
		SDL_Texture* mTexture;
		std::int32_t mWidth;
		std::int32_t mHeight;
		std::int32_t mParentWidth;
		std::int32_t mParentHeight;
	};
}
