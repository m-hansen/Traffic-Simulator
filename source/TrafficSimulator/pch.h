// Traffic Simulator
#pragma once

#include <Windows.h>
#include <cstdint>
#include <cmath>
#include <memory>
#include <list>
#include <vector>
#include <stdlib.h>
#include <stdio.h>
#include <string>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include "../Engine/Utils.h"
#include "TextureManager.h"

#include "GameInstance.h"
#include "Vehicle.h"
#include "Wall.h"
#include "RangeFinder.h"

#include "Node.h"
#include "Edge.h"
#include "Graph.h"
#include "GraphParser.h"
