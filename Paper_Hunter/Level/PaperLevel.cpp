#include "PaperLevel.h"
#include "Math\Vector2.h"
#include "Actor\Player.h"
#include "Actor/Wall.h"
#include "Actor\Ground.h"
#include "Actor/Paper.h"
#include "Actor/Goal.h"
#include "Util/Util.h"
#include "Manager/GameFlowManager.h"
#include "Render/Renderer.h"
#include "Core/Input.h"


#include <iostream>


PaperLevel::PaperLevel()
{
	//Manager 객체 생성
	gamemanager = new GameFlowManager(this);
	LoadMap("PaperMap.txt");

	// 초기 게임 상태 설정(MainMenu)
	gamemanager->SetGameState(GameState::MainMenu);
}

PaperLevel::~PaperLevel()
{
	SafeDelete(gamemanager);
}

Vector2 PaperLevel::GetRenderSize() const
{
	return Vector2(mapWidth, mapHeight);
}

Vector2 PaperLevel::GetNearestPaperPosition(const Vector2& playerPosition) const
{
	// 최단 거리 저장
	float minDistanceSq = -1.0f;
	// 가까운 페이퍼 위치
	Vector2 nearestPaperPos = Vector2::Zero;

	for (Actor* actor : actors)
	{
		// 현재 액터가 페이퍼 인지 확인
		Paper* paper = dynamic_cast<Paper*>(actor);
		if (paper)
		{
			float distSq = (paper->GetPosition() - playerPosition).LengthSq();
			
			// 첫 번째 페이퍼 or 더 가까운 페이퍼를 찾았다면 업데이트
			if (minDistanceSq < 0 || distSq < minDistanceSq)
			{
				minDistanceSq = distSq;
				nearestPaperPos = paper->GetPosition();
			}
		}
	}
	// 가장 가까운 페이퍼의 위치 반환
	return nearestPaperPos;
}

void PaperLevel::LoadMap(const char* filename)
{
	mapWidth = 0;
	mapHeight = 0;

	// 파일 로드
	char path[2048] = {};
	sprintf_s(path, "../../../../Assets/%s", filename);
	//sprintf_s(path, "../Assets/%s", filename);

	// 파일 열기
	FILE* file = nullptr;
	fopen_s(&file, path, "rb");

	// 예외 처리
	if (!file)
	{
		// std::cerr 활용
		std::cerr << "failed to open map file.\n";

		__debugbreak();
	}

	// 맵 읽기
	// 맵 크기 파악: file Position 포인터를 파일의 끝으로 이동
	fseek(file, 0, SEEK_END);

	// 현재 위치 읽기
	size_t fileSize = ftell(file);

	// File Position 처음으로 되돌리기
	rewind(file);

	// 데이터 읽기용 버퍼
	char* data = new char[fileSize + 1];

	// 데이터 읽기
	size_t readSize = fread(data, sizeof(char), fileSize, file);

	// 인덱스로 한 문자씩 읽기
	int index = 0;

	// 객체 생성 위치 값
	Vector2 position;

	while (true)
	{
		// 종료 조건
		if (index >= readSize)
		{
			break;
		}

		// char 읽기
		char mapCharacter = data[index];
		if (mapCharacter == '/r')
		{
			continue;
		}
		++index;

		// 개행 문자 처리
		if (mapCharacter == '\n')
		{
			// y좌표 증가 및 계산, x좌표 초기화 및 계산
			if (position.x > mapWidth)
			{
				mapWidth = position.x;
			}

			++position.y;
			position.x = 0;
			continue;
		}

		// 문자 처리
		switch (mapCharacter)
		{
		case '@':
			AddNewActor(new Wall(position));
			break;
			
		case ' ':
			AddNewActor(new Ground(position));
			break;

		case 'T':
			AddNewActor(new Paper(position));
			++totalPaperCount;
			break;

		case 'P':
			Player * p = new Player(position, this);
			AddNewActor(p);
			player = p;
			break;
		}
		// x좌표 증가 처리
		++position.x;

		//Todo: 시연용 페이퍼 갯수 변경위치(완료시 삭제)
		/*totalPaperCount = 3;*/
	}
	mapHeight = position.y + 1;

	// GOAL 위치 판정 및 생성
	Vector2 goalpos((mapWidth - 1) / 2, (mapHeight - 1 ) / 2);
	goal = new Goal(goalpos);
	AddNewActor(goal);

	// 버퍼 해제
	delete[] data;

	// 파일 닫기
	fclose(file);
}

bool PaperLevel::CanMove(const Vector2& playerPosition, const Vector2& nextPosition)
{
	Actor* actor = GetActorAt(nextPosition);
	
	if (!actor)
	{
		return true;
	}

	if (dynamic_cast<Paper*>(actor))
	{
		actor->Destroy();
		return true;
	}

	if (dynamic_cast<Wall*>(actor))
	{
		return false;
	}
	return true;
}

MoveResult PaperLevel::TryMove(const Vector2& nextPosition)
{
	// Goal 위치 판정
	if (goal)
	{
		// x 시작 좌표
		int goalStartX = goal->GetPosition().x - (goal->GetWidth() / 2);

		// 문자열 좌표 판정
		if (nextPosition.y == goal->GetPosition().y && 
			nextPosition.x >= goalStartX &&
			nextPosition.x < goalStartX + goal->GetWidth())
		{
			return MoveResult::ReachedGoal;
		}
	}

	// Goal 이 아니라면, 다른 액터 확인
	Actor* actor = GetActorAt(nextPosition);

	if (!actor)
	{
		return MoveResult::Moved;
	}
	
	if (dynamic_cast<Wall*>(actor))
	{
		return MoveResult::Blocked;
	}

	if (dynamic_cast<Paper*>(actor))
	{
		actor->Destroy();
		return MoveResult::GotPaper;
	}

	return MoveResult::Moved;
}

Vector2 PaperLevel::GetCameraTarget() const
{
	if (player)
	{
		return player->GetPosition();
	}

	return Vector2(0, 0);
}

void PaperLevel::HandleGoalReached(Player* player)
{
	// 총 페이퍼 개수와 비교
	if (player->GetPaperCount() < totalPaperCount)
	{
		char buffer[64];
		sprintf_s(buffer, "Collect %d more paper(s)!", totalPaperCount - player->GetPaperCount());
		gamemanager->DisplayMessage(buffer, 2.0f);
	}
	else
	{
		// 타이머 작동 중
		gamemanager->TriggerGameClear(playTime);
	}
}

void PaperLevel::CheckPaperCollectionStatus(Player* player)
{
	// 타이머 시작 전 페이퍼 수집 완료 시
	if (!timerStarted && player->GetPaperCount() >= totalPaperCount)
	{
		float messageDuration = 3.0f;
		gamemanager->DisplayMessage("All papers collected!\nReturn to the Goal!", messageDuration);
		timerStarted = true;
		remainingTime = timeLimit;

		// 페이퍼 HUD 설정
		paperBlinkTimer = messageDuration;
	}
}

bool PaperLevel::IsVisible(const Vector2& pos) const
{
	if (!player)
	{
		return false;
	}

	Vector2 diff = pos - player->GetPosition();

	// 시야각 제한 사각형 or 원형
	/*return (abs(diff.x) <= viewRadius && abs(diff.y) <= viewRadius);*/
	return (diff.x * diff.x + diff.y * diff.y <= viewRadius * viewRadius);
}

void PaperLevel::DrawUI(const Vector2& startPos, const char* text, Color color)
{
	// 예외처리
	if (!player)
	{
		return;
	}

	Vector2 worldPosForscreenUI = startPos + Renderer::Get().GetWorldOffset();

	Renderer::Get().Submit(text, worldPosForscreenUI, color, 999);
}

void PaperLevel::ResetLevel()
{
	// 레벨 상태 초기화
	// 모든 액터 파괴 및 다시 로드
	for (Actor* actor : actors)
	{
		actor->Destroy();
	}
	actors.clear();
	player = nullptr;
	goal = nullptr;

	// 초기화 변수

	totalPaperCount = 0;
	playTime = 0.0f;
	timerStarted = false;
	remainingTime = 0.0f;
	
	LoadMap("PaperMap.txt");
	gamemanager->SetGameState(GameState::Playing);
	gamemanager->DisplayMessage("Game Start!", 1.0f);
}

void PaperLevel::Tick(float deltaTime)
{	
	// 입력 처리
	gamemanager->HandleInput();

	// 게임 상태 판별
	gamemanager->Tick(deltaTime);

	if (gamemanager->GetCurrentGameState() == GameState::Playing)
	{
		Level::Tick(deltaTime);

		// HUD 타이머
		if (gamemanager->GetCurrentGameState() == GameState::Playing)
		{
			if (paperBlinkTimer > 0.0f)
			{
				paperBlinkTimer -= deltaTime;
				if (paperBlinkTimer <= 0.0f)
				{
					paperBlinkTimer = 0.0f;
				}
			}

			// 클리어 타이머
			if (timerStarted)
			{
				remainingTime -= deltaTime;
				if (remainingTime < 0.0f)
				{
					remainingTime = 0.0f;
					// 시간 초과 시 게임 오버
					gamemanager->TriggerGameOver();
				}
			}
			playTime += deltaTime;
		}
	}
}

void PaperLevel::Draw()
{
	// Playing 상태일 때
	if (gamemanager->GetCurrentGameState() == GameState::Playing)
	{
		// 카메라 위치 설정
		Vector2 cameraPos = GetCameraTarget();
		Vector2 screenSize = Renderer::Get().GetScreenSize();

		Vector2 halfScreenSize = Vector2(screenSize.x / 2, screenSize.y / 2);

		Vector2 cameraTopLeft = cameraPos - halfScreenSize;
		Renderer::Get().SetWorldOffset(cameraTopLeft);

		for (Actor* actor : actors)
		{
			if (IsVisible(actor->GetPosition()))
			{
				actor->Draw();
			}
		}

		// Paper HUD
		// 색상 변경 및 깜빡임
		Color paperHudColor = Color::Cyan;
		if (player)
		{
			// 모든 페이퍼 수집 시
			if (player->GetPaperCount() >= totalPaperCount)
			{
				paperHudColor = Color::Green;
			}

			// 타이머 활성화 시 
			if (paperBlinkTimer > 0.0f)
			{
				if (fmod(paperBlinkTimer, paperBlinkInterval * 2) < paperBlinkInterval)
				{
					paperHudColor = Color::Yellow;
				}
			}
		}

		// STATUS UI
		char buffer[64];

		sprintf_s(buffer, "===== STATUS =====");
		DrawUI(Vector2(0, 0), buffer);

		// Time 출력
		sprintf_s(buffer, "   Time:  %.1f", playTime);
		DrawUI(Vector2(0, 1), buffer, Color::Cyan);
		
		// Paper 출력
		sprintf_s(buffer, "   Paper: %d / %d", player->GetPaperCount(), PaperLevel::GetTotalPaperCount());
		DrawUI(Vector2(0, 2), buffer, paperHudColor);

		sprintf_s(buffer, "==================");
		DrawUI(Vector2(0, 4), buffer);

		if (timerStarted)
		{
			// 기본 UI 아래쪽에 출력
			// 남은 시간, 소수점 첫째 자리까지 표시
			sprintf_s(buffer, "   Time Left: %.1f", remainingTime);
			DrawUI(Vector2(0, 3), buffer, Color::Red);
		}
	}

	// 나침반 UI
	// 모든 페이퍼를 모으지 않고, Playing 상태일 때만 표시
	if (player && player->GetPaperCount() < totalPaperCount &&
		gamemanager->GetCurrentGameState() == GameState::Playing)
	{
		Vector2 nearestPaperPos = GetNearestPaperPosition(player->GetPosition());

		// 페이퍼 존재여부
		if (nearestPaperPos != Vector2::Zero)
		{
			// 플레이어 기준 페이퍼의 방향
			Vector2 direction = nearestPaperPos - player->GetPosition();

			char compassArrow = ' ';
			if (abs(direction.x) > abs(direction.y))
			{
				if (direction.x > 0)
				{
					compassArrow = '>';
				}
				else
				{
					compassArrow = '<';
				}
			}
			else
			{
				if (direction.y > 0)
				{
					compassArrow = 'v';
				}
				else
				{
					compassArrow = '^';
				}
			}
			if (compassArrow != ' ')
			{
				std::string compassText = "  Nearest Paper: ";
				compassText += compassArrow;
				// 나침반 출력 위치
				DrawUI(Vector2(0, 3), compassText.c_str(), Color::BrightYellow);
			}
		}
	}
	gamemanager->Draw();
}



