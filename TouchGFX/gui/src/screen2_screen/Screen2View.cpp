#include <gui/screen2_screen/Screen2View.hpp>
#include <touchgfx/Color.hpp>
#include "cmsis_os.h"
#include "main.h"

extern osMessageQueueId_t movingQueueHandle;
extern osMessageQueueId_t speedQueueHandle;
//extern void DoubleBeepTask(void *param);

Screen2View::Screen2View()
{
	//khởi tạo lưới
    for (int y = 0; y < GRID_HEIGHT; y++) {
        for (int x = 0; x < GRID_WIDTH; x++) {
            int px = x * 16;
            int py = y * 16;
            colBoxes[y][x].setBorderColor(Color::getColorFromRGB(0, 0, 0));
            colBoxes[y][x].setBorderSize(1);
            colBoxes[y][x].setPosition(px, py, 16, 16);
            colBoxes[y][x].setVisible(true);
            add(colBoxes[y][x]);
        }
    }

    //khởi tạo lưới để hiện thị next block
	int previewX = 200;
	int previewY = 90;
	for (int y = 0; y < 4; y++) {
		for (int x = 0; x < 4; x++) {
			int px = previewX + x * 8;
			int py = previewY + y * 8;
			previewBoxes[y][x].setAlpha(0);
			previewBoxes[y][x].setBorderSize(1);
			previewBoxes[y][x].setPosition(px, py, 8, 8);
			previewBoxes[y][x].setVisible(true);
			add(previewBoxes[y][x]);
		}
	}

	track1.setVisible(false);
    gameOver = false;
    DF_SendCommand(0x0F, 0x02, 0x01);

    while(osMessageQueueGetCount(movingQueueHandle) > 0){
    	char res = 'a';
    	osMessageQueueGet(movingQueueHandle, &res, NULL, 10);
    }

}

void Screen2View::setupScreen()
{
	engine.init();
	tickCount = 0;
	track1.setVisible(false);
	track1.invalidate();
}

void Screen2View::tearDownScreen()
{
    Screen2ViewBase::tearDownScreen();
//    presenter->UpdateImageX(localImageX);
}

void Screen2View::handleTickEvent()
{
    if (++tickCount % 20 == 0) {
		if(engine.isGameOver()) {	//kiểm tra gameover

			//set các ô là ko hiển thị nếu gameover
			for(int y = 0; y < GRID_HEIGHT; y++) {
				for(int x = 0; x < GRID_WIDTH; x++) {
					colBoxes[y][x].setVisible(false);
					colBoxes[y][x].invalidate();
				}
			}
			for (int y = 0; y < 4; y++) {
				for (int x = 0; x < 4; x++) {
					previewBoxes[y][x].setVisible(false);
					previewBoxes[y][x].invalidate();
				}
			}
			textArea1.setVisible(false);
			textArea2.setVisible(false);
			score.setVisible(false);
			track1.setVisible(true);
			track1.invalidate();
			presenter->setHighestScore(engine.getScore());
			return;
		}


    	if(osMessageQueueGetCount(movingQueueHandle) > 0){
    		char res;
    		osMessageQueueGet(movingQueueHandle, &res, NULL, 10);
    		if(res == 'L') engine.moveLeft();
    		else if(res == 'R') engine.moveRight();
    		else if(res == 'T') engine.rotate();
    		else if(res == 'D') engine.drop();
    		osThreadNew(SingleBeepTask, NULL, NULL);
    	}
        engine.update();
        Unicode::snprintf(scoreBuffer, SCORE_SIZE, "%d", engine.getScore());
        score.invalidate();
        presenter->setHighestScore(engine.getScore());
        if(engine.getTakeScore()){
        	osThreadNew(DoubleBeepTask, NULL, NULL);
        	engine.setTakeScore(false);
        }
        if(engine.isGameOver() && gameOver == false){
        	gameOver = true;
        	osThreadNew(GameOverTask, NULL, NULL);
        }
        drawGrid();
        drawPreview();
    }
}

void Screen2View::drawGrid(){
    const auto& grid = engine.getGrid();
    const auto& block = engine.getCurrentBlock();
    int currX = engine.getCurrX();
    int currY = engine.getCurrY();

    //Vẽ lưới
    for (int y = 0; y < GRID_HEIGHT; ++y) {
        for (int x = 0; x < GRID_WIDTH; ++x) {
            colBoxes[y][x].setColor(grid[y][x] ? Color::getColorFromRGB(255, 0, 255)
                                               : Color::getColorFromRGB(0, 0, 0));
            colBoxes[y][x].invalidate();
        }
    }

    // Vẽ block rơi
    int minX, maxX, minY, maxY;
    engine.getBlockBounds(block, minX, maxX, minY, maxY);
    for (int i = minY; i <= maxY; ++i)
        for (int j = minX; j <= maxX; ++j)
            if (block[i][j]) {
                int gx = currX + j;
                int gy = currY + i;
                if (gx >= 0 && gx < GRID_WIDTH && gy >= 0 && gy < GRID_HEIGHT) {
                    colBoxes[gy][gx].setColor(Color::getColorFromRGB(255, 255, 0));
                    colBoxes[gy][gx].invalidate();
                }
            }
}

void Screen2View::drawPreview() {
    // Lấy khối tiếp theo
    TetrisEngine::BlockMatrix nextBlock;
    int nextBlockSize;
    engine.getNextBlock(nextBlock, nextBlockSize);

    // Reset all previewBoxes to transparent state
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			previewBoxes[i][j].setAlpha(0);
			previewBoxes[i][j].invalidate();
		}
	}

    // Vẽ preview block
	int minX, maxX, minY, maxY;
	engine.getBlockBounds(nextBlock, minX, maxX, minY, maxY);

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (nextBlock[i][j]) {
				int px = j - minX;
				int py = i - minY;
				if (px < 4 && py < 4) {
					previewBoxes[py][px].setColor(Color::getColorFromRGB(0, 255, 0));
					previewBoxes[py][px].setAlpha(255);
					previewBoxes[py][px].invalidate();
				}
			}
		}
	}
}
