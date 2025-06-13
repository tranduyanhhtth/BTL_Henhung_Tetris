#include <gui/model/Model.hpp>
#include <gui/model/ModelListener.hpp>

Model::Model() : ImageX(95), modelListener(0)
{
	modelTickCount = 0;
	highestScore = 0;
}

void Model::tick()
{
	modelTickCount++;
}

void Model::update(int16_t x)
{
	ImageX = x;
}

int16_t Model::GetImageX()
{
	return ImageX;
}

int Model::getHighestScore() const {
	return this->highestScore;
}
void Model::setHighestScore(int score){
	if(score > highestScore) highestScore = score;
}
