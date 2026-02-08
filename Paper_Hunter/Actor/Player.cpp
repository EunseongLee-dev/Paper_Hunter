#include "Player.h"
#include "Core/Input.h"
#include "Engine/Engine.h"
#include "Level/Level.h"
#include "Level/PaperLevel.h"

#include <iostream>
#include <Windows.h>

Player::Player(const Vector2& position, PaperLevel* level)
	:super("P", position, Color::Red), owner(level)
{
	// 그리기 우선순위
	sortingOrder = 10;
}


void Player::BeginPlay()
{
	Actor::BeginPlay();
}

void Player::Tick(float deltaTime)
{
	super::Tick(deltaTime);

	// 플레이어 이동속도 조절
	moveTimer += deltaTime;

	if (moveTimer < moveInterval)
	{
		return;
	}

	moveTimer = 0.0f;

	// Todo: ESC키 처리(Player)

	// Q키 종료
	if (Input::Get().GetKey('Q'))
	{
		Engine::Get().QuitEngine();
	}

	// Todo: 박스 생성(player) 필요여부 고민중

	// 이동
	if (Input::Get().GetKey(VK_RIGHT))
	{
		// 이동 가능 여부 판단
		Vector2 newPosition(GetPosition().x + 1, GetPosition().y);
		MoveResult result = owner->TryMove(newPosition);
		if (result != MoveResult::Blocked)
		{
			SetPosition(newPosition);

			if (result == MoveResult::GotPaper)
			{
				++paperCount;
			}
		}
	}
	if (Input::Get().GetKey(VK_LEFT))
	{
		// 이동 가능 여부 판단
		Vector2 newPosition(GetPosition().x - 1, GetPosition().y);
		MoveResult result = owner->TryMove(newPosition);
		if (result != MoveResult::Blocked)
		{
			SetPosition(newPosition);

			if (result == MoveResult::GotPaper)
			{
				++paperCount;
			}
		}
	}

	if (Input::Get().GetKey(VK_DOWN))
	{
		// 이동 가능 여부 판단
		Vector2 newPosition(GetPosition().x, GetPosition().y + 1);
		MoveResult result = owner->TryMove(newPosition);
		if (result != MoveResult::Blocked)
		{
			SetPosition(newPosition);

			if (result == MoveResult::GotPaper)
			{
				++paperCount;
			}
		}
	}

	if (Input::Get().GetKey(VK_UP))
	{
		// 이동 가능 여부 판단
		Vector2 newPosition(GetPosition().x, GetPosition().y - 1);
		MoveResult result = owner->TryMove(newPosition);
		if (result != MoveResult::Blocked)
		{
			SetPosition(newPosition);

			if (result == MoveResult::GotPaper)
			{
				++paperCount;
			}
		}
	}
}


void Player::Draw()
{
	Actor::Draw();
}