#include <gui/screen1_screen/Screen1View.hpp>
#include "main.h"

Screen1View::Screen1View()
{
    DF_SendCommand(0x0F, 0x02, 0x02);
}

void Screen1View::setupScreen()
{
    Screen1ViewBase::setupScreen();
}

void Screen1View::tearDownScreen()
{
    Screen1ViewBase::tearDownScreen();
}

void Screen1View::handleTickEvent()
{
	tickCounter += 1;
	Unicode::snprintf(highestScoreBuffer, HIGHESTSCORE_SIZE, "%d", presenter->getHighestScore());
	highestScore.invalidate();
}
