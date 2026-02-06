#pragma once

#include "Level/Level.h"

using namespace Wanted;

class Player;

enum class MoveResult
{
	Blocked,
	Moved,
	GotPaper
};

class PaperLevel : public Level
{
	RTTI_DECLARATIONS(PaperLevel, Level)

public:
	PaperLevel();

private:
	// 맵 로드 함수
	void LoadMap(const char* filename);

	virtual void Draw() override;


public:
	// PlayerMove
	bool CanMove(const Vector2& playerPosition, const Vector2& nextPosition);

	// 다음 액터 판별 함수
	MoveResult TryMove(const Vector2& nextPosition);

private:
	int mapWidth;
	int mapHeight;
};

