#include "pch.h"

namespace TrafficSimulator
{
	RangeFinder::RangeFinder(const Vehicle& owner, float length, float angle)
		: mOwner(owner), mColor{ 0x00, 0xFF, 0x00, 0xFF }, mIsIntersecting(false), mLength(length), mAngleInDegrees(angle), mStartPosition(Vector2f{ 0, 0 }), mEndPosition(Vector2f{ 0, 0 })
	{
	}

	RangeFinder::~RangeFinder()
	{
	}

	void RangeFinder::Update(const SDL_Rect& position, float rotation, const std::vector<Wall>& walls, const std::list<Vehicle>& vehicles)
	{
		double rotationInRadians = (rotation - 90) * PI / 180;
		Vector2 vehicleOrigin = { position.x + position.w / 2, position.y + position.h / 2 };
		mStartPosition = { static_cast<float>(vehicleOrigin.x + position.h / 2 * cos(rotationInRadians)), static_cast<float>(vehicleOrigin.y + position.h / 2 * sin(rotationInRadians)) };

		// Calculate the begin and and of each line segment based on its rotation
		mEndPosition.x = mStartPosition.x - mLength * static_cast<float>(cos((mAngleInDegrees + rotation) * PI / 180));
		mEndPosition.y = mStartPosition.y - mLength * static_cast<float>(sin((mAngleInDegrees + rotation) * PI / 180));

		// Check for collisions
		mIsIntersecting = false;
		for (auto& wall : walls)
		{
			std::int32_t x1 = static_cast<std::int32_t>(mStartPosition.x);
			std::int32_t y1 = static_cast<std::int32_t>(mStartPosition.y);
			std::int32_t x2 = static_cast<std::int32_t>(mEndPosition.x);
			std::int32_t y2 = static_cast<std::int32_t>(mEndPosition.y);
			if (SDL_IntersectRectAndLine(&wall.GetBoundingRect(), &x1, &y1, &x2, &y2))
			{
				mIsIntersecting = true;
			}
		}
		for (auto& vehicle : vehicles)
		{
			if (vehicle == mOwner) continue;
			std::int32_t x1 = static_cast<std::int32_t>(mStartPosition.x);
			std::int32_t y1 = static_cast<std::int32_t>(mStartPosition.y);
			std::int32_t x2 = static_cast<std::int32_t>(mEndPosition.x);
			std::int32_t y2 = static_cast<std::int32_t>(mEndPosition.y);
			if (SDL_IntersectRectAndLine(&vehicle.GetBoundingRectangle(), &x1, &y1, &x2, &y2))
			{
				mIsIntersecting = true;
			}
		}
	}

	void RangeFinder::Draw(SDL_Renderer* renderer)
	{
		if (IsVisible())
		{
			mIsIntersecting ? SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0xFF) : SDL_SetRenderDrawColor(renderer, 0x00, 0xFF, 0x00, 0xFF);
			SDL_RenderDrawLine
			(
				renderer,
				static_cast<std::int32_t>(mStartPosition.x),
				static_cast<std::int32_t>(mStartPosition.y),
				static_cast<std::int32_t>(mEndPosition.x),
				static_cast<std::int32_t>(mEndPosition.y)
			);
		}
	}
}
