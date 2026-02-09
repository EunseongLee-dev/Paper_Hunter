#include "PaperLevel.h"
#include "Math\Vector2.h"
#include "Actor\Player.h"
#include "Actor/Wall.h"
#include "Actor\Ground.h"
#include "Actor/Paper.h"
#include "Actor/Goal.h"
#include "Util/Util.h"
#include "Actor/Player.h"
#include "Render/Renderer.h"
#include "Core/Input.h"

#include <iostream>
#include "GameFlowManager.h"


PaperLevel::PaperLevel()
{
	LoadMap("PaperMap.txt");
}

Vector2 PaperLevel::GetRenderSize() const
{
	return Vector2(mapWidth, mapHeight);
}

void PaperLevel::LoadMap(const char* filename)
{
	mapWidth = 0;
	mapHeight = 0;

	// 파일 로드
	char path[2048] = {};
	sprintf_s(path, "../Assets/%s", filename);

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

	// 플레이어가 모은 페이퍼 개수 확인
	int collectedPapers = player->GetPaperCount();

	// 총 페이퍼 개수와 비교
	if (player->GetPaperCount() < totalPaperCount)
	{
		std::string msg = "페이퍼를 " + std::to_string(totalPaperCount - collectedPapers)
			+ "개 더 모아야 합니다!";
		DisplayMessage(msg, 2.0f);
	}
	else if(timerStarted)
	{
		DisplayMessage("게임 클리어!", 5.0f);
		timerStarted = false;
	}
}


void PaperLevel::DisplayMessage(const std::string& message, float duration)
{
	currentMessage = message;
	messageDisplayTime = duration;
}

void PaperLevel::CheckPaperCollectionStatus(Player* player)
{
	// 타이머 시작 전 페이퍼 수집 완료 시
	if (!timerStarted && player->GetPaperCount() >= 1)
	{
		DisplayMessage("제한 시간 안에 Goal로 돌아오세요!", 5.0f);
		timerStarted = true;
		remainingTime = timeLimit;
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

	Vector2 pos = startPos;

	for (int i = 0; text[i] != '\0'; ++i)
	{
		char ch[2] = { text[i], '\0' };

		Renderer::Get().Submit(ch, pos + Renderer::Get().GetWorldOffset(),
			color, 100);

		pos.x += 1;

	}
}

void PaperLevel::Tick(float deltaTime)
{	

	if (currentGameState == GameState::Playing)
	{
		Level::Tick(deltaTime);

		// 메시지 타이머 업데이트 
		if (messageDisplayTime > 0.0f)
		{
			messageDisplayTime -= deltaTime;
			if (messageDisplayTime <= 0.0f)
			{
				currentMessage.clear();
			}
		}

		// 클리어 타이머 업데이트
		if (timerStarted)
		{
			remainingTime -= deltaTime;
			// 남은 시간이 0 이하가 되면 0 고정
			if (remainingTime < 0.0f)
			{
				remainingTime = 0.0f;
			}
		}

		// 게임 오버 조건 추가
		if (timerStarted && remainingTime <= 0.0f)
		{
			// 타이머가 0이 되면 게임 오버 처리
			DisplayMessage("시간 초과! 게임 오버!", 3.0f);

			// 타이머 작동 중지
			timerStarted = false;
		}
		playTime += deltaTime;
	}
}

void PaperLevel::Draw()
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

	char buffer[64];

	sprintf_s(buffer, "===== STATUS =====");
	DrawUI(Vector2(0, 0), buffer);

	// Paper 출력
	sprintf_s(buffer, "   Paper: %d / %d", player->GetPaperCount(), PaperLevel::GetTotalPaperCount());
	DrawUI(Vector2(0, 1), buffer);

	// Time 출력
	sprintf_s(buffer, "   Time:  %.1f", playTime);
	DrawUI(Vector2(0, 2), buffer);

	sprintf_s(buffer, "==================");
	DrawUI(Vector2(0, 3), buffer);

	if (!currentMessage.empty() && messageDisplayTime > 0.0f)
	{
		// 메시지는 화면 중앙 하단에 표시
		Vector2 consoleScreenSize = Renderer::Get().GetScreenSize();
		DrawUI(Vector2((consoleScreenSize.x / 2) - (currentMessage.length() / 2),
			consoleScreenSize.y - 2), currentMessage.c_str(), Color::Red);
	}

	if (timerStarted)
	{
		char buffer[64];
		
		// 남은 시간, 소수점 첫째 자리까지 표시
		sprintf_s(buffer, "   Time Left: %.1f", remainingTime);
		
		// 기본 UI 아래쪽에 출력
		DrawUI(Vector2(0, 4), buffer, Color::Red);
	}
}



