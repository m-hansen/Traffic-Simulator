#include "pch.h"

namespace TrafficSimulator
{
	Wall::Wall(SDL_Texture* texture, const Vector2f& position)
		: mTexture(texture), mWidth(25), mHeight(25), mRotation(0)
	{
		mBoundingRect =
		{
			static_cast<std::int32_t>(position.x - mWidth / 2),
			static_cast<std::int32_t>(position.y - mHeight / 2),
			mWidth,
			mHeight
		};
	}

	void Wall::Draw(SDL_Renderer* renderer)
	{
		SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0xFF);
		SDL_RenderCopyEx(renderer, mTexture, nullptr, &mBoundingRect, mRotation, nullptr, SDL_FLIP_NONE);
	}

	const SDL_Rect& Wall::GetBoundingRect() const
	{
		return mBoundingRect;
	}
}