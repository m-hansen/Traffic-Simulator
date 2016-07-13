#include "pch.h"

namespace TrafficSimulator
{
	AdjacentAgent::AdjacentAgent(const Vehicle* owner, const Vector2f& position, float radius)
		: mOwner(owner), mRadius(radius), mTexture(TextureManager::GetTexture("adjacentAgent")), mAdjacentAgents()
	{
		mBoundingRect =
		{
			static_cast<std::int32_t>(position.x - mRadius),
			static_cast<std::int32_t>(position.y - mRadius),
			static_cast<std::int32_t>(mRadius * 2),
			static_cast<std::int32_t>(mRadius * 2)
		};
	}

	void AdjacentAgent::Update(const Vector2f& position, const std::list<Vehicle>& vehicles)
	{
		mOwnerPosition = position;
		mBoundingRect.x = static_cast<std::int32_t>(position.x - mRadius);
		mBoundingRect.y = static_cast<std::int32_t>(position.y - mRadius);
		CalculateAdjacentAgents(vehicles);
	}

	void AdjacentAgent::Draw(SDL_Renderer* renderer)
	{
		SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0xFF);

		for (const auto& agent : mAdjacentAgents)
		{
			SDL_RenderDrawLine(
				renderer, 
				static_cast<std::int32_t>(mOwnerPosition.x),
				static_cast<std::int32_t>(mOwnerPosition.y),
				static_cast<std::int32_t>(agent.second->Position().x),
				static_cast<std::int32_t>(agent.second->Position().y)
			);
		}

		// Visual representation of the sensor
		SDL_RenderCopy(renderer, mTexture, nullptr, &mBoundingRect);
	}

	void AdjacentAgent::CalculateAdjacentAgents(const std::list<Vehicle>& vehicles)
	{
		Vector2f pos = mOwnerPosition;
		for (auto& vehicle : vehicles)
		{
			if (vehicle.ID() == mOwner->ID()) continue;
			Vector2f delta = pos - vehicle.Position();
			float distance = sqrt(delta.x * delta.x + delta.y * delta.y);
			if (distance <= mRadius)
			{
				mAdjacentAgents.emplace(std::pair<std::uint32_t, const Vehicle*>(vehicle.ID(), &vehicle));
			}
			else
			{
				mAdjacentAgents.erase(vehicle.ID());
			}
		}
	}
}
