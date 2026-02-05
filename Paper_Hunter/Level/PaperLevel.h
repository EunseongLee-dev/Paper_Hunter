#pragma once

#include "Level/Level.h"

using namespace Wanted;

class PaperLevel : public Level
{
	RTTI_DECLARATIONS(PaperLevel, Level)

public:
	PaperLevel();

private:
	// 맵 로드 함수
	void LoadMap(const char* filename);

	// 이벤트 함수
	virtual void Draw() override;

public:
	// PlayerMove
	bool CanMove(const Vector2& playerPosition, const Vector2& nextPosition);

};

