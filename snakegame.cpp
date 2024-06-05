#include <iostream>
#include <cstdlib>
#include <ctime>
#include <unistd.h>
#include <termios.h>
#include <vector>
using namespace std;

bool gameOver;
const int width = 40;
const int height = 20;
int x, y, fruitX, fruitY, score;
int aiX, aiY, aiScore;
vector<int> tailX, tailY;
vector<int> aiTailX, aiTailY;
int nTail, aiTail;
enum eDirection { STOP = 0, LEFT, RIGHT, UP, DOWN };
eDirection dir, aiDir;

void Setup() {
    gameOver = false;
    dir = STOP;
    aiDir = STOP;
    x = width / 2;
    y = height / 2;
    aiX = width / 3;
    aiY = height / 3;
    fruitX = rand() % width;
    fruitY = rand() % height;
    score = 0;
    aiScore = 0;
    nTail = 0;
    aiTail = 0;
    tailX.clear();
    tailY.clear();
    aiTailX.clear();
    aiTailY.clear();
}

void Draw() {
    system("clear"); // system("cls"); for Windows
    for (int i = 0; i < width + 2; i++)
        cout << "#";
    cout << endl;

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (j == 0)
                cout << "#";
            if (i == y && j == x)
                cout << "O";
            else if (i == aiY && j == aiX)
                cout << "A";
            else if (i == fruitY && j == fruitX)
                cout << "F";
            else {
                bool print = false;
                for (int k = 0; k < nTail; k++) {
                    if (tailX[k] == j && tailY[k] == i) {
                        cout << "o";
                        print = true;
                    }
                }
                for (int k = 0; k < aiTail; k++) {
                    if (aiTailX[k] == j && aiTailY[k] == i) {
                        cout << "a";
                        print = true;
                    }
                }
                if (!print)
                    cout << " ";
            }

            if (j == width - 1)
                cout << "#";
        }
        cout << endl;
    }

    for (int i = 0; i < width + 2; i++)
        cout << "#";
    cout << endl;

    cout << "Score:" << score << "  AI Score:" << aiScore << endl;
}

char getch() {
    char buf = 0;
    struct termios old = {0};
    if (tcgetattr(0, &old) < 0)
        perror("tcsetattr()");
    old.c_lflag &= ~ICANON;
    old.c_lflag &= ~ECHO;
    old.c_cc[VMIN] = 1;
    old.c_cc[VTIME] = 0;
    if (tcsetattr(0, TCSANOW, &old) < 0)
        perror("tcsetattr ICANON");
    if (read(0, &buf, 1) < 0)
        perror ("read()");
    old.c_lflag |= ICANON;
    old.c_lflag |= ECHO;
    if (tcsetattr(0, TCSADRAIN, &old) < 0)
        perror ("tcsetattr ~ICANON");
    return (buf);
}

void Input() {
    if (cin.peek() != EOF) {
        switch (getch()) {
            case 'a':
                dir = LEFT;
                break;
            case 'd':
                dir = RIGHT;
                break;
            case 'w':
                dir = UP;
                break;
            case 's':
                dir = DOWN;
                break;
            case 'x':
                gameOver = true;
                break;
        }
    }
}

void AILogic() {
    if (aiX < fruitX) {
        aiDir = RIGHT;
    } else if (aiX > fruitX) {
        aiDir = LEFT;
    } else if (aiY < fruitY) {
        aiDir = DOWN;
    } else if (aiY > fruitY) {
        aiDir = UP;
    }
}

void Logic() {
    int prevX = (nTail > 0) ? tailX[0] : x;
    int prevY = (nTail > 0) ? tailY[0] : y;
    int prev2X, prev2Y;

    tailX.insert(tailX.begin(), x);
    tailY.insert(tailY.begin(), y);

    if (tailX.size() > nTail) {
        tailX.pop_back();
        tailY.pop_back();
    }

    switch (dir) {
        case LEFT:
            x--;
            break;
        case RIGHT:
            x++;
            break;
        case UP:
            y--;
            break;
        case DOWN:
            y++;
            break;
        default:
            break;
    }

    int aiPrevX = (aiTail > 0) ? aiTailX[0] : aiX;
    int aiPrevY = (aiTail > 0) ? aiTailY[0] : aiY;

    aiTailX.insert(aiTailX.begin(), aiX);
    aiTailY.insert(aiTailY.begin(), aiY);

    if (aiTailX.size() > aiTail) {
        aiTailX.pop_back();
        aiTailY.pop_back();
    }

    switch (aiDir) {
        case LEFT:
            aiX--;
            break;
        case RIGHT:
            aiX++;
            break;
        case UP:
            aiY--;
            break;
        case DOWN:
            aiY++;
            break;
        default:
            break;
    }

    if (x >= width) x = 0; else if (x < 0) x = width - 1;
    if (y >= height) y = 0; else if (y < 0) y = height - 1;

    if (aiX >= width) aiX = 0; else if (aiX < 0) aiX = width - 1;
    if (aiY >= height) aiY = 0; else if (aiY < 0) aiY = height - 1;

    for (int i = 0; i < nTail; i++)
        if (tailX[i] == x && tailY[i] == y)
            gameOver = true;

    for (int i = 0; i < aiTail; i++)
        if (aiTailX[i] == aiX && aiTailY[i] == aiY)
            gameOver = true;

    if (x == fruitX && y == fruitY) {
        score += 10;
        fruitX = rand() % width;
        fruitY = rand() % height;
        nTail++;
    }

    if (aiX == fruitX && aiY == fruitY) {
        aiScore += 10;
        fruitX = rand() % width;
        fruitY = rand() % height;
        aiTail++;
    }
}

int main() {
    srand(static_cast<unsigned int>(time(0)));
    Setup();
    while (!gameOver) {
        Draw();
        Input();
        AILogic();
        Logic();
        usleep(100000); // Sleep for 100ms
    }
    return 0;
}
