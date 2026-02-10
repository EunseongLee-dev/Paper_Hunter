#include "GameFlowManager.h"
#include "Level/PaperLevel.h"
#include "Engine/Engine.h"
#include "Core/Input.h"

#include <fstream>
#include <sstream>


GameFlowManager::GameFlowManager(PaperLevel* ownerLevel)
	:ownerLevel(ownerLevel)
{
	LoadAsciiArt("MainMenu.txt", &mainMenuAscii, &mainMenuArtSize);
	LoadAsciiArt("Menu.txt", &pauseMenuAscii, &pauseMenuSize);
	LoadAsciiArt("GameOver.txt", &gameOverAscii, &gameOverSize);
	LoadAsciiArt("GameClear.txt", &gameClearAscii, &gameClearSize);
}

void GameFlowManager::HandleInput()
{
	// 게임 일시정지 및 재시작 버튼 할당
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
		if (Input::Get().GetKeyDown(VK_UP))
		{
			/*SetGameState(GameState::Playing);
			ownerLevel->ResetLevel();*/
			mainMenuOptionIndex = (mainMenuOptionIndex - 1 + (int)mainMenuOptions.size())
				% (int)mainMenuOptions.size();
		}
		if (Input::Get().GetKeyDown(VK_DOWN))
		{
			mainMenuOptionIndex = (int)(mainMenuOptionIndex + 1) % (int)mainMenuOptions.size();
		}
		// 옵션 선택
		if (Input::Get().GetKeyDown(VK_RETURN))
		{
			if (mainMenuOptionIndex == 0) // START GAME
			{
				SetGameState(GameState::Playing);
				ownerLevel->ResetLevel(); // 게임 시작 시 레벨 리셋
			}
			else if (mainMenuOptionIndex == 1) // EXIT GAME
			{
				QuitGame();
			}
		}
		break;
		
	case GameState::Playing:
		break;

	case GameState::Paused:
		// 일시 정지 메뉴 
		if (Input::Get().GetKeyDown(VK_UP))
		{
			pauseoptionIndex = (pauseoptionIndex - 1 + (int)pauseMenuOptions.size()) % (int)pauseMenuOptions.size();
		}
		if (Input::Get().GetKeyDown(VK_DOWN))
		{
			pauseoptionIndex = (pauseoptionIndex + 1) % (int)pauseMenuOptions.size();
		}
		if (Input::Get().GetKeyDown(VK_RETURN))
		{
			if (pauseoptionIndex == 0) // Resume
			{
				SetGameState(GameState::Playing);
			}
			else if (pauseoptionIndex == 1) // Return to Main Menu
			{
				SetGameState(GameState::Playing);
				ownerLevel->ResetLevel();
			}
			else if (pauseoptionIndex == 2)
			{
				SetGameState(GameState::MainMenu);
			}
		}
		break;

	case GameState::GameOver:
	case GameState::GameClear:
		
		// 게임 오버 / 클리어 화면에서 엔터 입력 시 메인 메뉴
		if (Input::Get().GetKeyDown(VK_RETURN))
		{
			SetGameState(GameState::MainMenu);
		}
		break;
	}
}

void GameFlowManager::SetGameState(GameState newstate)
{
	currentGameState = newstate;

	// 상태 전환 시 초기화 및 정리
	if (currentGameState == GameState::Playing)
	{
		// 게임 재개 시 메시지 지우기
		DisplayMessage("", 0.0f);
	}
}

void GameFlowManager::Tick(float deltaTime)
{
	if (messageDisplayTime > 0.0f)
	{
		messageDisplayTime -= deltaTime;
		if (messageDisplayTime <= 0.0f)
		{
			currentMessage.clear();
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
	{
		DrawMenuScreen(mainMenuAscii, mainMenuArtSize,
			mainMenuOptions, mainMenuOptionIndex);
		break;
	}
	case GameState::Playing:
		
		break;

	case GameState::Paused:
	{
		DrawMenuScreen(pauseMenuAscii, pauseMenuSize, pauseMenuOptions, pauseoptionIndex);
		break;
	}
	case GameState::GameOver:
	{
		Vector2 currentConsoleScreenSize = Renderer::Get().GetScreenSize();
		DrawMenuScreen(gameOverAscii, gameOverSize, {}, 0);

		std::string restartMsg = "Press Enter to Restart";
		Vector2 msgPos;
		msgPos.x = currentConsoleScreenSize.x / 2 - (restartMsg.length() / 2);
		msgPos.y = currentConsoleScreenSize.y / 2 + (gameOverSize.y / 2) + 2;
		
		CallDrawUI(msgPos, restartMsg.c_str(), Color::DarkGray);
		break;
	}

	case GameState::GameClear:
	{
		Vector2 currentConsoleScreensize = Renderer::Get().GetScreenSize();
		DrawMenuScreen(gameClearAscii, gameClearSize, {}, 0);

		char timeBuffer[64];
		sprintf_s(timeBuffer, "Play Time: %.1f seconds", finalPlayTime);

		Vector2 timePos;
		timePos.x = currentConsoleScreensize.x / 2 - (strlen(timeBuffer) / 2);
		timePos.y = currentConsoleScreensize.y / 2 + (gameClearSize.y / 2) + 2;
		CallDrawUI(timePos, timeBuffer, Color::Green);

		std::string returnMsg = "Press Enter to MainMenu";

		Vector2 returnMsgPos;
		returnMsgPos.x = currentConsoleScreensize.x / 2 - (returnMsg.length() / 2);
		returnMsgPos.y = timePos.y + 2;
		CallDrawUI(returnMsgPos, returnMsg.c_str(), Color::DarkGray);
		break;


	}
	}

	// 메시지 표시
	if (!currentMessage.empty() && messageDisplayTime > 0.0f)
	{
		// 메시지 줄 계산
		int numLines = 0;
		for (char c : currentMessage)
		{
			if (c == '\n')
			{
				numLines++;
			}
		}
		numLines++;

		// 메시지 중앙 정렬
		Vector2 consoleScreenSize = Renderer::Get().GetScreenSize();
		Vector2 messagePos;
		messagePos.y = (consoleScreenSize.y * 2 / 3) - (numLines / 2);

		std::string line;
		std::stringstream ss(currentMessage);

		while (std::getline(ss, line, '\n'))
		{
			// 가로 중앙 정렬
			messagePos.x = consoleScreenSize.x / 2 - (line.length() / 2);
			CallDrawUI(messagePos, line.c_str(), Color::Red);
			messagePos.y++;
		}
	}
}

void GameFlowManager::DisplayMessage(const std::string& message, float duration)
{
	// 메시지 표시
	currentMessage = message;
	messageDisplayTime = duration;
}

void GameFlowManager::TriggerGameOver()
{
	// 게임 오버 메시지 출력
	SetGameState(GameState::GameOver);
}

void GameFlowManager::TriggerGameClear(float currentPlayTime)
{
	// 게임 클리어 메시지 출력
	SetGameState(GameState::GameClear);
	finalPlayTime = currentPlayTime;
}

void GameFlowManager::QuitGame()
{
	Engine::Get().QuitEngine();
}

void GameFlowManager::LoadAsciiArt(const char* filename,
	std::vector<std::string>* artVector, Vector2* artsize)
{
	if (!artVector || !artsize)
	{
		return;
	}

	artVector->clear();
	*artsize = Vector2::Zero;

	char path[2048] = {};
	sprintf_s(path, "../../../../Assets/%s", filename);
	//sprintf_s(path, "../Assets/%s", filename);

	std::ifstream file(path);
	if (!file.is_open())
	{
		// 파일 로드 실패 시 오류 처리
		DisplayMessage("ERROR: failed to load MainMenu.txt!", 5.0f);
		return;
	}

	std::string line;
	int currentLineLength = 0;
	while (std::getline(file, line))
	{
		artVector->push_back(line);
		if (line.length() > currentLineLength)
		{
			currentLineLength = line.length();
		}
	}
	file.close();

	artsize->x = currentLineLength;
	artsize->y = mainMenuAscii.size();
}

void GameFlowManager::DrawMenuScreen(const std::vector<std::string>& asciiArt, const Vector2& artSize, const std::vector<std::string>& options, int optionIndex)
{
	Vector2 consoleScreenSize = Renderer::Get().GetScreenSize();
	Vector2 artDrawStartPos;

	// 아스키아트 화면 중앙 배치
	int totalHeight = artSize.y + options.size() + 1;

	artDrawStartPos.x = consoleScreenSize.x / 2 - artSize.x / 2;
	artDrawStartPos.y = consoleScreenSize.y / 2 - totalHeight / 2;

	for (size_t i = 0; i < asciiArt.size(); ++i)
	{
		// 아스키아트 그리기
		CallDrawUI(Vector2(artDrawStartPos.x, artDrawStartPos.y + (int)i),
			asciiArt[i].c_str(), Color::LightBlue);

		// 메뉴 옵션 그리기
		int menuStartPosY = artDrawStartPos.y + artSize.y + 1;

		for (size_t i = 0; i < options.size(); ++i)
		{
			std::string option = options[i];
			Color optionColor = (i == optionIndex) ? Color::Red : Color::White;

			CallDrawUI(Vector2(consoleScreenSize.x / 2 - (option.length() / 2),
				menuStartPosY + (int)i), option.c_str(), optionColor);
		}
	}
}

void GameFlowManager::CallDrawUI(const Vector2& startPos, const char* text, Color color)
{
	if (ownerLevel)
	{
		ownerLevel->DrawUI(startPos, text, color);
	}
}

