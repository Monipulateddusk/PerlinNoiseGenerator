#include <iostream>
#include "MainGame.h"
#define STB_TRUETYPE_IMPLEMENTATION
#include "stb_truetype.h"

int main(int argc, char** argv) //argument used to call SDL main
{
	MainGame mainGame;
	mainGame.run();

	return 0;
}