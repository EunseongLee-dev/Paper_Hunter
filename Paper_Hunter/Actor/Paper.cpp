#include "Paper.h"

Paper::Paper(const Vector2& position)
	:super("T", position, Color::Green)
{
	// 그리기 우선순위
	sortingOrder = 5;
}
