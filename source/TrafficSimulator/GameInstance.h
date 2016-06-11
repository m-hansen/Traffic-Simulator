#pragma once

#include <string>

namespace Engine
{
	class GameInstance
	{
	public:
		~GameInstance();
		const std::uint32_t ScreenWidth = 1280;
		const std::uint32_t ScreenHeight = 720;
		const std::string ContentPath = "../Content/";
		static GameInstance* Instance();

	private:
		GameInstance();
		static GameInstance* mInstance;
	};
}
