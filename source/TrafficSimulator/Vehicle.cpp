#include "pch.h"

namespace TrafficSimulator
{
	Vehicle::Vehicle(SDL_Texture* texture, const Vector2f& position, std::int32_t width, std::int32_t height, const Graph& map)
		: mSpeed(10), mRotationSpeed(5), mTexture(texture), mPosition(position), mMap(map), mLastVisitedNode(nullptr), mItinerary(),
		mWidth(width), mHeight(height),	mSensors(), mRotation(0), mRangeFinderLeft(150, 125), mRangeFinderCenter(150, 90),
		mRangeFinderRight(150, 55), mIsSelected(true) // TODO: initialize selected to false after left click enabled
	{
		mBoundingRect = 
		{ 
			static_cast<std::int32_t>(mPosition.x - width / 2),
			static_cast<std::int32_t>(mPosition.y - height / 2),
			mWidth,
			mHeight
		};
		mSensors.emplace_back(&mRangeFinderLeft);
		mSensors.emplace_back(&mRangeFinderCenter);
		mSensors.emplace_back(&mRangeFinderRight);
		
		// Set the color
		SDL_SetTextureColorMod(mTexture, 0x33, 0x99, 0xCC);

		mLastVisitedNode = const_cast<Node*>(mMap.GetNodeById(0));

		// Pick a random target location to navigate to
		srand(static_cast<uint32_t>(std::time(NULL)));
		std::int32_t randomIndex = rand() % mMap.GetNodeCount();
		NavigateTo(*mMap.GetNodeById(randomIndex));
	}

	Vehicle::~Vehicle()
	{
	}

	void Vehicle::NavigateTo(const Node& targetNode)
	{
		assert(mLastVisitedNode);
		mItinerary = Pathfinder::Dijkstras(mMap, *mMap.GetNodeById(mLastVisitedNode->Id()), *mMap.GetNodeById(targetNode.Id()));
	}

	void Vehicle::Update(std::uint32_t delta, const std::vector<Vehicle>& vehicles, const std::vector<Wall>& walls)
	{
		UNREFERENCED_PARAMETER(vehicles);

		// For debugging purposes
		const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);
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
		}
		////////////////////////////////////////

		// Update the bounding rectangle to reflect the vehicles current position
		mBoundingRect.x = static_cast<std::int32_t>(mPosition.x);
		mBoundingRect.y = static_cast<std::int32_t>(mPosition.y);

		// Update all sensors - do this after the vehicle update logic
		mRangeFinderLeft.Update(mBoundingRect, mRotation, walls);
		mRangeFinderCenter.Update(mBoundingRect, mRotation, walls);
		mRangeFinderRight.Update(mBoundingRect, mRotation, walls);
	}

	void Vehicle::Draw(SDL_Renderer* renderer)
	{
		SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0xFF);

		if (mIsSelected)
		{
			// Render the sensors first
			mRangeFinderLeft.Draw(renderer);
			mRangeFinderCenter.Draw(renderer);
			mRangeFinderRight.Draw(renderer);
		}

		// Draw the vehicle
		
		//SDL_RenderFillRect(renderer, &mRect);
		

		//SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0xFF);
		SDL_RenderCopyEx(renderer, mTexture, nullptr, &mBoundingRect, mRotation, nullptr, SDL_FLIP_NONE);
		// Display the sensors
		/*for (auto& sensor : mSensors)
		{
			sensor.Draw(renderer);
		}*/
		
	}

	const std::list<const Node*>& Vehicle::Itinerary() const
	{
		return mItinerary;
	}
}
