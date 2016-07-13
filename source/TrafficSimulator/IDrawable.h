#pragma once

struct SDL_Renderer;

namespace TrafficSimulator
{
	class IDrawable
	{
	public:
		IDrawable() : mIsVisible(true) { }
		virtual ~IDrawable() = default;
		virtual void Draw(SDL_Renderer* renderer) = 0;
		inline bool IsVisible() { return mIsVisible; }
		inline void SetVisible(bool isVisible) { mIsVisible = isVisible; }
	
	private :
		bool mIsVisible;
	};
}
