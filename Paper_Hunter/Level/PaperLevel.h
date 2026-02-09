#pragma once

#include "Level/Level.h"
#include "Math/Color.h"
#include "Manager/GameFlowManager.h"

#include <string>

using namespace Wanted;

class Player;
class Goal;

enum class MoveResult
{
	Blocked,
	Moved,
	GotPaper,
	ReachedGoal
};

class PaperLevel : public Level
{
	RTTI_DECLARATIONS(PaperLevel, Level)

public:
	PaperLevel();
	~PaperLevel();

	inline int GetMapWidth() { return mapWidth; }

	inline int GetMapHeight() { return mapHeight; }

	// 맵 사이즈 반환 함수
	virtual Vector2 GetRenderSize() const override;

private:
	// 맵 로드 함수
	void LoadMap(const char* filename);

	// 플레이어 시야 제한 함수
	bool IsVisible(const Vector2& pos) const;

	virtual void Tick(float deltaTime) override;

	virtual void Draw() override;

public:
	// UI 출력 함수
	void DrawUI(const Vector2& startPos, const char* text, Color color = Color::White);

	void ResetLevel();
	

public:
	// PlayerMove
	bool CanMove(const Vector2& playerPosition, const Vector2& nextPosition);

	// 다음 액터 판별 함수
	MoveResult TryMove(const Vector2& nextPosition);

	// 플레이어 카메라 시점 함수
	virtual Vector2 GetCameraTarget() const override;

	// 페이퍼 갯수 반환 함수
	inline int GetTotalPaperCount() const { return totalPaperCount; }

	// 플레이어 GOAL 도달 판정 함수
	void HandleGoalReached(Player* player);

	// 페이터 수집 상태 확인
	void CheckPaperCollectionStatus(Player* player);

private:
	int mapWidth;
	int mapHeight;

	Player* player = nullptr;

	Goal* goal = nullptr;

	GameFlowManager* gamemanager = nullptr;

	// 시야 반경
	int viewRadius = 5;

	// 페이퍼 총 갯수
	int totalPaperCount = 0;

	// 플레이 타임
	float playTime = 0.0f;

	//Todo: 삭제 예정
	//// 클리어 조건 미충족 메세지 출력
	//std::string currentMessage;
	//float messageDisplayTime = 0.0f;

	// 클리어 조건 타이머 변수
	bool timerStarted = false;
	float remainingTime = 0.0f;
	float timeLimit = 30.0f;


};

