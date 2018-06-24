#include <iostream>
#include <math.h>
#include <sys/time.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stack>
#include "shapes.h"
using namespace std;

int Height = 5, Width = 5;
const float w = 500, h = 500, o = 20;
float ww = w - 2 * o, hh = h - 2 * o;
int posx, posy, endx, endy, startx, starty;
Player m_Player(0, 0, ww / Width / 2 * 4.0f / 5.0f,
		hh / Height / 2 * 4.0f / 5.0f);
void DFS(int i, int j);
void DFSnr(int i, int j);
void display();
bool Zoom = false;

struct Cell {
	bool visited;
	bool road[4];   // is each four directions of cells(up, down, right, left)

	Cell() {
		visited = false;
		road[0] = false;
		road[1] = false;
		road[2] = false;
		road[3] = false;
	}
} Node[200][200];

void createMaze() {
	ww = w - 2 * o, hh = h - 2 * o;
	m_Player = Player(0, 0, ww / Width / 2 * 4.0f / 5.0f,
			hh / Height / 2 * 4.0f / 5.0f);
	posy = 0;
	posx = rand() % Width;
	endy = Height - 1;
	endx = rand() % Width;
	Node[0][posx].road[0] = Node[Height - 1][endx].road[1] = true;

	if (Width < 80)
		DFS(rand() % Height, rand() % Width);
	else
		DFSnr(rand() % Height, rand() % Width);

	for (int i = 0; i < Height; ++i)
		for (int j = 0; j < Width; ++j)
			Node[i][j].visited = false;

	startx = posx;
	starty = posy;
}

void DrawCells() {
	for (int i = 0; i < Height; ++i) {
		for (int j = 0; j < Width; ++j) {
			float cx = (2 * (j) + 1) * ww / Width / 2 + o, cy = (2
					* (Height - i - 1) + 1) * hh / Height / 2 + o, dx = ww
					/ Width / 2, dy = hh / Height / 2;

			if (Node[i][j].visited) {
				glColor3f(205 / 255.0, 133 / 255.0, 63 / 255.0);
				glBegin(GL_POLYGON);
				glVertex2f(cx - dx, cy + dy);
				glVertex2f(cx + dx, cy + dy);
				glVertex2f(cx + dx, cy - dy);
				glVertex2f(cx - dx, cy - dy);
				glEnd();
			}

			if (-1 == endx && -1 == endy)
				m_Player.SetState(Player::Happy);

			if (i == posy && j == posx) {
				glMatrixMode(GL_PROJECTION);
				glLoadIdentity();
				if (Zoom && Width > 8)
					gluOrtho2D(cx - 17 * dx, cx + 17 * dx, cy - 17 * dx,
							cy + 17 * dx);
				else
					gluOrtho2D(0, 500, 0, 500);
				glMatrixMode(GL_MODELVIEW);
				glLoadIdentity();
				m_Player.SetPos(cx, cy);
				m_Player.DrawFace();
			}

			glLineWidth(2);
			if (!Node[i][j].road[0]) {
				glBegin(GL_LINE_STRIP);
				glColor3f(rand() % 255 / 255.0, rand() % 255 / 255.0,
						rand() % 255 / 255.0);
				glVertex2f(cx - dx, cy + dy);
				glColor3f(rand() % 255 / 255.0, rand() % 255 / 255.0,
						rand() % 255 / 255.0);
				glVertex2f(cx + dx, cy + dy);
				glEnd();
			}
			if (!Node[i][j].road[1]) {
				glBegin(GL_LINE_STRIP);
				glColor3f(rand() % 255 / 255.0, rand() % 255 / 255.0,
						rand() % 255 / 255.0);
				glVertex2f(cx - dx, cy - dy);
				glColor3f(rand() % 255 / 255.0, rand() % 255 / 255.0,
						rand() % 255 / 255.0);
				glVertex2f(cx + dx, cy - dy);
				glEnd();
			}
			if (!Node[i][j].road[2]) {
				glBegin(GL_LINE_STRIP);
				glColor3f(rand() % 255 / 255.0, rand() % 255 / 255.0,
						rand() % 255 / 255.0);
				glVertex2f(cx + dx, cy - dy);
				glColor3f(rand() % 255 / 255.0, rand() % 255 / 255.0,
						rand() % 255 / 255.0);
				glVertex2f(cx + dx, cy + dy);
				glEnd();
			}
			if (!Node[i][j].road[3]) {
				glBegin(GL_LINE_STRIP);
				glColor3f(rand() % 255 / 255.0, rand() % 255 / 255.0,
						rand() % 255 / 255.0);
				glVertex2f(cx - dx, cy - dy);
				glColor3f(rand() % 255 / 255.0, rand() % 255 / 255.0,
						rand() % 255 / 255.0);
				glVertex2f(cx - dx, cy + dy);
				glEnd();
			}
		}
	}
	if (Node[posy][posx].visited)
		m_Player.SetState(Player::Sad);
	else
		m_Player.SetState(Player::Normal);
}

void DFS(int i, int j) {
	if (Node[i][j].visited)
		return;
	else {
		Node[i][j].visited = true;
		srand(time(NULL) * i * j);
		int i1 = rand() % 1000;
		for (int j1 = 0; j1 < 4; j1++) {
			if (i + 1 < Height && !Node[i + 1][j].visited && i1 % 4 == 0) {
				Node[i][j].road[1] = true;
				Node[i + 1][j].road[0] = true;
				DFS(i + 1, j);
			}
			if (i - 1 >= 0 && !Node[i - 1][j].visited && i1 % 4 == 1) {
				Node[i][j].road[0] = true;
				Node[i - 1][j].road[1] = true;
				DFS(i - 1, j);
			}
			if (j + 1 < Width && !Node[i][j + 1].visited && i1 % 4 == 2) {
				Node[i][j].road[2] = true;
				Node[i][j + 1].road[3] = true;
				DFS(i, j + 1);
			}
			if (j - 1 >= 0 && !Node[i][j - 1].visited && i1 % 4 == 3) {
				Node[i][j].road[3] = true;
				Node[i][j - 1].road[2] = true;
				DFS(i, j - 1);
			}
			i1++;
		}
	}
}

void DFSnr(int i, int j) {
	stack<pair<int, int> > Stack;
	Stack.push(pair<int, int>(i, j));

	if (Node[i][j].visited)
		return;
	else {
		while (!Stack.empty()) {
			pair<int, int> p = Stack.top();
			i = p.first, j = p.second;
			Stack.pop();
			srand(time(NULL) * i * j);
			int i1 = rand() % 100;
			for (int j1 = 0; j1 < 4; j1++) {
				if (i + 1 < Height && !Node[i + 1][j].visited && i1 % 4 == 0) {
					Node[i][j].road[1] = true;
					Node[i + 1][j].road[0] = true;
					Node[i + 1][j].visited = true;
					Stack.push(pair<int, int>(i + 1, j));
				}
				if (i - 1 >= 0 && !Node[i - 1][j].visited && i1 % 4 == 1) {
					Node[i][j].road[0] = true;
					Node[i - 1][j].road[1] = true;
					Node[i - 1][j].visited = true;
					Stack.push(pair<int, int>(i - 1, j));
				}
				if (j + 1 < Width && !Node[i][j + 1].visited && i1 % 4 == 2) {
					Node[i][j].road[2] = true;
					Node[i][j + 1].road[3] = true;
					Node[i][j + 1].visited = true;
					Stack.push(pair<int, int>(i, j + 1));
				}
				if (j - 1 >= 0 && !Node[i][j - 1].visited && i1 % 4 == 3) {
					Node[i][j].road[3] = true;
					Node[i][j - 1].road[2] = true;
					Node[i][j - 1].visited = true;
					Stack.push(pair<int, int>(i, j - 1));
				}
				i1++;
			}
		}
	}
}

void init() {

	createMaze();

	glMatrixMode(GL_PROJECTION);
	gluOrtho2D(0, w, 0, h);
	glMatrixMode(GL_MODELVIEW);
}

void display() {
	glClear(GL_COLOR_BUFFER_BIT);
	glClearColor(255 / 255.0, 228 / 255.0, 181 / 255.0, 1);

	Width -= 4;
	glColor3f(0, 0, 1);
	char ch[] = { '-', '>', ' ', 'L', 'E', 'V', 'E', 'L', ' ', (
			(Width / 100) ? (char) (Width / 100 + '0') : ' '), (
			(Width / 10) ? (char) ((Width / 10) % 10 + '0') : ' '),
			(char) (Width % 10 + '0'), ' ', '<', '-', '\0' };
	Width += 4;
	MakeText((w - strlen(ch) * 9) * 0.5f, h - o / 1.2f, ch);
	glColor3f(0, 0, 0);
	DrawCells();
	glutSwapBuffers();
}

void idle() {
	glutPostRedisplay();
}

void Reset() {
	posx = startx, posy = starty;
	for (int i = 0; i < Height; ++i)
		for (int j = 0; j < Width; ++j)
			Node[i][j].visited = false;
}

void Restart() {
	memset(Node, 0, sizeof(Node));
	createMaze();
}

void NextLevel() {
	Height++, Width++;
	assert(Height < 205);
	Restart();
}

void PreLevel() {
	Height--, Width--;
	assert(Height > 4);
	Restart();
}

void NextLevel(int val) {
	NextLevel();
}

void keyboard(unsigned char ch, int x, int y) {
	Node[posy][posx].visited = true;
	switch (ch) {
	case 'W':
	case 'w':
		if (Node[posy][posx].road[0] && posy > 0
				&& !Node[posy - 1][posx].visited)
			posy--;
		break;

	case 'S':
	case 's':
		if (Node[posy][posx].road[1] && posy < Height - 1
				&& !Node[posy + 1][posx].visited)
			posy++;
		break;

	case 'A':
	case 'a':
		if (Node[posy][posx].road[3] && posx > 0
				&& !Node[posy][posx - 1].visited)
			posx--;
		break;

	case 'D':
	case 'd':
		if (Node[posy][posx].road[2] && posx < Width - 1
				&& !Node[posy][posx + 1].visited)
			posx++;
		break;
	case 'r':
	case 'R':
		Reset();
		break;
	case 'n':
	case 'N':
		Restart();
		break;
	case 'U':
	case 'u':
		NextLevel();
		break;
	case 'L':
	case 'l':
		PreLevel();
		break;
	case '+':
		Zoom = !Zoom;
		break;
	}
	if (posx == endx && posy == endy) {
		glutTimerFunc(1000, NextLevel, 0);
		endx = endy = -1;
	}
}

int main(int argc, char ** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(w, h);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("MAZE GAME");
	glutDisplayFunc(display);
	glutIdleFunc(idle);
	glutKeyboardFunc(keyboard);
	init();
	glutMainLoop();
}
