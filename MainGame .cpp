#include <iostream>
#include <string>       // для строк std::string
#include <time.h>
#include <conio.h>      // для _getch(), _kbhit()
#include "Win10.h"
#include <ctime>
#include <iomanip> 
#include <fstream>
using namespace std;
#define RAND(min,max)  (rand()%((max)-(min)+1)+(min))
#define SIZE 5

// структура для хранения координаты игрока, противника
//  struct Coord - пользовательский тип данных
struct Coord
{
    int row, col; // переменные для координат
};

//
// игровое поле - массив строк типа string
// холст кадра игры (Canvas)
// Символы: & - главный игрок; 1 - противники;
//          # - стена; 1,2,3,4 - бонусы;
//          * - жизнь;
//          ? - дверь, переход на следующий уровень;
//


string Pole[]   // <==> char pole[15][35+1];
{
  "####################################",// +'\0'
  "##                                ##",
  "##                                ##",
  "##                                ##",
  "##                                ##",
  "##                                ##",
  "##                           &o   ##",
  "##                                ##",
  "##                                ##",
  "##                                ##",
  "##                                ##",
  "##                                ##",
  "####################################"
};

// высота и ширина игрового поля
int PoleHeight;   // = 15
int PoleWidth;    // = 35

// Экранные координаты вывода поля на экран
int PoleRow = 3; // по строке
int PoleCol = 8; // по столбцу
Coord tail[13 * 37];
int cnttail = 1;
int Score = 0;
int lives;
void Menu();
int SettingsMenu();
void GameOver();
void About();
// прототипы функций
void StartGame();     // ф-ция для запуска новой игры
//GetCoordHero();       // Функция для получения координаты главного игрока
//GetCoordEnemys();     // Функция для получения координат противников
int DrawPole(int key);      // функция вывода игрового поля на экран
int Menu(string* Items, int cntItems,
    int Row, int Col, int Select);
int SettingsMen(string* Items, int cntItems,
    int Row, int Col, int Select);
void Spawn();
int SdvigHvosta(Coord* tail, int cnttail, Coord Head);
void PrintHvost(Coord* tail, int cnttail);
void Win();
// пункты главного меню программы
// массив строк
string MainMenu[]
{
  "Start New Game",
  "Settings",
  "About",
  "Exit"
};
// вычисление размера массива MainMenu
const int cntMainMenu =
sizeof(MainMenu) / sizeof(MainMenu[0]);

string Settings[]
{
  "Easy (2 Lives, 1x ScorePoints, Speed: low)",
  "Medium (1 Lives, 2x ScorePoints, Speed: medium)",
  "Hard (Extra lives for the weak, 3x ScorePoints, Speed: Flash) ",
};
// вычисление размера массива MainMenu
const int cntSetting =
sizeof(Settings) / sizeof(Settings[0]);
int dif = 0;


int main()
{
    //system("chcp 1251 > nul"); //setlocale(LC_ALL, "RUSSIAN");
    //system("chcp 866 > nul"); //setlocale(LC_ALL, "C");
    // вычисление высоты игрового поля:
    //   размер всего поля / размер одной строки
    Menu();

    cin.get(); cin.get();
} // main()

int SettingsMenu() { // подменю настроек для выбора сложности
    int  Select = 0;
    int  menuRow = 3, menuCol = 12;

    do {
        // Главное меню игры
        if (Select == -1) { Select = 0; }
        Select = SettingsMen(Settings, cntSetting,
            menuRow, menuCol, Select);

        switch (Select)
        {
        case 0: // "Easy"
            dif = 0;
            CLS;
            Menu();
            break;
        case 1: // "Medium"
            dif = 1;
            CLS;
            Menu();
            break;
        case 2: // "Hard"
            dif = 2;
            CLS;
            Menu();
            break;
            // switch(Select)
        case -1: // ESC
            CLS;
            Menu();
            break;
        }
    } while (Select != 0);
    return dif;
}

void Menu() {
    PoleHeight = sizeof(Pole) / sizeof(Pole[0]);
    PoleWidth = Pole[0].size(); // ширина строки поля

    bool isExit = false; // сигнал завершения работы
    int  Select = 0;
    int  menuRow = 3, menuCol = 12;

    do {
        // Главное меню игры
        if (Select == -1) { Select = 0; }
        Select = Menu(MainMenu, cntMainMenu,
            menuRow, menuCol, Select);

        switch (Select)
        {
        case 0: // "Start New Game"
            CLS;
            StartGame();     // ф-ция для запуска новой игры
            break;
        case 1: // "Settings"
            CLS;
            SettingsMenu();      // настройки
            break;
        case 2: // "About"
            CLS;
            About();         // об авторах
            break;
        case 3: // "Exit"
        case -1: // ESC
        {
            // надо сделать запрос на выход из программы
            SETLOCAL_RU; // для вывода кириллицы 1251
            SetPos(menuRow + cntMainMenu + 1, menuCol);
            cout << "Дейстивельно ли Вы хотите выйти"
                " из программы(y/n): ";
            SETLOCAL_EN; // для вывода псевдографики 866
            char key = _getche();
            if (key == 'y' || key == 'Y') {
                isExit = true; // сигнал на завершение работы программы
                SetPos(menuRow + cntMainMenu + 2, menuCol);
                cout << "Good bye...";
            }
            else {
                CLS; // очистка экрана
            }
        }
        break;
        } // switch(Select)

    } while (isExit == false);

}


// SearchPlayer(Pole, poleHeight, '*');
Coord SearchPlayer(string* pole, int height, char sym)
{
    Coord result{ -1,-1 };// переменная типа Coord для хранения координат
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < pole[i].size(); j++) {
            if (pole[i][j] == sym) {
                result.row = i; // индекс строки
                result.col = j; // индекс столбца
                return result; // возвращаем координаты в структурной
            }
        }
    }
    //result.row = result.col = -1;
    return result;
}

// Функция начала новой игры
void StartGame()
{
        Spawn();

        // получить текущую позицию игрока
        Coord pl = SearchPlayer(Pole, PoleHeight, '&'); // '&'

        //Coord tail = SearchPlayer(Pole, PoleHeight, 'o');
        tail[0].row = pl.row;
        tail[0].col = pl.col + 1;
        tail[1].row = pl.row;
        tail[1].col = pl.col + 1;

        int key = 0;
        //Coord en = SearchEnemy(Pole, PoleHeight);
        int frams = 0; // счетчик кадров
        double speed; // скорость
        const int max = 481;
        int last_step = 0; // последнее значение для перемещения
        int t = 0, ScorePoint;
        bool gameover = false;
        switch (dif) {
        case 0:
            lives = 2;
            ScorePoint = 10;
            speed = 100;
            break;
        case 1:
            lives = 1;
            ScorePoint = 20;
            speed = 75;
            break;
        case 2:
            lives = 0;
            ScorePoint = 30;
            speed = 50;
            break;
        }
        do // цикл пока идет игра, завершить цикл по завершению игры
        {
            if (_kbhit()) // 1) проверить нажатие клавиш
            {
                key = _getch(); // 2) получить код нажатой клавиши
            }
            // регулировка скрости объекта - игрока
            if (int(frams) != last_step)
            {
                last_step = int(frams);
                // 3) изменить позицию игрока, если он двигается
                Coord old = pl; // запоминаем старые координаты
                switch (key)
                {
                case _KEY::UP:    // вверх
                    if (Pole[pl.row - 1][pl.col] == '#' || Pole[pl.row - 1][pl.col] == 'o') {
                        lives--;
                        key = 0;
                    }
                    else {
                        if (Pole[pl.row - 1][pl.col] == '0') {
                            Spawn();
                            Score += ScorePoint;
                            cnttail++;
                        }
                        pl.row--;
                        Sleep(100);
                    }
                    break;
                case _KEY::DOWN:  // вниз
                    if (Pole[pl.row + 1][pl.col] == '#' || Pole[pl.row + 1][pl.col] == 'o') {
                        lives--;
                        key = 0;
                    }
                    else {
                        if (Pole[pl.row + 1][pl.col] == '0') {
                            Spawn();
                            Score += ScorePoint;
                            cnttail++;
                        }
                        pl.row++;
                        Sleep(100);
                    }
                    break;
                    //SdvigHvosta(key, cnttail);
                    break;
                case _KEY::LEFT:  // влево
                    if (Pole[pl.row][pl.col - 1] == '#' || Pole[pl.row][pl.col - 1] == 'o') {
                        lives--;
                        key = 0;
                    }
                    else {
                        if (Pole[pl.row][pl.col - 1] == '0') {
                            Spawn();
                            Score += ScorePoint;
                            cnttail++;
                        }
                        pl.col--;
                    }
                    break;
                case _KEY::RIGHT: // вправо
                    if (Pole[pl.row][pl.col + 1] == '#' || Pole[pl.row][pl.col + 1] == 'o') {
                        lives--;
                        key = 0;
                    }
                    else {
                        if (Pole[pl.row][pl.col + 1] == '0') {
                            Spawn();
                            Score += ScorePoint;
                            cnttail++;
                        }
                        pl.col++;
                    }
                    break;
                    // switch (key)
                }
                if (old.row != pl.row || old.col != pl.col) {
                    SdvigHvosta(tail, cnttail, old);
                }
                // перерисовать игрока
                Pole[old.row][old.col] = ' '; // стереть из старой позиции
                Pole[pl.row][pl.col] = '&'; // игрок в новой позиции
                PrintHvost(tail, cnttail);

                srand(time(NULL));
                t++;
                SetPos(5, PoleWidth + 15);
                cout << "Time: " << t / 10;
                SetPos(7, PoleWidth + 14);
                cout << "Score: " << Score;
                SetPos(9, PoleWidth + 14);
                cout << "Lives: " << lives;

            } //
            //} // if( _kbhit() )

            // 4) движение противников
            //Pole[oldEnemy.row][oldEnemy.col] = point; // стереть из старой позиции
            //Pole[enemy.row][enemy.col] = '1'; // игрок в новой позиции

            // 5) перепросивока поля на экране
            DrawPole(key); // вывод поля на экран
            // 6) проверка на завершение: победа или поражение
            Sleep(speed); // задержка программы
            frams++; // подсчет кадров

            if (lives < 0) { // условия поражения
                GameOver();
                cin.get(); cin.get();
                gameover = false;
            }
            if (cnttail == max) { // условия победы
                Win();
                cin.get(); cin.get();
                gameover = false;
            }
        } while (gameover == false);
} // StartGame()

void Win() {
    CLS;
    SetPos(13, PoleWidth + 14);
    cout << "    You WIN!";
    SetPos(14, PoleWidth + 12);
    cout << "Your Total SCORE: " << Score;
}

void GameOver() {
    CLS;
    SetPos(13, PoleWidth + 14);
    cout << "  GAME OVER!";
    SetPos(14, PoleWidth + 12);
    cout << "Your Total SCORE: " << Score;

}

void Spawn() { // спавн яблока
    int apple = 0;
    do {
        int X_Apple = RAND(1, PoleHeight - 2);
        int Y_Apple = RAND(2, PoleWidth - 3);
        if (Pole[X_Apple][Y_Apple] != 'O' && Pole[X_Apple][Y_Apple] != 'o') {
            Pole[X_Apple][Y_Apple] = '0';
            apple = 1;
        }
    } while (apple != 1);
}

void PrintHvost(Coord* tail, int cnttail) { // функция прорисовки хвоста
    for (int i = 0; i < cnttail; i++) {
        Pole[tail[i].row][tail[i].col] = 'o';
    }
    Pole[tail[cnttail].row][tail[cnttail].col] = ' ';
}

void About() {
    SetPos(5, 12);
    cout << "Developed by Mussabekov Temirlan in 2022";
    SetPos(7, 12);
    cout << "Press ESC for back in menu";
    int key = 0;
    key = _getch();
    switch (key)
    {
    case -1: // ESC
        CLS;
        Menu();
        break;
    }
}

int SdvigHvosta(Coord* tail, int cnttail, Coord Head) { // функция передвижения хвоста за головой

    for (int i = cnttail; i != 0; i--) {
        tail[i] = tail[i - 1];
    }
    tail[0] = Head;

    return 0;
}
// функция вывода игрового поля на экран
int DrawPole(int key) {
    for (int i = 0; i < PoleHeight; i++)
    {

        // поставить курсор консоли для i-ой строки поля
        SetPos(PoleRow + i, PoleCol);

        for (int j = 0; j < PoleWidth; j++)
        {
            if (Pole[i][j] == '#') { // символ стены
                SetColor(COLOR::black, COLOR::green);
                cout << char(177);
            }
            else if (Pole[i][j] == '0') { // яблоко
                SetColor(COLOR::black, COLOR::light_red);
                cout << '0';
            }
            else if (Pole[i][j] == '&') { // главный герой игры
                SetColor(COLOR::black, COLOR::yellow);
                cout << 'O';
            }
            else if (Pole[i][j] == 'o') { // хвост
                SetColor(COLOR::black, COLOR::yellow);
                cout << 'o';
            }
            else { cout << Pole[i][j]; } // все остальные символы поля

        } // for (j : PoleWidth)
    } // for (i : PoleHeight)
    return 1;
} // void DrawPole()
