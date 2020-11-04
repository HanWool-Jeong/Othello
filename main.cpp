#include <bangtal>
#include <string>
using namespace bangtal;
using namespace std;

ObjectPtr board[8][8];

// �� ������ ǥ���ϴ� �� ��ü
// 10Label�� 10���ڸ�, 1Label�� 1�� �ڸ��� ǥ��
ObjectPtr black10Label, black1Label;
ObjectPtr white10Label, white1Label;

enum class State {
	BLANK,
	POSSIBLE,
	BLACK,
	WHITE
};
State board_state[8][8];

enum class Turn {
	BLACK,
	WHITE
};
Turn turn = Turn::BLACK;

void setState(int x, int y, State state)
{
	switch (state) {
		case State::BLANK: board[y][x]->setImage("images/blank.png"); break;
		case State::POSSIBLE: board[y][x]->setImage(turn == Turn::BLACK ? "images/black possible.png" : "images/white possible.png"); break;
		case State::BLACK: board[y][x]->setImage("images/black.png"); break;
		case State::WHITE: board[y][x]->setImage("images/white.png"); break;
	}
	board_state[y][x] = state;
}

bool checkPossible(int x, int y, int dx, int dy)
{
	State self = turn == Turn::BLACK ? State::BLACK : State::WHITE;
	State other = turn == Turn::BLACK ? State::WHITE : State::BLACK;

	bool possible = false;
	for (x += dx, y += dy; x >= 0 && x < 8 && y >= 0 && y < 8; x += dx, y += dy) {
		if (board_state[y][x] == other) {
			possible = true;
		}
		else if (board_state[y][x] == self) {
			return possible;
		}
		else {
			return false;
		}
	}

	return false;
}

bool checkPossible(int x, int y)
{
	if (board_state[y][x] == State::BLACK) return false;
	if (board_state[y][x] == State::WHITE) return false;
	setState(x, y, State::BLANK);

	int delta[8][2] = {
		{ 0, 1 },
		{ 1, 1 },
		{ 1, 0 },
		{ 1, -1 },
		{ 0, -1 },
		{ -1, -1 },
		{ -1, 0 },
		{ -1, 1 },
	};

	bool possible = false;
	for (auto d : delta) {
		if (checkPossible(x, y, d[0], d[1])) possible = true;
	}

	return possible;
}

bool setPossible()
{
	// 8x8�� ��� ��ġ�� ���ؼ� 
	// ������ �� ������ ���¸� POSSIBLE�� �ٲ۴�.
	bool possible = false;
	for (int y = 0; y < 8; y++) {
		for (int x = 0; x < 8; x++) {
			if (checkPossible(x, y)) {
				setState(x, y, State::POSSIBLE);
				possible = true;
			}
		}
	}
	return possible;
}

void reverse(int x, int y, int dx, int dy)
{
	State self = turn == Turn::BLACK ? State::BLACK : State::WHITE;
	State other = turn == Turn::BLACK ? State::WHITE : State::BLACK;

	bool possible = false;
	for (x += dx, y += dy; x >= 0 && x < 8 && y >= 0 && y < 8; x += dx, y += dy) {
		if (board_state[y][x] == other) {
			possible = true;
		}
		else if (board_state[y][x] == self) {
			if (possible) {
				for (x -= dx, y -= dy; x >= 0 && x < 8 && y >= 0 && y < 8; x -= dx, y -= dy) {
					if (board_state[y][x] == other) {
						setState(x, y, self);
					}
					else {
						return;
					}
				}
			}
		}
		else {
			return;
		}
	}
}

void reverse(int x, int y)
{
	int delta[8][2] = {
		{ 0, 1 },
		{ 1, 1 },
		{ 1, 0 },
		{ 1, -1 },
		{ 0, -1 },
		{ -1, -1 },
		{ -1, 0 },
		{ -1, 1 },
	};

	for (auto d : delta) {
		reverse(x, y, d[0], d[1]);
	}
}

// �� ���� ���� �����ִ� �Լ�
// 10Label, 1Label ��ü�� �ٲ㼭 �� ������ ǥ���Ѵ�
void setStoneNumber()
{
	int blackNum = 0, whiteNum = 0;

	// �浹�� �鵹�� ������ ����.
	for (int y = 0; y < 8; y++) {
		for (int x = 0; x < 8; x++) {
			if (board_state[y][x] == State::BLACK)
				blackNum++;
			else if (board_state[y][x] == State::WHITE)
				whiteNum++;
		}
	}
	
	// ������ �� ���� �ٲٱ�
	// ������ �� ������ 10 �̻��̸� ���� 10�ڸ� ���� ����� �ش�.
	int black10 = blackNum / 10;
	string img;

	if (!black10 == 0) {
		img = "images/L" + to_string(black10) + ".png";
		black10Label->setImage(img);
	}
	else {
		black10Label->setImage("images/blank.png");
	}
	
	blackNum = blackNum - black10 * 10;
	img = "images/L" + to_string(blackNum) + ".png";
	black1Label->setImage(img);

	// �Ͼ�� �� ���� �ٲٱ�
	int white10 = whiteNum / 10;
	
	if (!white10 == 0) {
		img = "images/L" + to_string(white10) + ".png";
		white10Label->setImage(img);

		img = "images/L" + to_string(whiteNum - white10 * 10) + ".png";
		white1Label->setImage(img);
	}
	// �鵹 ������ 10 �̸��� ��, ������ ���� ������ ���� 10Label���� 1�� �ڸ��� ǥ���Ѵ�.
	else {
		img = "images/L" + to_string(whiteNum) + ".png";
		white10Label->setImage(img);
		white1Label->setImage("images/blank.png");
	}
}

// ��ǻ�Ͱ� ���� �� �ִ� ���� ������ ����2
int checkComStoneGet(int x, int y, int dx, int dy)
{
	State com  = State::WHITE;		// ��ǻ��

	int possible = 0;
	for (x += dx, y += dy; x >= 0 && x < 8 && y >= 0 && y < 8; x += dx, y += dy) {
		if (board_state[y][x] == com) {
			possible++;
		}
		else {
			return possible;
		}
	}
	return possible;
}

// ��ǻ�Ͱ� ���� �� �ִ� ���� ������ ����1
int checkComStoneGet(int x, int y)
{
	int delta[8][2] = {
		{ 0, 1 },
		{ 1, 1 },
		{ 1, 0 },
		{ 1, -1 },
		{ 0, -1 },
		{ -1, -1 },
		{ -1, 0 },
		{ -1, 1 },
	};

	int sum_of_possible_stone = 0;
	for (auto d : delta) {
		// ��� 8������ üũ�ؼ� ���� �� �ִ� ���� �ջ��Ѵ�
		sum_of_possible_stone += checkComStoneGet(x, y, d[0], d[1]);
	}

	return sum_of_possible_stone;
}

// ��ǻ�Ͱ� �鵹�� ������ �ϴ� �Լ�
void computerPutWhite()
{
	// � x, y�� ������ ��, �󸶸�ŭ ���� ���� �� �ִ����� �����ϴ� ����ü
	struct LargestStone {
		int x;
		int y;
		int num_of_stone;
	};
	LargestStone lsAry[64];
	int lsAry_index = 0;

	// ��� 8x8�� ���ؿ� white possible�� ã�´�.
	for (int y = 0; y < 8; y++) {
		for (int x = 0; x < 8; x++) {
			if (board_state[y][x] == State::POSSIBLE) {
				// ã�� Possible�� ������ �� ��� ���� ���� �� �ִ����� üũ, lsAry�� ������ �д�.
				lsAry[lsAry_index].num_of_stone = checkComStoneGet(x, y);
				lsAry[lsAry_index].x = x;
				lsAry[lsAry_index].y = y;
				lsAry_index++;
			}
		}
	}
	lsAry_index--;

	// ��� Possible�� ���Ͽ� �װ��� ������ �� ���� ���� ���� �� �ִ� ���� ã�´�(linear search).
	LargestStone max = lsAry[0];
	for (int i = 1; i < lsAry_index; i++) {
		if (lsAry[i].num_of_stone > max.num_of_stone)
			max = lsAry[i];
	}

	// ���� ���� ���� �� �ִ� ��ǥ�� ���´�.
	setState(max.x, max.y, State::WHITE);
	reverse(max.x, max.y);
}

int main(void)
{
	setGameOption(GameOption::GAME_OPTION_INVENTORY_BUTTON, false);
	setGameOption(GameOption::GAME_OPTION_MESSAGE_BOX_BUTTON, false);
	setGameOption(GameOption::GAME_OPTION_ROOM_TITLE, false);

	auto scene = Scene::create("������", "images/background.png");
	black10Label = Object::create("images/blank.png", scene, 750, 280);
	black1Label  = Object::create("images/L2.png", scene, 820, 220);
	white10Label = Object::create("images/L2.png", scene, 1075, 220);
	white1Label  = Object::create("images/blank.png", scene, 1140, 280);
	auto com_turn = Object::create("images/com_turn.png", scene, 720, 40, false);

	for (int y = 0; y < 8; y++) {
		for (int x = 0; x < 8; x++) {
			board[y][x] = Object::create("images/blank.png", scene, 40 + x * 80, 40 + y * 80);
			board[y][x]->setOnMouseCallback([&, x, y](ObjectPtr object, int, int, MouseAction action)->bool {
				if (board_state[y][x] == State::POSSIBLE) {
					if (turn == Turn::BLACK) {
						setState(x, y, State::BLACK);
						reverse(x, y);
						turn = Turn::WHITE;
						setStoneNumber();

						if (!setPossible()) {
							turn = turn == Turn::BLACK ? Turn::WHITE : Turn::BLACK;	// toggle

							if (!setPossible()) {
								showMessage("End Game!!!");
							}
						}
						com_turn->show();

						// ��ǻ�Ͱ� �ٷ� ������ �̻��ϴϱ� 3�� Ÿ�̸Ӹ� �д�.
						auto timer = Timer::create(3);
						timer->start();
						timer->setOnTimerCallback([&](TimerPtr timer)-> bool {

							computerPutWhite();
							turn = Turn::BLACK;
							setStoneNumber();

							if (!setPossible()) {
								turn = turn == Turn::BLACK ? Turn::WHITE : Turn::BLACK;	// toggle

								if (!setPossible()) {
									showMessage("End Game!!!");
								}
							}

							timer->set(3);
							com_turn->hide();
							return true;
						});		
					}
				}
				return true;
			});
			board_state[y][x] = State::BLANK;
		}
	}

	// ���� �ʱ�ȭ
	setState(3, 3, State::BLACK);
	setState(4, 4, State::BLACK);
	setState(3, 4, State::WHITE);
	setState(4, 3, State::WHITE);
	setPossible();

	startGame(scene);
	return 0;
}