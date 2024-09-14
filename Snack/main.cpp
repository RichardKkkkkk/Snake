#include<iostream>
#include<easyx.h>
#include<vector>
#include<ctime>

class Sprite{
public:
	Sprite():Sprite(0,0) {};
	Sprite(int x, int y):x(x), y(y),color(RED){}
	virtual void draw()
	{
		setfillcolor(color);
		fillrectangle(x, y, x + 10, y + 10);
	}
	void moveBy(int dx, int dy) {
		x += dx;
		y += dy;
	}
	bool collision(const Sprite& spr) {
		return x == spr.x && y == spr.y;
	}
	int getX() const {
		return x;
	}
	int getY() const {
		return y;
	}
protected:
	int x;
	int y;
	COLORREF color;
};
class Snake : public Sprite {
public:
	int dir;
	Snake():Snake(0,0){}
	Snake(int x, int y):Sprite(x,y),dir(VK_RIGHT){
		nodes.push_back(Sprite(30, 10));
		nodes.push_back(Sprite(20, 10));
		nodes.push_back(Sprite(10, 10));
	}
	void draw() override {
		for (int i = 0; i < nodes.size(); i++) {
			nodes[i].draw();
		}
	}
	void bodyMove() {
		
		for (size_t i = nodes.size()-1; i>0; i--) {
			nodes[i] = nodes[i - 1];
		}
		switch (dir) {
		case VK_UP:
			nodes[0].moveBy(0,-10);
			break; 
		case VK_DOWN:
			nodes[0].moveBy(0,10);
			break;
		case VK_LEFT:
			nodes[0].moveBy(-10, 0);
			break;
		case VK_RIGHT:
			nodes[0].moveBy(10, 0);
			break;
		}
	}
	bool collision(const Sprite& spr) {
		return nodes[0].collision(spr);
	}
	bool collisionWall() {
		int x;
		int y;
		int flag = 0;
		const Sprite& head = nodes[0];

		y = 0;
		for (int i = 0; i < 64; i++) {
			x = i * 10;
			if (head.getX() == x && head.getY() == y) {
				flag = 1;
			}
		}
		y = 470;
		for (int i = 0; i < 64; i++) {
			x = i * 10;
			if (head.getX() == x && head.getY() == y) {
				flag = 1;
			}
		}
		x = 0;
		for (int i = 0; i < 48; i++) {
			y = i * 10;
			if (head.getX() == x && head.getY() == y) {
				flag = 1;
			}
		}
		x = 630;
		for (int i = 0; i < 48; i++) {
			y = i * 10;
			if (head.getX() == x && head.getY() == y) {
				flag = 1;
			}
		}
		return flag == 1;
	}
	bool collisionSelf() {
		int flag = 0;
		for (int i = 1; i < nodes.size(); i++) {
			if (nodes[0].collision(nodes[i])) {
				flag = 1;
			}
		}
		return flag == 1;
	}
	void increament() {
		nodes.push_back(Sprite());
	}
	int getLength() {
		return nodes.size();
	}
private:
	std::vector<Sprite> nodes;
};
class Food : public Sprite {
public:
	Food():Sprite(0,0){
		x = rand() % 64 *10;
		y = rand() % 48 *10;
	}
	void draw()override {
		setfillcolor(YELLOW);
		solidellipse(x, y, x + 10, y + 10);
	}
	void changePos() {
		int tem_x;
		int tem_y;
		int wallData[64][48] = { 0 };
		tem_x = rand() % 64 * 10;
		tem_y = rand() % 48 * 10;

		for (int i = 0; i < 64; ++i) {
			wallData[i][0] = 1;
		}
		for (int i = 0; i < 64; ++i) {
			wallData[i][48-1] = 1;
		}
		for (int i = 0; i < 48; i++) {
			wallData[0][i] = 1;
		}
		for (int i = 0; i < 48; i++) {
			wallData[64 - 1][i] = 1;
		}
		while (wallData[tem_x / 10][tem_y / 10] == 1) {
			x = rand() % 64 * 10;
			y = rand() % 48 * 10;
		}
		x = tem_x;
		y = tem_y;
	}

};
class Wall : public Sprite {
public:
	Wall() :Sprite(0, 0){}
	void setWall() {
		setfillcolor(LIGHTGRAY);
		int x;
		int y;
		
		y = 0;
		for (int i = 0; i < 64; i++) {
			x = i * 10;
			fillrectangle(x, y, x + 10, y + 10);
		}
		y = 470;
		for (int i = 0; i < 64; i++) {
			x = i * 10;
			fillrectangle(x, y, x + 10, y + 10);
		}
		x = 0;
		for (int i = 0; i < 48; i++) {
			y = i * 10;
			fillrectangle(x, y, x + 10, y + 10);
		}
		x = 630;
		for (int i = 0; i < 48; i++) {
			y = i * 10;
			fillrectangle(x, y, x + 10, y + 10);
		}

	}
};
class GameScene {
public:
	GameScene() {
		startTime = time(nullptr);
	};
	void run() {
		cleardevice();
		BeginBatchDraw();
		snake.draw();
		food.draw();
		snake.bodyMove();
		wall.setWall();
		snackEatFood();
		snackStrikeWall();
		snackStrikeSelf();
		displaySurviveTime();
		displayPoints();
		EndBatchDraw();
		
		ExMessage msg = { 0 };
		while (peekmessage(&msg, EX_KEY)) {
			onMsg(msg);
		}
	}
	void onMsg(const ExMessage& msg) {
		if (msg.message == WM_KEYDOWN) {
			switch (msg.vkcode) {
				case VK_UP:
					if (snake.dir != VK_DOWN)
						snake.dir = msg.vkcode;
					break;
				case VK_DOWN:
					if (snake.dir != VK_UP)
						snake.dir = msg.vkcode;
					break;
				case VK_LEFT:
					if (snake.dir != VK_RIGHT)
						snake.dir = msg.vkcode;
					break;
				case VK_RIGHT:
					if (snake.dir != VK_LEFT)
						snake.dir = msg.vkcode;
					break;
			}
		}
	}
	void snackEatFood() {
		if (snake.collision(food)) {
			snake.increament();
			food.changePos();
		}
	}
	void snackStrikeWall() {
		if (snake.collisionWall()) {
			std::cout << "撞墙啦，寄啦!";
			exit(0);
		}
	}
	void snackStrikeSelf() {
		if (snake.collisionSelf()) {
			std::cout << "撞到自己啦！";
			exit(1);
		}
	}
	void displaySurviveTime() {
		time_t currentTime = time(nullptr);
		double elapsed = difftime(currentTime, startTime);
		TCHAR timeStr[50];
		swprintf_s(timeStr, _T("你存活了:%.0f 秒"), elapsed);
		settextcolor(WHITE);
		settextstyle(20, 20, L"微软雅黑");
		outtextxy(100, 100, timeStr);
	}
	void displayPoints() {
		int length = snake.getLength() - 3;
		TCHAR lengthstr[50];
		swprintf_s(lengthstr, _T("当前得分:%.0d "), length);
		settextcolor(LIGHTMAGENTA);
		settextstyle(20, 20, L"微软雅黑");
		outtextxy(200, 200, lengthstr);
	}
	void showStartScreen() {
		cleardevice(); 
		setfillcolor(LIGHTCYAN);
		fillrectangle(0, 0, 640, 480); 
		settextcolor(BROWN); 

		settextstyle(40, 30, L"微软"); 
		wchar_t s0[] = L"欢迎使用贪吃蛇";
		outtextxy(20, 0, s0);

		settextstyle(40, 30, L"微软"); 
		TCHAR s1[] = _T("请按回车键开始游戏");
		outtextxy(20, 50, s1);

		settextstyle(40, 30, L"微软"); 
		TCHAR s2[] = _T("按ESC退出游戏");
		outtextxy(20, 90, s2);
	}
private:
	Snake snake;
	Food food;
	Wall wall;
	time_t startTime;
};

int main() {
	initgraph(640, 480);
	srand(time(nullptr));
	GameScene scene;
	scene.showStartScreen();
	ExMessage msg1;
	while (true) {
		if (peekmessage(&msg1, EX_KEY)) {
			if (msg1.message == WM_KEYDOWN) {
				if (msg1.vkcode == VK_RETURN) {
					// 按下 Enter 键，开始游戏
					break;
				}
				else if (msg1.vkcode == VK_ESCAPE) {
					// 按下 Esc 键，退出程序
					std::cout << "你真退啊？.jpg";
					exit(0);
				}
			}
		}
	}
	while (true) {
		scene.run();
		Sleep(100);
	}
	
	getchar();
	return 0;
}