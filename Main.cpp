#include "Command.h"
#include "Game.h"

int main()
{
	CGame game;

	while (!game.m_ShouldQuit)
	{
		HandleCommand(game);
	}

	return 0;
}