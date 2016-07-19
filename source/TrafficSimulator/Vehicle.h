#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include <list>

#include <SDL.h>
#include "IDrawable.h"
#include "RangeFinder.h"
#include "AdjacentAgent.h"
#include "Wall.h"
#include "../Engine/Utils.h"

using namespace Engine;

class Sensor;
class Node;
class Graph;

namespace TrafficSimulator
{
	class Vehicle : public IDrawable
	{
	public:
		Vehicle(SDL_Texture* texture, const Vector2f& position, std::int32_t width, std::int32_t height, const Graph& map, const Node& startNode);
		bool operator==(const Vehicle& rhs) const;
		bool operator!=(const Vehicle& rhs) const;
		~Vehicle() = default;
		void Select();
		void Deselect();
		void NavigateTo(const Node& targetNode);
		void Update(std::uint32_t delta, const std::list<Vehicle>& vehicles, const std::vector<Wall>& walls);
		void Draw(SDL_Renderer* renderer);
		const std::list<const Node*>& Itinerary() const;
		void Seek(const Vector2& target);
		void Seek(const Vector2f& target);
		const SDL_Rect& GetBoundingRectangle() const;
		const Vector2f& Position() const;
		std::uint32_t ID() const;
		bool HasReachedDestination() const;
		void SetTargetSpeed(float targetSpeed);
		static std::uint32_t TotalVehicleCount();
		void ClearItinerary();

	private:
		void DisplayInfoBox(SDL_Renderer* renderer) const;
		void RouteToRandomPath();

		static std::uint32_t sTotalVehicles;
		SDL_Texture* mTexture;
		float mRotation;
		float mSpeed;
		float mInitialTargetSpeed;
		float mTargetSpeed;
		float mRotationSpeed;
		float mAcceleration;
		float mDeceleration;
		std::uint32_t mVehicleId;
		SDL_Rect mVisibleRect;
		SDL_Rect mBoundingRect;
		Vector2f mPosition;
		const Node* mTarget;
		Vector2f mVelocity;
		Vector2f mSteering;
		std::int32_t mWidth;
		std::int32_t mHeight;
		RangeFinder mRangeFinderLeft;
		RangeFinder mRangeFinderCenter;
		RangeFinder mRangeFinderRight;
		AdjacentAgent mAdjacentAgentSensor;
		std::vector<Sensor*> mSensors;
		const Node* mLastVisitedNode;
		std::list<const Node*> mItinerary;
		std::string mPathNodeIdString;
		std::uint32_t mItineraryIndex;
		const Graph& mMap;
		bool mIsSelected;
	};
}
