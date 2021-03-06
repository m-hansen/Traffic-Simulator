#include "pch.h"

namespace TrafficSimulator
{
	std::list<Vehicle> Spawner::sVehicles = {};

	Spawner::Spawner(SDL_Texture* texture, const Vector2f& position, const Graph& map)
		: mTexture(texture), mWidth(25), mHeight(25), mRotation(0), mMap(map), 
		mNextSpawnTime(1000), mTimeSinceLastSpawn(0)
	{
		mBoundingRect =
		{
			static_cast<std::int32_t>(position.x - mWidth / 2),
			static_cast<std::int32_t>(position.y - mHeight / 2),
			mWidth,
			mHeight
		};

		// Find the closest node
		float smallestDistanceSq = FLT_MAX;
		for (auto& node : mMap.Nodes())
		{
			float deltaX = position.x - node.Position().x;
			float deltaY = position.y - node.Position().y;
			float distanceSq = deltaX * deltaX + deltaY * deltaY;
			if (distanceSq < smallestDistanceSq)
			{
				mClosestNode = &node;
				smallestDistanceSq = distanceSq;
			}
		}

		SDL_SetTextureAlphaMod(mTexture, 175);
	}

	void Spawner::Update(std::uint32_t delta)
	{
		assert(mMap.Nodes().size() > 0);

		mTimeSinceLastSpawn += delta;
		if (mTimeSinceLastSpawn > mNextSpawnTime)
		{
			SpawnVehicle();
			mNextSpawnTime = (rand() % 5 + 1) * 1000;
			mTimeSinceLastSpawn = 0;
		}
	}

	void Spawner::Draw(SDL_Renderer* renderer)
	{
		SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0xFF);
		SDL_RenderCopyEx(renderer, mTexture, nullptr, &mBoundingRect, mRotation, nullptr, SDL_FLIP_NONE);
	}

	const SDL_Rect& Spawner::GetBoundingRect() const
	{
		return mBoundingRect;
	}

	void Spawner::SpawnVehicle()
	{
		assert(mClosestNode != nullptr);

		for (auto& vehicle : sVehicles)
		{
			if (Utils::CollisionChecker(vehicle.GetBoundingRectangle(), mBoundingRect))
			{
				// Early out - Don't spawn a vehicle if the spawner isn't clear
				return;
			}
		}

		const std::int32_t CarWidth = 12;
		const std::int32_t CarHeight = 24;
		Vehicle* car = new Vehicle(
			TextureManager::GetTexture("car"), 
			Vector2f{ static_cast<float>(mBoundingRect.x + mWidth / 2), static_cast<float>(mBoundingRect.y + mHeight / 2) }, 
			CarWidth, 
			CarHeight, 
			mMap, 
			*mClosestNode);
		sVehicles.emplace_back(*car);
	}

	const Node& Spawner::ClosestNode() const
	{
		assert(mClosestNode != nullptr);
		return *mClosestNode;
	}
}