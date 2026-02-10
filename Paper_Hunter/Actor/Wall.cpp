#include "Wall.h"

Wall::Wall(const Vector2& position)
	:super("#", position, Color::Gray)
{
	// 그리기 우선 순위
	sortingOrder = 0;
}
