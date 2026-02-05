#include <iostream>
#include "Engine/Engine.h"
#include "Level/PaperLevel.h"

int main()
{
	Engine engine;
	engine.SetNewLevel(new PaperLevel());
	engine.Run();
}