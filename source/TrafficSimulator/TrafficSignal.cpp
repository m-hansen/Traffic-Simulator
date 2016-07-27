#include "pch.h"

namespace TrafficSimulator
{
	TrafficSignal::TrafficSignal(SDL_Texture* texture, const Vector2& position)
		: mTexture(texture), mWidth(10), mHeight(10), mIsRed(true)
	{
		mBoundingRect =
		{
			position.x - mWidth / 2,
			position.y - mHeight / 2,
			mWidth,
			mHeight
		};
	}

	void TrafficSignal::Draw(SDL_Renderer* renderer)
	{
		if (mIsRed)
			SDL_SetTextureColorMod(mTexture, 0xFF, 0x00, 0x00); // Red
		else
			SDL_SetTextureColorMod(mTexture, 0x00, 0xFF, 0x00); // Green
		SDL_RenderCopy(renderer, mTexture, nullptr, &mBoundingRect);
	}

	bool TrafficSignal::GetIsRed() const
	{
		return mIsRed;
	}

	void TrafficSignal::SetIsRed(bool isRed)
	{
		mIsRed = isRed;
	}

	const SDL_Rect& TrafficSignal::GetBoundingRect() const
	{
		return mBoundingRect;
	}
}
