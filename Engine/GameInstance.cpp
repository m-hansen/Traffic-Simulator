#include "GameInstance.h"

namespace Engine
{
	GameInstance* GameInstance::mInstance = nullptr;

	GameInstance::GameInstance()
	{
	}

	GameInstance::~GameInstance()
	{
	}

	GameInstance* GameInstance::Instance()
	{
		if (mInstance == nullptr)
		{
			mInstance = new GameInstance();
		}
		return mInstance;
	}
}
