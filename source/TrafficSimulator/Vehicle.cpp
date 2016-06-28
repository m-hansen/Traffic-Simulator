#include "pch.h"

namespace TrafficSimulator
{
	std::uint32_t Vehicle::sTotalVehicles = 0;

	Vehicle::Vehicle(SDL_Texture* texture, const Vector2f& position, std::int32_t width, std::int32_t height, const Graph& map)
		: mSpeed(30), mRotationSpeed(5), mTexture(texture), mPosition(position), mMap(map), mLastVisitedNode(nullptr), mItinerary(),
		mWidth(width), mHeight(height), mSensors(), mRotation(0), mRangeFinderLeft(100, 125), mRangeFinderCenter(100, 90),
		mRangeFinderRight(100, 55), mItineraryIndex(0), mTarget(nullptr), mVelocity(Vector2f{ 0, -1 }), mIsSelected(false),
		mVehicleId(sTotalVehicles++), mAdjacentAgentSensor(mPosition, 100), mPathNodeIdString()
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
		mItinerary = Pathfinder::Dijkstras(mMap, *mMap.GetNodeById(mLastVisitedNode->Id()), *mMap.GetNodeById(targetNode.Id()));
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
		UNREFERENCED_PARAMETER(vehicles);
		UNREFERENCED_PARAMETER(delta);
		// For debugging purposes
		/*const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);
		if (currentKeyStates[SDL_SCANCODE_UP])
		{
			mPosition.x += mSpeed * static_cast<float>(cos((mRotation - 90) * PI / 180));
			mPosition.y += mSpeed * static_cast<float>(sin((mRotation - 90) * PI / 180));
		}
		else if (currentKeyStates[SDL_SCANCODE_LEFT])
		{
			mRotation -= mRotationSpeed * delta;
		}
		else if (currentKeyStates[SDL_SCANCODE_RIGHT])
		{
			mRotation += mRotationSpeed * delta;
		}*/
		////////////////////////////////////////

		// TODO: rotate to face towards target
		mRotation += atan(mVelocity.x / mVelocity.y) * mRotationSpeed * delta;

		// Seek to target
		if (mTarget != nullptr)
		{
			Seek(mTarget->Position());
			mPosition += ((mVelocity + mSteering) * mSpeed) * (static_cast<float>(delta) / 1000);

			// Check if we reached target, set a new one if so
			if (abs(mPosition.x - mTarget->Position().x) < 50 &&
				abs(mPosition.y - mTarget->Position().y) < 50)
			{
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
					}
					++tempIndex;
				}
				if (!newTargetAcquired)
				{
					mTarget = nullptr;
				}
			}
		}

		// Update the bounding rectangle to reflect the vehicles current position
		mBoundingRect.x = static_cast<std::int32_t>(mPosition.x) - mBoundingRect.w / 2;
		mBoundingRect.y = static_cast<std::int32_t>(mPosition.y) - mBoundingRect.h / 2;

		// Update all sensors - do this after the vehicle update logic
		mRangeFinderLeft.Update(mBoundingRect, mRotation, walls);
		mRangeFinderCenter.Update(mBoundingRect, mRotation, walls);
		mRangeFinderRight.Update(mBoundingRect, mRotation, walls);
		mAdjacentAgentSensor.Update(mPosition);
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

		// TODO: rendering should be done on UI layer in future
		if (mIsSelected)
		{
			// Draw a box
			SDL_Rect r;
			r.x = 20;
			r.y = 550;
			r.w = 200;
			r.h = 150;

			SDL_SetRenderDrawColor(renderer, 0xAA, 0xAA, 0xAA, 0xFF);
			SDL_RenderFillRect(renderer, &r);

			std::int32_t padding = 20;
			
			Vector2 titlePosition = { r.x, r.y };
			Vector2 selectedAgentsPosition = { r.x, r.y + padding };
			Vector2 agentPositionPosition = { r.x, r.y + padding * 2 };
			Vector2 pathPosition = { r.x, r.y + padding * 3 };
			TextureManager::RenderText(renderer, "calibri", "INFO", titlePosition);
			TextureManager::RenderText(renderer, "calibri", "Agent ID: " + std::to_string(mVehicleId), selectedAgentsPosition);
			TextureManager::RenderText(renderer, "calibri", "Position: " + std::to_string(static_cast<std::int32_t>(mPosition.x)) + ", " + std::to_string(static_cast<std::int32_t>(mPosition.y)), agentPositionPosition);
			TextureManager::RenderText(renderer, "calibri", "Path: " + mPathNodeIdString, pathPosition);
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
}
