#pragma once

#include <vector>
#include "IDrawable.h"
#include "RangeFinder.h"
#include "../Engine/Utils.h"

using namespace Engine;

class Node;
class Graph;

namespace TrafficSimulator
{
	class Vehicle : public IDrawable
	{
	public:
		Vehicle(SDL_Texture* texture, const Vector2f& position, std::int32_t width, std::int32_t height, const Graph& map);
		~Vehicle() = default;
		void Update(std::uint32_t delta, const std::vector<Vehicle>& vehicles, const std::vector<Wall>& walls);
		void Draw(SDL_Renderer* renderer);
		void Select();
		void Deselect();
		void NavigateTo(const Node& targetNode);
		const std::list<const Node*>& Itinerary() const;
		void Seek(const Vector2& target);
		void Seek(const Vector2f& target);
		const SDL_Rect& GetBoundingRectangle() const { return mBoundingRect; }
		inline std::uint32_t ID() const { return mVehicleId; }

	private:
		static std::uint32_t sTotalVehicles;
		SDL_Texture* mTexture;
		float mRotation;
		float mSpeed;
		float mRotationSpeed;
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
		std::vector<Sensor*> mSensors;
		Node* mLastVisitedNode;
		std::list<const Node*> mItinerary;
		std::uint32_t mItineraryIndex;
		const Graph& mMap;
		bool mIsSelected;
	};
}
