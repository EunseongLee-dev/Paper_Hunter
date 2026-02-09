#pragma once

#include "Actor/Actor.h"


using namespace Wanted;

class PaperLevel;

class Player : public Actor
{
	RTTI_DECLARATIONS(Player, Actor)

public:
	Player(const Vector2& position, PaperLevel* level);


protected:
	virtual void BeginPlay() override;
	virtual void Tick(float deltaTime) override;
	virtual void Draw() override;

public:
	inline int GetPaperCount() { return paperCount; }

private:
	PaperLevel* owner;
	int paperCount = 0;

	float moveTimer = 0.0f;
	float moveInterval = 0.10f;

};
