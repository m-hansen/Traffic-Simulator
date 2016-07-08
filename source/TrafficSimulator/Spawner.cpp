#include "pch.h"

namespace TrafficSimulator
{
	std::list<Vehicle> Spawner::sVehicles = {};

	Spawner::Spawner(SDL_Texture* texture, const Vector2f& position, const Graph& map)
		: mTexture(texture), mWidth(25), mHeight(25), mRotation(0), mMap(map), 
		mNextSpawnTime(0), mTimeSinceLastSpawn(0)
	{
		mBoundingRect =
		{
			static_cast<std::int32_t>(position.x - mWidth / 2),
			static_cast<std::int32_t>(position.y - mHeight / 2),
			mWidth,
			mHeight
		};
	}

	Spawner::~Spawner()
	{
		if (!sVehicles.empty())
		{
			for (auto vehicle : sVehicles)
			{
				delete &vehicle;
			}
		}
	}

	void Spawner::Update(std::uint32_t delta)
	{
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
		const std::int32_t CarWidth = 12;
		const std::int32_t CarHeight = 24;
		Vehicle* car = new Vehicle(
			TextureManager::GetTexture("car"), 
			Vector2f{ static_cast<float>(mBoundingRect.x), static_cast<float>(mBoundingRect.y) }, 
			CarWidth, 
			CarHeight, 
			mMap);
		sVehicles.emplace_back(*car);
	}
}