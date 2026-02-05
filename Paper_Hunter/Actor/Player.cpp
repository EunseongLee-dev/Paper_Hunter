#include "Player.h"
#include "Core/Input.h"
#include "Engine/Engine.h"
#include "Level/Level.h"

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

	// Todo: ESC키 처리(Player)

	// Q키 종료
	if (Input::Get().GetKeyDown('Q'))
	{
		Engine::Get().QuitEngine();
	}

	// Todo: 박스 생성(player) 필요여부 고민중

	// 이동
	if (Input::Get().GetKeyDown(VK_RIGHT) && GetPosition().x < 20)
	{
		// 이동 가능 여부 판단
		Vector2 newPosition(GetPosition().x + 1, GetPosition().y);
		if (owner->CanMove(GetPosition(), newPosition))
		{
			SetPosition(newPosition);
		}
	}

	if (Input::Get().GetKeyDown(VK_LEFT) && GetPosition().x > 0)
	{
		// 이동 가능 여부 판단
		Vector2 newPosition(GetPosition().x - 1, GetPosition().y);
		if (owner->CanMove(GetPosition(), newPosition))
		{
			SetPosition(newPosition);
		}
	}

	if (Input::Get().GetKeyDown(VK_DOWN) && GetPosition().y < 15)
	{
		// 이동 가능 여부 판단
		Vector2 newPosition(GetPosition().x, GetPosition().y + 1);
		if (owner->CanMove(GetPosition(), newPosition))
		{
			SetPosition(newPosition);
		}
	}

	if (Input::Get().GetKeyDown(VK_UP) && GetPosition().y > 0)
	{
		// 이동 가능 여부 판단
		Vector2 newPosition(GetPosition().x, GetPosition().y - 1);
		if (owner->CanMove(GetPosition(), newPosition))
		{
			SetPosition(newPosition);
		}
	}

}

void Player::Draw()
{
	Actor::Draw();
}
