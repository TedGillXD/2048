#pragma once
#include "GameMachine.h"

const int SCREEN_WIDTH = 600;
const int SCREEN_HEIGHT = 800;
const char* WINDOW_TITLE = "2048";

const int SCORE_Y = 650;

const float backgroundVerties[] = {
	//position		//uv coordinate
	-1.f, -1.f,		0.f, 0.f,
	1.f, -1.f,		1.f, 0.f,
	1.f, 1.f,		1.f, 1.f,

	1.f, 1.f,		1.f, 1.f,
	-1.f, -1.f,		0.f, 0.f,
	-1.f, 1.f,		0.f, 1.f
};
