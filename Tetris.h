#pragma once
#include<vector>
#include<graphics.h>
#include "Block.h"
using namespace std;
class Tetris
{
public:
	//构造函数
	Tetris(int rows,int cols,int left,int top,int blocksize);
	void init();//初始化
	void play();//开始游戏

private:
	
	void keyEvent();
	void updateWindow();
	int getDelay();//延时函数
	void drop();
	void clearLine();
	void moveLeftRight(int offset);
	void rotate();//旋转

	void drawScore();//打印分数
	void checkOver();
	void saveScore();
	void displayOver();

private:
	
	int delay;
	bool update;//控制更新
	//int map[20][10];
	vector<vector<int>> map;	//0:没有方块	5:是第五种方块
	int rows;
	int cols;
	int leftMargin;
	int topMargin;
	int blockSize;
	IMAGE imgBg;

	Block* curBlock;	//当前方块
	Block* nextBlock;	//下一个方块
	Block bakBlock;		//备份方块

	int score;  //分数
	int highestScore;
	int level; //关卡
	int lineCount; //已经消除的行数
	bool gameOver;

	IMAGE imgOver;
	IMAGE imgWin;
};