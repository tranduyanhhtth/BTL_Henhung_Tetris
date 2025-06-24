#include <gui/model/Model.hpp>
#include <gui/model/ModelListener.hpp>
#include "main.h"

Model::Model() : modelListener(0)
{
	modelTickCount = 0;
//	highestScore = 0;
	uint32_t score = 0;
	Flash_Read_Data(0x08104000, &score, 1);
	highestScore = score;
}

void Model::tick()
{
	modelTickCount++;
}

int Model::getHighestScore() const {
	return this->highestScore;
}
void Model::setHighestScore(int score){
	if(score > highestScore) highestScore = score;
}
