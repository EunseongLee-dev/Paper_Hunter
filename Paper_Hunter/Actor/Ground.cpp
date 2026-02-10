#include "Ground.h"

Ground::Ground(const Vector2& position)
	:super(".", position, Color::DarkGray)
{
	// 그리기 우선순위
	sortingOrder = 0;
}
