#pragma once

#include <SDL.h>
#include <list>
#include "Sensor.h"
#include "../Engine/Utils.h"

using namespace Engine;

namespace TrafficSimulator
{
	class Vehicle;
	class AdjacentAgent final : public Sensor, public IDrawable
	{
	public:
		AdjacentAgent(const Vehicle* owner, const Vector2f& position, float radius);
		~AdjacentAgent() = default;
		void Update(const Vector2f& position, const std::list<Vehicle>& vehicles);
		void Draw(SDL_Renderer* renderer);
		void CalculateAdjacentAgents(const std::list<Vehicle>& vehicles);
		std::map<std::uint32_t, const Vehicle*> AdjacentAgents() const { return mAdjacentAgents; }

	private:
		const Vehicle* mOwner;
		Vector2f mOwnerPosition;
		float mRadius;
		SDL_Rect mBoundingRect;
		SDL_Texture* mTexture;
		std::map<std::uint32_t, const Vehicle*> mAdjacentAgents;
	};
}
