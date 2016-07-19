#pragma once

#include <SDL.h>
#include "Graph.h"
#include "Vehicle.h"
#include "../Engine/Utils.h"

using namespace Engine;

namespace TrafficSimulator
{
	class Spawner final : public IDrawable
	{
	public:
		Spawner(SDL_Texture* texture, const Vector2f& position, const Graph& map);
		~Spawner() = default;
		void Update(std::uint32_t delta);
		void Draw(SDL_Renderer* renderer);
		const SDL_Rect& GetBoundingRect() const;
		void Spawner::SpawnVehicle();
		static std::list<Vehicle>& Vehicles() { return sVehicles; };
		const Node& ClosestNode() const;

	private:
		static std::list<Vehicle> sVehicles;
		const Graph& mMap;
		SDL_Texture* mTexture;
		SDL_Rect mBoundingRect;
		float mRotation;
		bool mIsIntersecting;
		std::int32_t mWidth;
		std::int32_t mHeight;
		std::uint32_t mTimeSinceLastSpawn;
		std::uint32_t mNextSpawnTime;
		const Node* mClosestNode;
	};
}
