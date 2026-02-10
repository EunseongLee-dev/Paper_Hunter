#include "Paper.h"

Paper::Paper(const Vector2& position)
	:super("*", position, Color::LightBlue)
{
	// 그리기 우선순위
	sortingOrder = 5;
}
