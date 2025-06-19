#ifndef MODEL_HPP
#define MODEL_HPP

#include <stdint.h>
class ModelListener;

class Model
{
public:
    Model();
    long modelTickCount = 0;
    void bind(ModelListener* listener)
    {
        modelListener = listener;
    }

    void tick();
    void update(int16_t x);

    int getHighestScore() const;
    void setHighestScore(int score);
    int16_t GetImageX();

    int16_t ImageX;
protected:
    ModelListener* modelListener;
    int highestScore = 0;
};

#endif // MODEL_HPP
