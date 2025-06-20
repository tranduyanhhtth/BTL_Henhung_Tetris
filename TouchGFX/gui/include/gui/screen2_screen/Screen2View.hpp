#ifndef SCREEN2VIEW_HPP
#define SCREEN2VIEW_HPP

#include <gui_generated/screen2_screen/Screen2ViewBase.hpp>
#include <gui/screen2_screen/Screen2Presenter.hpp>
#include "TetrisEngine.hpp"

class Screen2View : public Screen2ViewBase
{
public:
    Screen2View();
    virtual ~Screen2View() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
    virtual void handleTickEvent();

    void drawGrid();	//vẽ lưới chính và block đang rơi
    void drawPreview();	//vẽ pre block

protected:
    TetrisEngine engine;				//game engine
    BoxWithBorder colBoxes[20][10];		//lưới box chính hiển thị
    BoxWithBorder previewBoxes[4][4];	//next box
    int tickCount;						//biếm đếm
    bool musicGameOver;					//trạng thái music game over
};

#endif // SCREEN2VIEW_HPP
