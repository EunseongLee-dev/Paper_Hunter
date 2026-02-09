#pragma once

#include "Math/Color.h"
#include "Math/Vector2.h"
#include "Render\Renderer.h"

#include <string>
#include <vector>

using namespace Wanted;

class PaperLevel;
class Player;

enum class GameState
{
	MainMenu,
	Playing,
	Paused,
	GameOver,
	GameClear
};

class GameFlowManager
{
public:
	GameFlowManager(PaperLevel* ownerLevel);

	void Tick(float deltaTime);
	void Draw();

	// 입력 처리 함수
	void HandleInput();

	// 게임 상태 외부 전달 함수
	inline GameState GetCurrentGameState() const { return currentGameState; }

	// 게임 상태 변경 함수
	void SetGameState(GameState newstate);

	// 메시지 표시 함수
	void DisplayMessage(const std::string& message, float duration = 2.0f);

	// Gmae Over/clear 상태 함수
	void TriggerGameOver();
	void TriggerGameClear();
	//// 일시정지 메뉴
	//inline const std::vector<std::string>& GetPauseMenuOptions() const { return pauseMenuOptions; }
	//inline int OptionIndex() const { return optionIndex; }
	//void SelectPauseOption(int index);

	void QuitGame();

private:
	PaperLevel* ownerLevel;

	// 게임 시작 기본 옵션
	GameState currentGameState = GameState::MainMenu;

	// 메시지 표시 
	std::string currentMessage;
	float messageDisplayTime = 0.0f;

	// 일시정지 메뉴
	std::vector<std::string> pauseMenuOptions = { "Resume", "Exit" };
	int optionIndex = 0;

	// DrawUI 호출 헬퍼
	void CallDrawUI(const Vector2& startPos, const char* text, Color color);

private:
	//// UI 출력 함수
	//void DrawUI(const Vector2& statrPos, const char* text, Color color = Color::White);

};

