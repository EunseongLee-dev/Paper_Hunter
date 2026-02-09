#pragma once
#include "Actor/Actor.h"

using namespace Wanted;

class Goal : public Actor
{
	RTTI_DECLARATIONS(Goal, Actor)

public:
	Goal(const Vector2& pos);

	inline const Vector2& Getposition() const { return position; }

	virtual void Draw() override;

};

