#include "pch.h"

namespace TrafficSimulator
{
	std::uint32_t Vehicle::sTotalVehicles = 0;

	Vehicle::Vehicle(SDL_Texture* texture, const Vector2f& position, std::int32_t width, std::int32_t height, const Graph& map)
		: mInitialTargetSpeed(static_cast<float>(rand() % 40 + 20)), mSpeed(0.0f), mTargetSpeed(0.0f), mRotationSpeed(5), mTexture(texture), mPosition(position), mMap(map), mLastVisitedNode(nullptr), mItinerary(),
		mWidth(width), mHeight(height), mSensors(), mRotation(0), mRangeFinderLeft(*this, 50, 125), mRangeFinderCenter(*this, 50, 90),
		mRangeFinderRight(*this, 50, 55), mItineraryIndex(0), mTarget(nullptr), mVelocity(Vector2f{ 0, -1 }), mIsSelected(false),
		mVehicleId(sTotalVehicles++), mAdjacentAgentSensor(this, mPosition, 35), mPathNodeIdString(),
		mAcceleration(0.01f), mDeceleration(0.02f)
	{
		mBoundingRect = 
		{
			static_cast<std::int32_t>(mPosition.x - mWidth / 2),
			static_cast<std::int32_t>(mPosition.y - mHeight / 2),
			mWidth,
			mHeight
		};

		mSensors.emplace_back(&mRangeFinderLeft);
		mSensors.emplace_back(&mRangeFinderCenter);
		mSensors.emplace_back(&mRangeFinderRight);
		mSensors.emplace_back(&mAdjacentAgentSensor);

		// Set the color
		SDL_SetTextureColorMod(mTexture, 0x33, 0x99, 0xCC);

		mLastVisitedNode = const_cast<Node*>(mMap.GetNodeById(0));

		// Pick a random target location to navigate to
		srand(static_cast<uint32_t>(std::time(NULL)));
		std::int32_t randomIndex = rand() % mMap.GetNodeCount();
		NavigateTo(*mMap.GetNodeById(randomIndex));

		mTarget = mItinerary.front();

		mTargetSpeed = mInitialTargetSpeed;
	}

	bool Vehicle::operator==(const Vehicle& rhs) const
	{
		return (mVehicleId == rhs.mVehicleId);
	}

	bool Vehicle::operator!=(const Vehicle& rhs) const
	{
		return !(*this == rhs);
	}

	void Vehicle::Select()
	{
		if (!mIsSelected)
		{
			mIsSelected = true;
			if (!mItinerary.empty())
				const_cast<Graph&>(mMap).HighlightPath(mItinerary);
		}
	}

	void Vehicle::Deselect()
	{
		if (mIsSelected)
		{
			mIsSelected = false;
			if (!mItinerary.empty())
				const_cast<Graph&>(mMap).RemoveHighlight(mItinerary);
		}
	}

	void Vehicle::NavigateTo(const Node& targetNode)
	{
		assert(mLastVisitedNode);
		ClearItinerary();
		mItinerary = Pathfinder::Dijkstras(mMap, *mMap.GetNodeById(mLastVisitedNode->Id()), *mMap.GetNodeById(targetNode.Id()));
		assert(!mItinerary.empty());
		mTarget = mItinerary.front();
		if (mIsSelected) 
			const_cast<Graph&>(mMap).HighlightPath(mItinerary);
		mPathNodeIdString.clear();
		std::string delim = ", ";
		for (auto& node : mItinerary)
		{
			mPathNodeIdString += (std::to_string(node->Id()) + delim);
		}
		mPathNodeIdString = mPathNodeIdString.substr(0, mPathNodeIdString.length() - delim.length());
	}

	void Vehicle::Update(std::uint32_t delta, const std::list<Vehicle>& vehicles, const std::vector<Wall>& walls)
	{
		// Rotate to face towards target
		mRotation = 90 + static_cast<float>(atan2(mVelocity.y + mSteering.y, mVelocity.x + mSteering.x) * 180 / PI);// *mRotationSpeed * delta;

		if (mSpeed < mTargetSpeed)
		{
			mSpeed = min(mSpeed + (mAcceleration * delta), mTargetSpeed);
		}
		else if (mSpeed > mTargetSpeed)
		{
			mSpeed = max(mSpeed - (mDeceleration * delta), mTargetSpeed);
		}

		// Seek to target
		if (mTarget != nullptr)
		{
			Seek(mTarget->Position());
			mPosition += ((mVelocity + mSteering) * mSpeed) * (static_cast<float>(delta) / 1000);

			// Check if we reached target, set a new one if so
			if (abs(mPosition.x - mTarget->Position().x) < 50 &&
				abs(mPosition.y - mTarget->Position().y) < 50)
			{
				mLastVisitedNode = mTarget;
				++mItineraryIndex;
				// TODO: expensive, change the data structure after moving to smart pointers
				std::uint32_t tempIndex = 0;
				bool newTargetAcquired = false;
				for (auto& iter : mItinerary)
				{
					if (tempIndex == mItineraryIndex)
					{
						mTarget = iter;
						newTargetAcquired = true;
						break;
					}
					++tempIndex;
				}
				if (!newTargetAcquired)
				{
					RouteToRandomPath();
				}
			}
		}

		if (mRangeFinderCenter.IsIntersecting())
		{
			// Begin to decelerate
			SetTargetSpeed(0.0f);
		}
		else
		{
			SetTargetSpeed(mInitialTargetSpeed);
		}

		// Update the bounding rectangle to reflect the vehicles current position
		mBoundingRect.x = static_cast<std::int32_t>(mPosition.x) - mBoundingRect.w / 2;
		mBoundingRect.y = static_cast<std::int32_t>(mPosition.y) - mBoundingRect.h / 2;

		// Update all sensors - do this after the vehicle update logic
		mRangeFinderLeft.Update(mBoundingRect, mRotation, walls, vehicles);
		mRangeFinderCenter.Update(mBoundingRect, mRotation, walls, vehicles);
		mRangeFinderRight.Update(mBoundingRect, mRotation, walls, vehicles);
		mAdjacentAgentSensor.Update(mPosition, vehicles);
	}

	void Vehicle::Draw(SDL_Renderer* renderer)
	{
		if (!IsVisible()) 
			return;

		SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0xFF);

		if (mIsSelected)
		{
			// Render the sensors first
			mRangeFinderLeft.Draw(renderer);
			mRangeFinderCenter.Draw(renderer);
			mRangeFinderRight.Draw(renderer);
			mAdjacentAgentSensor.Draw(renderer);
		}

		SDL_RenderCopyEx(renderer, mTexture, nullptr, &mBoundingRect, mRotation, nullptr, SDL_FLIP_NONE);

		// Render the vehicle info box
		if (mIsSelected)
		{
			DisplayInfoBox(renderer);
		}
	}

	const std::list<const Node*>& Vehicle::Itinerary() const
	{
		return mItinerary;
	}

	void Vehicle::Seek(const Vector2& target)
	{
		Seek(Vector2f{ static_cast<float>(target.x), static_cast<float>(target.y) });
	}

	void Vehicle::Seek(const Vector2f& target)
	{
		Vector2f desiredVelocity = (target - mPosition).Normalize();
		mSteering = desiredVelocity - mVelocity;
	}

	const SDL_Rect& Vehicle::GetBoundingRectangle() const
	{
		return mBoundingRect;
	}

	const Vector2f& Vehicle::Position() const
	{
		return mPosition;
	}

	std::uint32_t Vehicle::ID() const
	{
		return mVehicleId;
	}

	bool Vehicle::HasReachedDestination() const
	{
		return (mTarget == nullptr);
	}

	void Vehicle::SetTargetSpeed(float targetSpeed)
	{
		mTargetSpeed = targetSpeed;
	}

	std::uint32_t Vehicle::TotalVehicleCount()
	{
		return sTotalVehicles;
	}

	void Vehicle::ClearItinerary()
	{
		if (mIsSelected) 
			const_cast<Graph&>(mMap).RemoveHighlight(mItinerary);
		mItinerary.clear();
	}

	void Vehicle::DisplayInfoBox(SDL_Renderer* renderer) const
	{
		const std::int32_t padding = 20;

		SDL_Rect infoBox;
		infoBox.x = 20;
		infoBox.y = 550;
		infoBox.w = 200;
		infoBox.h = 150;

		// Draw the box background
		SDL_SetRenderDrawColor(renderer, 0xAA, 0xAA, 0xAA, 0xAA);
		SDL_RenderFillRect(renderer, &infoBox);

		// Build the adjacent agents string
		std::string adjacentAgentsString;
		for (const auto& agent : mAdjacentAgentSensor.AdjacentAgents())
			adjacentAgentsString += std::to_string(agent.first) + ", ";
		adjacentAgentsString = adjacentAgentsString.substr(0, adjacentAgentsString.length() - 2);

		// Build the info strings
		std::string infoBoxStrings[7];
		infoBoxStrings[0] = "Agent ID: " + std::to_string(mVehicleId);
		infoBoxStrings[1] = "Position: " + std::to_string(static_cast<std::int32_t>(mPosition.x)) + ", " + std::to_string(static_cast<std::int32_t>(mPosition.y));
		infoBoxStrings[2] = "Path: " + mPathNodeIdString;
		infoBoxStrings[3] = "Adjacent Agent IDs: " + adjacentAgentsString;
		infoBoxStrings[4] = "Speed: " + Utils::FloatToString(mSpeed, 1) + " mph";
		infoBoxStrings[5] = "Last Node: " + (mLastVisitedNode != nullptr ? std::to_string(mLastVisitedNode->Id()) : "null");
		infoBoxStrings[6] = "Target Node: " + (mTarget != nullptr ? std::to_string(mTarget->Id()) : "null");

		// Render each string
		for (std::int32_t i = 0; i < sizeof(infoBoxStrings) / sizeof(infoBoxStrings[0]); ++i)
		{
			TextureManager::RenderText(renderer, "calibri", infoBoxStrings[i], Vector2{ infoBox.x, infoBox.y + padding * i });
		}
	}

	void Vehicle::RouteToRandomPath()
	{
		// Route to a new path
		mItineraryIndex = 0;
		mTarget = nullptr;

		srand(static_cast<uint32_t>(std::time(NULL)));
		std::int32_t randomIndex = rand() % mMap.GetNodeCount();
		NavigateTo(*mMap.GetNodeById(randomIndex));
		if (mItinerary.size() > 1)
		{
			auto first = mItinerary.begin();
			auto second = std::next(first);
			if (second != mItinerary.end())
				mTarget = *second;
			if (mIsSelected)
				const_cast<Graph&>(mMap).HighlightPath(mItinerary);
		}
		else
		{
			// Already at target node - clear so we calculate a new one
			ClearItinerary();
		}
	}

}
