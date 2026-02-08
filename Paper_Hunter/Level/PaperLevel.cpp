#include "PaperLevel.h"
#include "Math\Vector2.h"
#include "Actor\Player.h"
#include "Actor/Wall.h"
#include "Actor\Ground.h"
#include "Actor/Paper.h"
#include "Util/Util.h"
#include "Actor/Player.h"
#include "Render/Renderer.h"

#include <iostream>

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

bool PaperLevel::IsVisible(const Vector2& pos) const
{
	if (!player)
	{
		return false;
	}

	Vector2 diff = pos - player->GetPosition();

	// 시야각 제한 사각형 or 원형
	//return (abs(diff.x) <= viewRadius && abs(diff.y) <= viewRadius);
	return (diff.x * diff.x + diff.y * diff.y <= viewRadius * viewRadius);
}

void PaperLevel::DrawUI(const Vector2& startPos, const char* text)
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

		Renderer::Get().Submit(ch, pos, Color::White, 100);

		pos.x += 1;
	}



}

void PaperLevel::Tick(float deltaTime)
{
	Level::Tick(deltaTime);

	playTime += deltaTime;
}

void PaperLevel::Draw()
{
	for (Actor* actor : actors)
	{
		if (IsVisible(actor->GetPosition()))
		{
			actor->Draw();
		}
	}

	char buffer[64];

	// Paper 출력
	sprintf_s(buffer, "Paper: %d / %d", player->GetPaperCount(), PaperLevel::GetTotalPaperCount());
	DrawUI(Vector2(0, 0), buffer);

	// Time 출력
	sprintf_s(buffer, "Time:  %.1f", playTime);
	DrawUI(Vector2(0, 1), buffer);
}



