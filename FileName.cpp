#include <iostream>
#include <Windows.h>
using namespace std;

//Создаейм пораметры змейки
struct Snake {
    int direction;
    int positionX;
    int positionY;
};

//Очищает поле
void clear() {
    COORD topLeft = { 0, 0 };
    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO screen;
    DWORD written;

    GetConsoleScreenBufferInfo(console, &screen);
    FillConsoleOutputCharacterA(
        console, ' ', screen.dwSize.X * screen.dwSize.Y, topLeft, &written
    );
    FillConsoleOutputAttribute(
        console, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE,
        screen.dwSize.X * screen.dwSize.Y, topLeft, &written
    );
    SetConsoleCursorPosition(console, topLeft);
}

void printArr2(int** arr, int rows, int cols)
{
    cout << "SNAKE RUN" << endl;
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            if (arr[i][j] == 2) {
                cout << "=" << " ";
            }
            else if (arr[i][j] == 0) {
                cout << "  ";
            }
            else if (arr[i][j] == 1) {
                cout << "*" << " ";
            }
            else if (arr[i][j] == 3) {
                cout << "@" << " ";
            }
        }
        cout << endl;
    }

}

void creatArr(int** arr, int rows, int cols) {
    for (int i = 0; i < rows; i++)
    {
        arr[i] = new int[cols] {};
    }
    //создание поля
    for (int i = 0; i < rows; i++) {
        arr[i][0] = 2;
        arr[i][cols - 1] = 2;
    }
    for (int i = 0; i < cols; i++) {
        arr[0][i] = 2;
        arr[rows - 1][i] = 2;
    }
}

//Добовляет змейку на доску
void addSnakeToBoard(Snake* snake, int length, int** board) {
    board[snake[0].positionY][snake[0].positionX] = 1;
    for (int i = 1; i < length; i++) {
        board[snake[i].positionY][snake[i].positionX] = 2;
    }
}

//появление яблок
void createApple(int**& arr, int rows, int cols, bool& applecounter)
{
    while (applecounter == false)
    {
        int y = rand() % ((rows - 1) + 1) - 1;
        int x = rand() % ((cols - 1) + 1) - 1;
        if (arr[y][x] == 0)
        {
            arr[y][x] = 3;
            applecounter = true;

        }
        else {
            continue;
        }
    }
}

//Увиличиваем змейку на один фрагмент
void largeSnake(Snake*& snake, int length) {
    Snake* new_snake = new Snake[length]{};
    for (int i = 0; i < length; i++) {
        if (i < length - 1) {
            new_snake[i] = snake[i];
        }
        else {
            new_snake[length - 1] = { 0, snake[length - 2].positionX, snake[length - 2].positionY };
        }
    }
    delete[] snake;
    snake = new_snake;
}

//проверяем пустая следующая клетка или ивент
int checkEvent(Snake* snake, int** board, int& score) {
    int x = snake[0].positionX;
    int y = snake[0].positionY;
    if (snake[0].direction <= 0) {
        snake[0].direction = 4;
    }
    else if (snake[0].direction > 4) {
        snake[0].direction = 1;
    }

    if (snake[0].direction == 4) {
        if (board[y + 1][x] == 3) {
            return 3;
        }
        else if (board[y + 1][x] == 2) {
            return 0;
        }
        else {
            return 1;
        }
    }
    else if (snake[0].direction == 3) {
        if (board[y][x - 1] == 3) {
            score += 1;
            return 3;
        }
        else if (board[y][x - 1] == 2) {
            return 0;
        }
        else {
            return 1;
        }
    }
    else if (snake[0].direction == 2) {
        if (board[y - 1][x] == 3) {
            score += 1;
            return 3;
        }
        else if (board[y - 1][x] == 2) {
            return 0;
        }
        else {
            return 1;
        }
    }
    else if (snake[0].direction == 1) {
        if (board[y][x + 1] == 3) {
            score += 1;
            return 3;
        }
        else if (board[y][x + 1] == 2) {
            return 0;
        }
        else {
            return 1;
        }

    }
}
//Двигаем змейку на одну клетку в заданом направлении
void snakeMove(Snake* snake, int length, int** board) {
    board[snake[length - 1].positionY][snake[length - 1].positionX] = 0;
    for (int i = length - 1; i > 0; i--) {
        snake[i].positionX = snake[i - 1].positionX;
        snake[i].positionY = snake[i - 1].positionY;
    }
    if (snake[0].direction == 4) {
        snake[0].positionY += 1;
    }
    else if (snake[0].direction == 3) {
        snake[0].positionX -= 1;
    }
    else if (snake[0].direction == 2) {
        snake[0].positionY -= 1;
    }
    else if (snake[0].direction == 1) {
        snake[0].positionX += 1;
    }


}

//Считывает нажатие клафиш.
void snakeControl(Snake* snake) {
    if (GetAsyncKeyState(65)) {
        snake[0].direction += 1;

    }
    if (GetAsyncKeyState(68)) {
        snake[0].direction -= 1;

    }
}

void lose(int score) {
    clear();
    cout << "You lose" << endl;
    cout << "Your score: " << score << endl;
}
/*бинарный файл для лудшего ризультата*/
void binFileOfBestScore(int score)
{
    const char* pathToFile = "BestScore.bin";
    FILE* file;
    if (fopen_s(&file, pathToFile, "a+") == NULL)
    {
        int best_score = 0;
        fread(&best_score, sizeof(best_score), 1, file);
        fclose(file);
        if (best_score < score) {
            fopen_s(&file, pathToFile, "w");
            best_score = score;
            fwrite(&score, sizeof(score), 1, file);
            fclose(file);
        }
        cout << "Best score: " << best_score;
    }
}
int main()
{
    int defeat = 1;
    bool applecounter = false;
    int rows = 20, cols = 20;
    int score = 0;
    int length = 4;
    int** arr = new int* [rows] {};
    Snake* snake = new Snake[length]{ {4, 10, 10}, {0, 10, 9},{4, 10, 8},{0, 10, 7} };
    creatArr(arr, rows, cols);
    while (defeat != 0) {
        clear();
        createApple(arr, rows, cols, applecounter);
        snakeControl(snake);
        defeat = checkEvent(snake, arr, score);
        if (defeat == 3) {
            score += 1;
            length += 1;
            applecounter = false;
            largeSnake(snake, length);
        }
        snakeMove(snake, length, arr);
        addSnakeToBoard(snake, length, arr);
        printArr2(arr, rows, cols);
        Sleep(200);
    }
    lose(score);
    binFileOfBestScore(score);
}