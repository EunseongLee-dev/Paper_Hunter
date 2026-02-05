#pragma once

#include "Actor/Actor.h"
#include "Level/PaperLevel.h"

using namespace Wanted;

class Player : public Actor
{
	RTTI_DECLARATIONS(Player, Actor)

public:
	Player(const Vector2& position, PaperLevel* level);

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float deltaTime) override;
	virtual void Draw() override;

private:
	PaperLevel* owner;

};

