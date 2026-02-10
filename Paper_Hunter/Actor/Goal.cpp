#include "Goal.h"
#include "Render\Renderer.h"
#include "Actor/Actor.h"

Goal::Goal(const Vector2& pos)
	:super("GOAL", pos, Color::LightRed)
{
}

void Goal::Draw()
{
		Vector2 drawPosition = position;
	drawPosition.x -= (width / 2);
	Renderer::Get().Submit("GOAL", drawPosition, Color::LightBlue, 200);
}
