#include "pch.h"

namespace TrafficSimulator
{
	RangeFinder::RangeFinder(float length, float angle)
		: mIsIntersecting(false), mLength(length), mAngleInDegrees(angle), mStartPosition(Vector2f(0, 0)), mEndPosition(Vector2f(0, 0))
	{
	}

	RangeFinder::~RangeFinder()
	{
	}

	void RangeFinder::Update(const SDL_Rect& position, float rotation)
	{
		// Only accounting for a forward facing sensor for testing purposes
		mStartPosition = { static_cast<float>(position.x + position.w / 2), static_cast<float>(position.y + position.h / 2) };
		//mEndPosition.x = mStartPosition.x;
		//mEndPosition.y = mStartPosition.y - mLength;

		// Calculate the begin and and of each line segment based on its rotation
		mEndPosition.x = mStartPosition.x - mLength * static_cast<float>(cos((mAngleInDegrees + rotation) * 3.14159265 / 180));
		mEndPosition.y = mStartPosition.y - mLength * static_cast<float>(sin((mAngleInDegrees + rotation) * 3.14159265 / 180));
		//mEndPosition.y = mLength * sin(mAngleInDegrees);
	}

	void RangeFinder::Draw(SDL_Renderer* renderer)
	{
		if (IsVisible())
		{
			SDL_SetRenderDrawColor(renderer, 0x00, 0xFF, 0x00, 0xFF);
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
