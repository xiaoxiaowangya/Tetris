#include "Tetris.h"
#include<time.h>
#include<stdlib.h>
#include<conio.h>
#include<mmsystem.h>
#include "Block.h"
#include<fstream>
#include<iostream>

#pragma comment(lib,"winmm.lib")

#define RECORDER_FILE "recorder.txt"

//const int SPEED_NORMAL = 500;	//ms
const int SPEED_NORMAL[5] = { 500,400,300,250,100 };
const int SEEPD_QUICK = 30;		//ms

Tetris::Tetris(int rows, int cols, int left, int top, int blocksize)
{
	this->rows = rows;
	this->cols = cols;
	this->leftMargin = left;
	this->topMargin = top;
	this->blockSize = blocksize;
	for (int i = 0; i < rows; i++) {
		vector<int> mapRow;
		for (int j = 0; j < cols; j++) {
			mapRow.push_back(0);
		}
		map.push_back(mapRow);
	}

}

void Tetris::init()
{
	mciSendString("play res/bg.mp3 repeat",0,0,0);
	delay = SPEED_NORMAL[0];

	//�����������
	srand(time(NULL));
	//��Ϸ����
	initgraph(938, 896);
	//���ر���
	loadimage(&imgBg, "res/bg2.png");

	loadimage(&imgWin, "res/win.png");
	loadimage(&imgOver, "res/over.png");

	//��ʼ����Ϸ����
	char data[20][10];
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			map[i][j] = 0;
		}
	}
	//update = false;

	score = 0;
	lineCount = 0;
	level = 1;

	//��ʼ����߷�
	ifstream file(RECORDER_FILE);
	if (!file.is_open()) {
		std::cout << "�ļ���ʧ��" << endl;
		highestScore = 0;
	}
	else {
		file >> highestScore;
	}
	file.close(); //�ر��ļ�
	gameOver = false;
}

void Tetris::play()
{
	init();		//��ʼ��

	nextBlock = new Block;
	curBlock = nextBlock;
	nextBlock = new Block;


	int timer = 0;		//��ʱ��
	while (1) {
		//�����û�����
		keyEvent();

		timer += getDelay();
		if (timer > delay) {
			timer = 0;
			drop();
			update = true;		//�Ƿ���Ⱦ��Ϸ����
		}
		if (update) {		
			update = false;
			updateWindow();		//���»���

			//������Ϸ����
			clearLine();
		}

		if (gameOver) {
			saveScore();
			displayOver();
			system("pause");
			init();
		}
	}
}

void Tetris::keyEvent() {
	unsigned char ch;
	bool rotateFlag = false;
	int dx = 0;
	if (_kbhit()) {
		ch=_getch();

		//
		if (ch == 224) {
			ch = _getch();
			switch (ch)
			{
			case 72:
				rotateFlag = true;
				break;
			case 80:
				delay = SEEPD_QUICK;
				break;
			case 75:
				dx = -1;
				break;
			case 77:
				dx = 1;
				break;
			default:
				break;
			}
		}
	}
	if (rotateFlag) {
		rotate();
		update = true;
	}

	if (dx != 0) {
		moveLeftRight(dx);
		update = true;
	}
	
}

void Tetris::updateWindow()
{
	putimage(0, 0, &imgBg);	//���Ʊ���
	BeginBatchDraw();
	IMAGE** imgs = Block::getImages();

	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			if(map[i][j] == 0)continue;

			int x = j * blockSize + leftMargin;
			int y = i * blockSize + topMargin;
			putimage(x, y, imgs[map[i][j] - 1]);
		}
	}

	curBlock->draw(leftMargin, topMargin);
	nextBlock->draw(689, 150);

	drawScore();//��ӡ����

	EndBatchDraw();
}

int Tetris::getDelay()
{
	static unsigned long long lastTime = 0;
	
	unsigned long long currentTime = GetTickCount();
	if (lastTime == 0) {
		lastTime = currentTime;
		return 0;
	}
	else {
		int ret = currentTime - lastTime;
		lastTime = currentTime;
		return ret;
	}
}

void Tetris::drop()
{
	bakBlock = *curBlock;
	curBlock->drop();

	if (!curBlock->blockInMap(map)) {
		//���鵽��ײ��������������飬�̶�����
		bakBlock.solidify(map);
		delete curBlock;
		curBlock = nextBlock;
		nextBlock = new Block;
	}

	delay = SPEED_NORMAL[level-1];

	checkOver();
}

void Tetris::clearLine()
{
	int lines = 0;
	int k = rows - 1;//�洢��������
	for(int i = rows - 1; i >= 0; i--) {
		int count = 0;
		for (int j = 0; j < cols; j++) {
			if (map[i][j]) {
				count++;
			}
			map[k][j] = map[i][j];//��ɨ�ߴ�
		}

		if (count < cols) {//��δ��
			k--;
		}
		else {
			lines++;
		}
	}
	if (lines > 0){
		//����÷�
		int addScore[4] = { 10,30,60,80 };
		score += addScore[lines - 1];


		mciSendString("play res/xiaochu3.mp3", 0, 0, 0);
		update = true;

		//ÿһ�ٷ�һ������
		level = (score + 99) / 100;
		if (level > 5) {
			gameOver = true;
		}
		lineCount += lines;

	}
}

void Tetris::moveLeftRight(int offset)
{
	bakBlock = *curBlock;
	curBlock->moveLeftRight(offset);
	if (!curBlock->blockInMap(map)) {
		*curBlock = bakBlock;
	}
}

void Tetris::rotate()
{
	if (curBlock->getBlockType() == 7)
		return;
	bakBlock = *curBlock;
	curBlock->rotate();
	if (!curBlock->blockInMap(map)) {
		*curBlock = bakBlock;
	}
}

void Tetris::drawScore()
{
	char scoreText[32];
	sprintf_s(scoreText, sizeof(scoreText), "%d", score);

	setcolor(RGB(180, 180, 180));

	LOGFONT f;
	gettextstyle(&f);
	f.lfHeight = 60;
	f.lfWeight = 30;
	f.lfQuality = ANTIALIASED_QUALITY;
	strcpy_s(f.lfFaceName,sizeof(f.lfFaceName),_T("Score UI Black"));
	settextstyle(&f);

	setbkmode(TRANSPARENT);

	outtextxy(670, 727, scoreText);

	sprintf_s(scoreText, sizeof(scoreText), "%d", lineCount);
	gettextstyle(&f);
	int xPos = 224 - f.lfWeight * strlen(scoreText);
	outtextxy(xPos, 817, scoreText);

	sprintf_s(scoreText, sizeof(scoreText), "%d", level);
	outtextxy(224 - 30, 727, scoreText);

	sprintf_s(scoreText, sizeof(scoreText), "%d", highestScore);
	outtextxy(670, 817, scoreText);
}

void Tetris::checkOver()
{
	gameOver=(curBlock->blockInMap(map) == false);

}

void Tetris::saveScore()
{
	if (score > highestScore) {
		highestScore = score;
		ofstream file(RECORDER_FILE);
		file << highestScore;
		file.close();
	}
}

void Tetris::displayOver()
{
	mciSendString("stop res/bg.mp3", 0, 0, 0);

	if (level <= 5) {
		putimage(262, 361, &imgOver);
		mciSendString("play res/over.mp3", 0, 0, 0);
	}
	else {
		putimage(262, 361, &imgWin);
		mciSendString("play res/win.mp3", 0, 0, 0);
	}
}
