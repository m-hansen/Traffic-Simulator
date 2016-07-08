#include "pch.h"

std::uint32_t Node::sTotalNodes = 0;
std::int32_t Node::sWidth = 50;
std::int32_t Node::sHeight = 50;

Node::Node(SDL_Renderer* renderer, SDL_Texture* texture, TTF_Font* font, std::uint32_t id, const Vector2& position)
	: mTexture(texture), mFont(font), mNodeId(id), mPosition(position), mAdjacentNodes()
{
	++sTotalNodes;

	mBoundingRect = 
	{
		mPosition.x - static_cast<std::int32_t>(sWidth / 2),
		mPosition.y - static_cast<std::int32_t>(sHeight / 2),
		sWidth,
		sHeight
	};

	SDL_Color black = { 0, 0, 0 };
	SDL_Surface* idSurface = TTF_RenderText_Solid(mFont, std::to_string(mNodeId).c_str(), black);
	if (idSurface != nullptr) 
	{
		mIdTexture = SDL_CreateTextureFromSurface(renderer, idSurface);
		SDL_FreeSurface(idSurface);
		idSurface = nullptr;
	}
}

Node::Node(const Node& rhs)
	: mTexture(rhs.mTexture), mIdTexture(rhs.mIdTexture), mFont(rhs.mFont),
	mBoundingRect(rhs.mBoundingRect), mNodeId(rhs.mNodeId), mPosition(rhs.mPosition),
	mAdjacentNodes(rhs.mAdjacentNodes)
{
}

Node& Node::operator=(const Node& rhs)
{
	if (this != &rhs)
	{
		mTexture = rhs.mTexture;
		mIdTexture = rhs.mIdTexture;
		mFont = rhs.mFont;
		mBoundingRect = rhs.mBoundingRect;
		mNodeId = rhs.mNodeId;
		mPosition = rhs.mPosition;
		mAdjacentNodes = rhs.mAdjacentNodes;
	}

	return *this;
}

bool Node::operator==(const Node& rhs) const
{
	return ((mNodeId == rhs.Id()) && (mPosition == rhs.Position()));
}

bool Node::operator!=(const Node& rhs) const
{
	return !(*this == rhs);
}

Node::~Node()
{
	// TODO: Either create a move constructor, allocate on the heap and store a vector of pointers, or use unique_ptr.
	// You are currently leaking memory, but if the texture is destroyed in the dtor you will lose the node ID until 
	// the above is implemented. Waiting on decision about graph rep before choosing implementation.
	/*if (mIdTexture != nullptr)
	{
		SDL_DestroyTexture(mIdTexture);
		mIdTexture = nullptr;
	}*/
}

void Node::InitializeForPathfinding()
{
	mMovementCost = DBL_MAX;
	mHeuristic = 0.0;
	mTotalCost = 0.0;
	mParent = nullptr;
}

void Node::Draw(SDL_Renderer* renderer)
{
	SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0xFF);

	// Visual representation of a node
	SDL_RenderCopy(renderer, mTexture, nullptr, &mBoundingRect);

	// Display the node's id
	SDL_RenderCopy(renderer, mIdTexture, nullptr, &mBoundingRect);
}

const Vector2& Node::Position() const
{
	return mPosition;
}

std::uint32_t Node::Id() const
{
	return mNodeId;
}

std::uint32_t Node::Count()
{
	return sTotalNodes;
}

void Node::ResetCount()
{
	sTotalNodes = 0;
}