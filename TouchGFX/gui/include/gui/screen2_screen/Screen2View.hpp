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

    void drawGrid();
    void drawPreview();

protected:
    TetrisEngine engine;				//game engin
    BoxWithBorder colBoxes[20][10];		//lưới box hiển thị
    BoxWithBorder previewBoxes[4][4];	//next box
    int tickCount;						//biếm đếm
    bool gameOver;						//trạng thái game
};

#endif // SCREEN2VIEW_HPP
