/*
 * TetrisEngine.hpp
 *
 *  Created on: Jun 3, 2025
 *      Author: admin
 */

#ifndef APPLICATION_USER_GUI_TETRISENGINE_HPP_
#define APPLICATION_USER_GUI_TETRISENGINE_HPP_

#include <array>
#include <cstdlib>

const int GRID_WIDTH = 10;
const int GRID_HEIGHT = 20;

class TetrisEngine {
public:

	using Grid = std::array<std::array<int, GRID_WIDTH>, GRID_HEIGHT>;
	using BlockMatrix = std::array<std::array<int, 4>, 4>;

	TetrisEngine();

    void init();
    void update();
    void moveLeft();
    void moveRight();
    void moveDown();
    void rotate();
    void drop();

	void getBlockBounds(const BlockMatrix& block, int& minX, int& maxX, int& minY, int& maxY);
    const Grid& getGrid() const { return grid; }
    const BlockMatrix& getCurrentBlock() const { return currBlock; }
    int getBlockSize() const { return blockSize; }
    int getCurrX() const { return currX; }
    int getCurrY() const { return currY; }
    int getScore() const { return score; } 
    void setTakeScore(bool param) { takeScore = param; }
	bool getTakeScore() const { return takeScore; }
	void generateNextBlock();
	void getNextBlock(BlockMatrix& block, int& size) const;
	bool isGameOver();

private:
	Grid grid;
	BlockMatrix currBlock;
	int currX, currY;
	int blockSize;
	bool gameOver;
	int score;
	bool takeScore;
	int nextBlockSize;
	int nextBlockId;
	BlockMatrix nextBlock;

	void spawnBlock();
	bool checkCollision(int newX, int newY, const BlockMatrix& block);
	void lockBlock();
	void clearLines();
	void rotateMatrix(BlockMatrix& mat);
};

#endif /* APPLICATION_USER_GUI_TETRISENGINE_HPP_ */
