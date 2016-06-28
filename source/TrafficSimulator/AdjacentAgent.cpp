#include "pch.h"

namespace TrafficSimulator
{
	AdjacentAgent::AdjacentAgent(const Vector2f& position, std::int32_t parentWidth, std::int32_t parentHeight, float radius)
		: mRadius(radius), mWidth(150), mHeight(150), mParentWidth(parentWidth),
		mParentHeight(parentHeight), mTexture(TextureManager::GetTexture("adjacentAgent"))
	{
		mBoundingRect =
		{
			static_cast<std::int32_t>(position.x - mWidth / 2 +  mParentWidth/ 2),
			static_cast<std::int32_t>(position.y - mHeight / 2 + mParentHeight / 2),
			mWidth,
			mHeight
		};
	}

	void AdjacentAgent::Update(const Vector2f& position)
	{
		mBoundingRect.x = static_cast<std::int32_t>(position.x) - mWidth / 2 + mParentWidth /2;
		mBoundingRect.y = static_cast<std::int32_t>(position.y) - mHeight / 2 + mParentHeight / 2;
	}

	void AdjacentAgent::Draw(SDL_Renderer* renderer)
	{
		SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);

		// Visual representation of the sensor
		SDL_RenderCopy(renderer, mTexture, nullptr, &mBoundingRect);
	}

	//std::list<Vehicle*> AdjacentAgent::AdjacentAgents(const Vehicle& self, const std::list<Vehicle>& vehicles)
	//{
	//	UNREFERENCED_PARAMETER(self);
	//	UNREFERENCED_PARAMETER(vehicles);
	//	//Vector2f pos = self.Position();
	//	//for (auto& vehicle : vehicles)
	//	{
	//		//Vector2f distance = pos - vehicle.Position()
	//		//if ((pos - vehicle.Position())
	//	}
	//}
}
