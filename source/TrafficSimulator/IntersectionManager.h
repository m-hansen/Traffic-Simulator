#pragma once

#include <cstdint>
#include <vector>
#include <SDL.h>
#include "TrafficSignal.h"
#include "../Engine/Utils.h"

using namespace Engine;

namespace TrafficSimulator
{
	class IntersectionManager
	{
	public:
		IntersectionManager(std::uint32_t cycleTime);
		~IntersectionManager() = default;
		void Update(std::uint32_t delta);
		void AddSignal(SDL_Texture* texture, const Vector2 position);
		const std::vector<TrafficSignal>& GetTrafficSignals() const;

	private:
		std::vector<TrafficSignal> mSignals;
		std::uint32_t mTimeSinceLastCycle;
		std::uint32_t mLightCycleTimeInMs;
		std::uint32_t mSignalIndex;
	};
}
