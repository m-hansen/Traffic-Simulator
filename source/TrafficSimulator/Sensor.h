#pragma once

namespace TrafficSimulator
{
	class Sensor
	{
	public:
		Sensor();
		virtual ~Sensor();
		bool IsActive() { return mIsActive; }

	private:
		bool mIsActive;
	};
}
