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

	inline int GetMapWidth() { return mapWidth; }

	inline int GetMapHeight() { return mapHeight; }

	// 맵 사이즈 반환 함수
	virtual Vector2 GetRenderSize() const override;

private:
	// 맵 로드 함수
	void LoadMap(const char* filename);

	// 플레이어 시야 제한 함수
	bool IsVisible(const Vector2& pos) const;

	// UI 출력 함수
	void DrawUI(const Vector2& startPos, const char* text);

	virtual void Tick(float deltaTime) override;

	virtual void Draw() override;
	

public:
	// PlayerMove
	bool CanMove(const Vector2& playerPosition, const Vector2& nextPosition);

	// 다음 액터 판별 함수
	MoveResult TryMove(const Vector2& nextPosition);

	// 플레이어 카메라 시점 함수
	virtual Vector2 GetCameraTarget() const override;

	// 페이퍼 갯수 반환 함수
	inline int GetTotalPaperCount() const { return totalPaperCount; }

private:
	int mapWidth;
	int mapHeight;

	Player* player = nullptr;

	// 시야 반경
	int viewRadius = 5;

	// 페이퍼 총 갯수
	int totalPaperCount = 0;

	// 플레이 타임
	float playTime = 0.0f;
};

