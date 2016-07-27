#pragma once

#include <cstdint>
#include <SDL.h>
#include "IDrawable.h"
#include "../Engine/Utils.h"

using namespace Engine;

namespace TrafficSimulator
{
	class TrafficSignal : public IDrawable
	{
	public:
		TrafficSignal(SDL_Texture* texture, const Vector2& position);
		~TrafficSignal() = default;
		void Draw(SDL_Renderer* renderer);
		bool GetIsRed() const;
		void SetIsRed(bool isRed);
		const SDL_Rect& GetBoundingRect() const;

	private:
		SDL_Texture* mTexture;
		SDL_Rect mBoundingRect;
		bool mIsRed;
		std::int32_t mWidth;
		std::int32_t mHeight;
	};
}
