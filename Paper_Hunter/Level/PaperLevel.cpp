#include "PaperLevel.h"
#include "Math\Vector2.h"
#include "Actor\Player.h"
#include "Actor/Wall.h"
#include "Actor\Ground.h"

#include <iostream>

PaperLevel::PaperLevel()
{
	LoadMap("TestMap.txt");
}

void PaperLevel::LoadMap(const char* filename)
{
	// 파일 로드
	char path[2048] = {};
	sprintf_s(path, "../Assets/%s", filename);

	// 파일 열기
	FILE* file = nullptr;
	fopen_s(&file, path, "rt");

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
		if (index >= fileSize)
		{
			break;
		}

		// char 읽기
		char mapCharacter = data[index];
		++index;

		// 개행 문자 처리
		if (mapCharacter == '\n')
		{
			// y좌표 증가, x좌표 초기화
			++position.y;
			position.x = 0;
			continue;
		}

		// 문자 처리
		switch (mapCharacter)
		{
		case '#':
			AddNewActor(new Wall(position));
			break;
			
		case '.':
			AddNewActor(new Ground(position));
			break;

		case 'P':
			AddNewActor(new Player(position, this));
			break;
		}
		// x좌표 증가 처리
		++position.x;
	}

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

	if (dynamic_cast<Wall*>(actor))
	{
		return false;
	}

	return true;
}

void PaperLevel::Draw()
{
	super::Draw();
}
