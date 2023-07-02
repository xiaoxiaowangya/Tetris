#pragma once
#include<vector>
#include<graphics.h>
#include "Block.h"
using namespace std;
class Tetris
{
public:
	//���캯��
	Tetris(int rows,int cols,int left,int top,int blocksize);
	void init();//��ʼ��
	void play();//��ʼ��Ϸ

private:
	
	void keyEvent();
	void updateWindow();
	int getDelay();//��ʱ����
	void drop();
	void clearLine();
	void moveLeftRight(int offset);
	void rotate();//��ת

	void drawScore();//��ӡ����
	void checkOver();
	void saveScore();
	void displayOver();

private:
	
	int delay;
	bool update;//���Ƹ���
	//int map[20][10];
	vector<vector<int>> map;	//0:û�з���	5:�ǵ����ַ���
	int rows;
	int cols;
	int leftMargin;
	int topMargin;
	int blockSize;
	IMAGE imgBg;

	Block* curBlock;	//��ǰ����
	Block* nextBlock;	//��һ������
	Block bakBlock;		//���ݷ���

	int score;  //����
	int highestScore;
	int level; //�ؿ�
	int lineCount; //�Ѿ�����������
	bool gameOver;

	IMAGE imgOver;
	IMAGE imgWin;
};