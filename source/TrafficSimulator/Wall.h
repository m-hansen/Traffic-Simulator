#pragma once

#include <cstdint>
#include <SDL.h>
#include "IDrawable.h"
#include "../Engine/Utils.h"

using namespace Engine;

namespace TrafficSimulator
{
	class Wall final : public IDrawable
	{
	public:
		Wall(SDL_Texture* texture, const Vector2f& position);
		~Wall() = default;
		void Draw(SDL_Renderer* renderer);
		const SDL_Rect& GetBoundingRect() const;

	private:
		SDL_Texture* mTexture;
		SDL_Rect mBoundingRect;
		float mRotation;
		bool mIsIntersecting;
		std::int32_t mWidth;
		std::int32_t mHeight;
	};
}
