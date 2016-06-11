#include "pch.h"

std::uint32_t Node::sTotalNodes = 0;
std::int32_t Node::sWidth = 25;
std::int32_t Node::sHeight = 25;

Node::Node(SDL_Renderer* renderer, SDL_Texture* texture, TTF_Font* font, const Vector2f& position)
	: mTexture(texture), mFont(font), mNodeId(sTotalNodes++), mPosition(position), mAdjacentNodes()
{
	mBoundingRect = 
	{
		static_cast<std::int32_t>(mPosition.x - sWidth / 2),
		static_cast<std::int32_t>(mPosition.y - sHeight / 2),
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

void Node::Draw(SDL_Renderer* renderer)
{
	SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0xFF);

	// Visual representation of a node
	SDL_RenderCopy(renderer, mTexture, nullptr, &mBoundingRect);

	// Display the node's id
	SDL_RenderCopy(renderer, mIdTexture, nullptr, &mBoundingRect);
}

const Vector2f& Node::GetPosition() const
{
	return mPosition;
}

std::uint32_t Node::GetId() const
{
	return mNodeId;
}