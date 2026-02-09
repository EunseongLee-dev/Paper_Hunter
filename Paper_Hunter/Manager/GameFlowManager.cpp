#include "GameFlowManager.h"
#include "Level/PaperLevel.h"
#include "Engine/Engine.h"
#include "Core/Input.h"


GameFlowManager::GameFlowManager(PaperLevel* ownerLevel)
	:ownerLevel(ownerLevel)
{
}

void GameFlowManager::HandleInput()
{
	if (Input::Get().GetKeyDown(VK_ESCAPE))
	{
		if (currentGameState == GameState::Playing)
		{
			SetGameState(GameState::Paused);
		}
		else if (currentGameState == GameState::Paused)
		{
			SetGameState(GameState::Playing);
		}
	}

	switch (currentGameState)
	{
	case GameState::MainMenu:
		if (Input::Get().GetKeyDown(VK_RETURN))
		{
			SetGameState(GameState::Playing);
			/*ownerLevel->ResetLevel();*/
		}
		break;
		
	case GameState::Playing:
		break;

	case GameState::Paused:
		// 일시 정지 메뉴 
		if (Input::Get().GetKeyDown(VK_UP))
		{
			optionIndex = (optionIndex - 1 + pauseMenuOptions.size()) % pauseMenuOptions.size();
		}
		if (Input::Get().GetKeyDown(VK_DOWN))
		{
			optionIndex = (optionIndex + 1) % pauseMenuOptions.size();
		}
		if (Input::Get().GetKeyDown(VK_RETURN))
		{
			if (optionIndex == 0)
			{
				SetGameState(GameState::Playing);
			}
			else if (optionIndex == 1)
			{
				QuitGame();
			}
		}
		break;

	case GameState::GameOver:
	case GameState::GameClear:
		
		// 게임 오버 / 클리어 화면에서 엔터 입력 시 메인 메뉴
		if (Input::Get().GetKeyDown(VK_RETURN))
		{
			SetGameState(GameState::MainMenu);
			/*ownerLevel->ResetLevel();*/
		}
		break;
	}
}

void GameFlowManager::SetGameState(GameState newstate)
{
}

void GameFlowManager::Tick(float deltaTime)
{
	// 게임 일시정지 및 재시작 버튼 할당
	if (Input::Get().GetKeyDown(VK_ESCAPE))
	{
		if (currentGameState == GameState::Playing)
		{
			currentGameState = GameState::Paused;
		}
		else if (currentGameState == GameState::Paused)
		{
			currentGameState = GameState::Playing;
		}
	}

	switch (currentGameState)
	{
	case GameState::MainMenu:
		// Main 상태
		break;
	case GameState::Playing:
		// Playing 상태 로직
		break;
	case GameState::Paused:
		// 플레이 타이머 정지
		break;
	case GameState::GameOver:
		// GameOver 상태 로직
		break;
	case GameState::GameClear:
		// Clear 상태 로직
		break;
	}
}

void GameFlowManager::Draw()
{
	switch (currentGameState)
	{
	case GameState::MainMenu:
		CallDrawUI(Vector2(10, 5), "--- Main Menu ---", Color::Blue);
		CallDrawUI(Vector2(10, 7), "Press Enter to start", Color::White);
		break;

	case GameState::Playing:
		
		break;

	case GameState::Paused:
	{
		Vector2 consoleScreenSize = Renderer::Get().GetScreenSize();
		std::string pauseTitle = "PAUSED";

		CallDrawUI(Vector2(consoleScreenSize.x / 2 - (pauseTitle.length() / 2),
			consoleScreenSize.y / 2 - 3), pauseTitle.c_str(), Color::White);

		for (size_t i = 0; i < pauseMenuOptions.size(); ++i)
		{
			std::string option = pauseMenuOptions[i];
			Color optionColor = (i == optionIndex) ? Color::Red : Color::White;

			CallDrawUI(Vector2(consoleScreenSize.x / 2 - (option.length() / 2),
				consoleScreenSize.y / 2 + (int)i), option.c_str(), optionColor);
		}
	}
	break;

	case GameState::GameOver:
		CallDrawUI(Vector2(10, 5), "--- GAME OVER ---", Color::Red);
		CallDrawUI(Vector2(10, 7), "Press Enter to Restart", Color::White);
		break;

	case GameState::GameClear:
		CallDrawUI(Vector2(10, 5), "--- GAME CLEAR ---", Color::Green);
		CallDrawUI(Vector2(10, 7), "Press Enter to MainMenu", Color::White);
		break;
	}

	// 메시지 표시
	if (!currentMessage.empty() && messageDisplayTime > 0.0f)
	{
		Vector2 consoleScreenSize = Renderer::Get().GetScreenSize();
		CallDrawUI(Vector2(consoleScreenSize.x / 2 - (currentMessage.length() / 2),
			1), currentMessage.c_str(), Color::Red);
	}
}

void GameFlowManager::DisplayMessage(const std::string& message, float duration = 2.0f)
{

}


void GameFlowManager::SelectPauseOption(int index)
{
}

void GameFlowManager::QuitGame()
{

}

void GameFlowManager::DrawUI(const Vector2& statrPos, const char* text, Color color = Color::White)
{

}
