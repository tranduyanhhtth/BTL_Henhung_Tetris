/*
 * TetrisEngine.cpp
 *
 *  Created on: Jun 3, 2025
 *      Author: admin
 */

#include "TetrisEngine.hpp"
#include "cmsis_os.h"

namespace {
    const int Tetrominoes[7][4][4] = {
        // I
        {{0,0,0,0}, {1,1,1,1}, {0,0,0,0}, {0,0,0,0}},
        // O
        {{1,1,0,0}, {1,1,0,0}, {0,0,0,0}, {0,0,0,0}},
        // T
        {{0,1,0,0}, {1,1,1,0}, {0,0,0,0}, {0,0,0,0}},
        // S
        {{0,1,1,0}, {1,1,0,0}, {0,0,0,0}, {0,0,0,0}},
        // Z
        {{1,1,0,0}, {0,1,1,0}, {0,0,0,0}, {0,0,0,0}},
        // J
        {{1,0,0,0}, {1,1,1,0}, {0,0,0,0}, {0,0,0,0}},
        // L
        {{0,0,1,0}, {1,1,1,0}, {0,0,0,0}, {0,0,0,0}},
    };
}

TetrisEngine::TetrisEngine() {
	init();
}

void TetrisEngine::init() {
	//Khởi tạo giá trị ban đầu cho grid
    for (auto& row : grid) row.fill(0);
    gameOver = false;
    score = 0;

	// Dùng cho tạo khối tiếp theo
	nextBlockId = -1;
	generateNextBlock();

    //random khối mới
    spawnBlock();
}

void TetrisEngine::generateNextBlock() {
    nextBlockId = osKernelGetTickCount() % 7;
    nextBlockSize = (nextBlockId == 0) ? 4 : 3;
    nextBlockSize = (nextBlockId == 1) ? 2 : nextBlockSize;
    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < 4; ++j)
            nextBlock[i][j] = Tetrominoes[nextBlockId][i][j];
}

void TetrisEngine::getNextBlock(BlockMatrix& block, int& size) const {
	block = nextBlock;
	size = nextBlockSize;
}

void TetrisEngine::spawnBlock() {
    if (nextBlockId == -1) generateNextBlock(); // Spawn đầu
    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < 4; ++j)
            currBlock[i][j] = nextBlock[i][j];
    blockSize = nextBlockSize;
    currX = (GRID_WIDTH - blockSize) / 2;
    currY = 0;
    generateNextBlock(); // Tạo khối tiếp theo
}


//xoay block
void TetrisEngine::rotateMatrix(BlockMatrix& mat) {
    BlockMatrix temp = {};
    for (int i = 0; i < blockSize; ++i)
        for (int j = 0; j < blockSize; ++j)
            temp[j][blockSize - 1 - i] = mat[i][j];
    mat = temp;
}

//lấy đường biên của block
void TetrisEngine::getBlockBounds(const BlockMatrix& block, int& minX, int& maxX, int& minY, int& maxY) {
    minX = blockSize; maxX = 0; minY = blockSize; maxY = 0;
    for (int i = 0; i < blockSize; ++i)
        for (int j = 0; j < blockSize; ++j)
            if (block[i][j]) {
                if (j < minX) minX = j;
                if (j > maxX) maxX = j;
                if (i < minY) minY = i;
                if (i > maxY) maxY = i;
            }
}

//kiểm tra va trạm
bool TetrisEngine::checkCollision(int newX, int newY, const BlockMatrix& block) {
    int minX, maxX, minY, maxY;
    getBlockBounds(block, minX, maxX, minY, maxY);
    for (int i = minY; i <= maxY; ++i)
        for (int j = minX; j <= maxX; ++j)
            if (block[i][j]) {
                int gx = newX + j;
                int gy = newY + i;

                //ra ngoài hoặc ô đã được đặt -> va chạm -> return true
                if (gx < 0 || gx >= GRID_WIDTH || gy < 0 || gy >= GRID_HEIGHT) return true;
                if (grid[gy][gx]) return true;
            }
    return false;
}

//cố định lại block trên lưới
void TetrisEngine::lockBlock() {
    for (int i = 0; i < blockSize; ++i)
        for (int j = 0; j < blockSize; ++j)
            if (currBlock[i][j]) {
                int gx = currX + j;
                int gy = currY + i;
                if (gy >= 0 && gy < GRID_HEIGHT && gx >= 0 && gx < GRID_WIDTH)
                    grid[gy][gx] = 1;
            }

    //xóa đường nếu full -> gen khối mới
    clearLines();
    spawnBlock();
}

//xóa line nếu full
void TetrisEngine::clearLines() {
    for (int y = GRID_HEIGHT - 1; y >= 0; --y) {
        bool full = true;
        for (int x = 0; x < GRID_WIDTH; ++x)
            if (!grid[y][x]) full = false;

        if (full) {
        	takeScore = true;
        	score++; //tăng điểm
            for (int row = y; row > 0; --row)
                grid[row] = grid[row - 1];
            grid[0].fill(0);
            ++y; // re-check this row
        }
    }
}

//check va chạm + khóa khối nếu được
void TetrisEngine::update() {
	if(!gameOver){
		if (!checkCollision(currX, currY + 1, currBlock))
			currY++;
		else{
			lockBlock();
			for(int i = 0; i < GRID_WIDTH; i++)
				if(grid[0][i]) gameOver = true;
		}
	}

}

//check gameover
bool TetrisEngine::isGameOver() {
	BlockMatrix newBlock = nextBlock;
	int newX = GRID_WIDTH / 2;
	int newY = 0;
	return checkCollision(newX, newY, newBlock); // Nếu có va chạm, game over
}

//di chuyển trái
void TetrisEngine::moveLeft() {
	if(gameOver) return;
    if (!checkCollision(currX - 1, currY, currBlock)) currX--;
}

//di chuyển phải
void TetrisEngine::moveRight() {
	if(gameOver) return;
    if (!checkCollision(currX + 1, currY, currBlock)) currX++;
}

//thả block
void TetrisEngine::drop() {
	if(gameOver) return;
    while (!checkCollision(currX, currY + 1, currBlock)) currY++;
    lockBlock();
}

//xoay block
void TetrisEngine::rotate() {
	if(gameOver) return;
    BlockMatrix temp = currBlock;
    rotateMatrix(temp);
    if (!checkCollision(currX, currY, temp))
        currBlock = temp;
}
