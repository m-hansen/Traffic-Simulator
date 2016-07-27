#include "pch.h"

namespace TrafficSimulator
{
	IntersectionManager::IntersectionManager(std::uint32_t cycleTime)
		: mLightCycleTimeInMs(cycleTime)
	{
	}
	
	void IntersectionManager::Update(std::uint32_t delta)
	{
		mTimeSinceLastCycle += delta;
		if (mTimeSinceLastCycle > mLightCycleTimeInMs)
		{
			// Cycle the lights
			mTimeSinceLastCycle = 0;
			mSignals[mSignalIndex].SetIsRed(true);
			++mSignalIndex;
			if (mSignalIndex >= mSignals.size())
				mSignalIndex = 0;
			mSignals[mSignalIndex].SetIsRed(false);
		}
	}

	void IntersectionManager::AddSignal(SDL_Texture* texture, const Vector2 position)
	{
		mSignals.emplace_back(TrafficSignal(texture, position));
	}

	const std::vector<TrafficSignal>& IntersectionManager::GetTrafficSignals() const
	{
		return mSignals;
	}
}
