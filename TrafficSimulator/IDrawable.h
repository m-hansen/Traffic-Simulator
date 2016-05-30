#pragma once

namespace TrafficSimulator
{
	class IDrawable
	{
	public:
		IDrawable() : mIsVisible(true) { }
		virtual ~IDrawable() = default;
		virtual void Draw(SDL_Renderer* renderer) = 0;
		bool IsVisible() { return mIsVisible; }
	
	private :
		bool mIsVisible;
	};
}
