/*开发日志
* 1.创建项目
* 2.C++开发
* 3.设计项目模块
*		Block方块类
*		Tetris游戏类
* 4.接口设计
* 6.mian函数
*/


#include"Tetris.h"
int main(void) {
	Tetris game(20, 10, 263, 133, 36);
	game.play();
	return 0;
}