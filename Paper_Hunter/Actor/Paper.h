#pragma once

#include "Actor/Actor.h"

using namespace Wanted;

class Paper : public Actor
{
	RTTI_DECLARATIONS(Paper, Actor)

public:
	Paper(const Vector2& position);
};

