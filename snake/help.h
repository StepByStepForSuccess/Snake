#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>

#include <ctime>

using namespace std;
using namespace sf;

class Cell {
public:
	RectangleShape main, border;
	float size = 25;
	Vector2f direct;

	Cell(float _size, float proc) {
		size = _size;

		border.setSize(Vector2f(_size, _size));
		Color border_color(7, 2, 171);
		border.setFillColor(border_color);

		float size_main = _size * proc;
		main.setSize(Vector2f(size_main, size_main));
		Color main_color(94, 89, 255);
		main.setFillColor(main_color);
		main.setPosition(Vector2f((_size - size_main) / 2, (_size - size_main) / 2));
	}

	void DrawCell(RenderWindow& win) {
		win.draw(border);
		win.draw(main);
	}

	void TranslateCell(Vector2f v2) {
		border.move(v2);
		main.move(v2);
	}

	//RectangleShape returnMain() { return main; }
	//RectangleShape returnBorder() { return border; }
};

class Apple {
public:
	vector <Cell> cells; // copy array from snake to spawn apple

	Vector2f pos;
	RectangleShape main, border;
	float size = 25, proc;

	Apple(float _size, float _proc) {
		size = _size;
		proc = _proc;

		border.setSize(Vector2f(_size, _size));
		Color border_color(255, 0, 0);
		border.setFillColor(border_color);

		float size_main = _size * _proc;
		main.setSize(Vector2f(size_main, size_main));
		Color main_color(245, 137, 5);
		main.setFillColor(main_color);
		main.setPosition(Vector2f((_size - size_main) / 2, (_size - size_main) / 2));
		Spawn();
	}

	void SetPosMainPart() { // ����� ����� �� ��������� ��� ������� ���� ���������...
		float size_main = size * proc;
		main.setPosition(Vector2f((size - size_main) / 2, (size - size_main) / 2));
	}

	bool canSpawn(Vector2f pos) {
		for (int i = 0; i < cells.size(); i++) {
			if (pos == cells[i].border.getPosition()) return false;
		}
		return true;
	}

	void Spawn() {
		
		srand(time(0));
		int x = (rand() % 18 + 2) * 25, y = (rand() % 18 + 2) * 25;
		while (!canSpawn(Vector2f(x, y))) {
			srand(time(0));
			x = (rand() % 18 + 2) * 25, y = (rand() % 18 + 2) * 25;
			cout << "Try to spawn... X = " << x << " Y = " << y << endl;
		}
		cout << "Respawn Apple! New points are: x = " << x << " y = " << y << endl;
		Vector2f _pos = Vector2f(x, y);
		pos = _pos;
		SetPosMainPart(); // ��� ���� ����� mainPart ������ �� ������
		main.move(_pos);
		border.setPosition(_pos);

	}

	void DrawApple(RenderWindow& win) {
		win.draw(border);
		win.draw(main);
	}
};

class Timer {
public:
	Time time;
	Clock clock;
	float seconds = 0, per;

	Timer(float _per) {
		per = _per;
	}

	void Update() {
		time = clock.restart();
		seconds += time.asSeconds();
	}

	bool Check() {
		if (seconds >= per) {
			seconds = 0;
			return true;
		}
		else return false;
	}
};

class Snake {
public:
	vector<Cell> cells;
	bool Alive = true, God = false;

	void TranslateSnake(Vector2f v2) {
		for (int i = 0; i < cells.size(); i++)
			cells[i].TranslateCell(v2);
	}

	void TranslateSnake(Apple &apple) {
		for (int i = 0; i < cells.size(); i++)
			cells[i].TranslateCell(cells[i].direct);
		apple.cells = cells;

	}

	Snake(int size, Cell cell) {
		for (int i = 0; i < size; i++) {
			cells.push_back(cell);
			TranslateSnake(Vector2f(0, cell.size));
			cells[i].direct = Vector2f(0, 25);
		}
		TranslateSnake(Vector2f(0, -cell.size));

	}

	Snake(int size, Cell cell, bool godEnable) {
		for (int i = 0; i < size; i++) {
			cells.push_back(cell);
			TranslateSnake(Vector2f(0, cell.size));
			cells[i].direct = Vector2f(0, 25);
		}
		TranslateSnake(Vector2f(0, -cell.size));
		God = godEnable;
	}

	void addCell(Cell cell) {
		cell = cells[cells.size() - 1];
		cell.border.move(Vector2f(-cells[cells.size() - 1].direct));
		cell.main.move(Vector2f(-cells[cells.size() - 1].direct));
		cells.push_back(cell);
	}

	void Draw(RenderWindow& win) {
		for (int i = 0; i < cells.size(); i++) {
			cells[i].DrawCell(win);
		}
	}

	void ChangeDirect() {
		for (int i = cells.size() - 1; i > 0; i--) {
			if (cells[i].direct != cells[i - 1].direct) {
				cells[i].direct = cells[i - 1].direct;
			}
		}
	}
	void CheckCollectApple(Apple &apple, Cell cell, int &score, Timer &timer, float proc) {
		if (apple.pos == cells[0].border.getPosition()) {
			timer.per -= timer.per * proc;

			apple.Spawn();
			addCell(cell);
			score++;
			cout << "Now your score iqual to " << score << endl;
			cout << "Apple was collected! && snake size = " << cells.size() << endl;
		}
	}

	void Death() {
		cout << "Snake dead!";
	}

	void CheckDeath() {
		// 1 ����� ������ ��������� ����
		if (!God) {
			for (int i = 0; i < cells.size(); i++) {
				for (int j = 0; j < cells.size(); j++) {
					if (cells[i].border.getPosition() == cells[j].border.getPosition() && i != j) {
						Alive = false;
					}
				}
			}

			// 2 ����� ������ ��������� ����� ���� (�� ������ ���� �������� ������ 500�500)
			if (cells[0].border.getPosition().x == 500 || cells[0].border.getPosition().y == 500 || cells[0].border.getPosition().x == -25 || cells[0].border.getPosition().y == -25) Alive = false;
			if (!Alive) Death();
		}
	}
};

