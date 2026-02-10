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
	void TriggerGameClear(float currentPlayTime);

	void QuitGame();

private:
	// 메뉴 로드 함수 
	void LoadAsciiArt(const char* filename, std::vector<std::string>* artVector,
		Vector2* artsize);

	// 메뉴 옵션 그리기 헬퍼 함수
	void DrawMenuScreen(
		const std::vector<std::string>& asciiArt,
		const Vector2& artSize,
		const std::vector<std::string>& options,
		int optionIndex
	);

	// DrawUI 호출 헬퍼
	void CallDrawUI(const Vector2& startPos, const char* text, Color color);

private:
	// 게임 시작 기본 옵션
	PaperLevel* ownerLevel;
	GameState currentGameState = GameState::MainMenu;

	// 메시지 표시 
	std::string currentMessage;
	float messageDisplayTime = 0.0f;

	// 메인 메뉴 변수
	std::vector<std::string> mainMenuOptions = { "START GAME", "EXIT GAME" };
	std::vector<std::string> mainMenuAscii;
	Vector2 mainMenuArtSize;
	int mainMenuOptionIndex = 0;

	// 일시정지 메뉴 변수
	std::vector<std::string> pauseMenuOptions = { "Resume", "Restart", "Return to main Menu" };
	std::vector<std::string> pauseMenuAscii;
	Vector2 pauseMenuSize;
	int pauseoptionIndex = 0;

	// 게임 오버 / 클리어 변수
	std::vector<std::string> gameOverAscii;
	Vector2 gameOverSize;
	std::vector<std::string> gameClearAscii;
	Vector2 gameClearSize;
	float finalPlayTime = 0.0f;
};

