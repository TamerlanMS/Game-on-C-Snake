#include <iostream>
#include <string>
#include "Win10.h"
using namespace std;

// цвета для активного пункта меню
int clSelFon = COLOR::green; // для фона
int clSelSym = COLOR::black; // для символов

// цвета для неактивных пунктов меню
int clUnselFon = COLOR::blue;         // для фона
int clUnselSym = COLOR::light_yellow; // для символов

// цвет консоли по умолчанию
int clDefFon = COLOR::black; // черный фон
int clDefSym = COLOR::white; // белые буквы


int Menu(string* Items, int cntItems,
    int Row, int Col, int Select)
{
    CursorHide(); // ==> CursorHide(FALSE, 10);
    while (true) // цикл работы меню
    {
        // 1) вывод пунктов меню
        for (int i = 0; i < cntItems; i++)
        {
            SetPos(Row + i, Col); // координаты для i-го пункта меню
            if (i == Select) { SetColor(clSelFon, clSelSym); }
            else { SetColor(clUnselFon, clUnselSym); }
            cout << Items[i]; // вывод i-го пункта меню
        } // for();

        // Восстанавливаем цвет вывода на консоль по умолчанию
        SetColor(clDefFon, clDefSym);

        // 2) обработка нажатий клавиш
        //if (_kbhit()) // проверка на нажатие клавиши
        {
            int key = _getch(); // получить код нажатой клавиши
            switch (key)
            {
            case _KEY::UP:  case _KEY::LEFT: // вверх
                if (Select > 0) { Select--; }
                else { Select = cntItems - 1; }
                break;

            case _KEY::DOWN:
            case _KEY::RIGHT: // вниз
                if (Select < cntItems - 1) { Select++; }
                else { Select = 0; }
                break;

            case _KEY::ENTER: // выбор
                return Select; // вернуть индекс выбранного пункта

            case _KEY::ESC: // отказ от выбора
                return -1; // вернуть несущ. индекс

            } // switch (key)

        } // if (_kbhit())

    } // while (true)
} // Menu()

int SettingsMen(string* Items, int cntItems,
    int Row, int Col, int Select) {
    CursorHide(); // ==> CursorHide(FALSE, 10);
    while (true) // цикл работы меню
    {
        // 1) вывод пунктов меню
        for (int i = 0; i < cntItems; i++)
        {
            SetPos(Row + i, Col); // координаты для i-го пункта меню
            if (i == Select) { SetColor(clSelFon, clSelSym); }
            else { SetColor(clUnselFon, clUnselSym); }
            cout << Items[i]; // вывод i-го пункта меню
        } // for();

        // Восстанавливаем цвет вывода на консоль по умолчанию
        SetColor(clDefFon, clDefSym);

        // 2) обработка нажатий клавиш
        //if (_kbhit()) // проверка на нажатие клавиши
        {
            int key = _getch(); // получить код нажатой клавиши
            switch (key)
            {
            case _KEY::UP:  case _KEY::LEFT: // вверх
                if (Select > 0) { Select--; }
                else { Select = cntItems - 1; }
                break;

            case _KEY::DOWN:
            case _KEY::RIGHT: // вниз
                if (Select < cntItems - 1) { Select++; }
                else { Select = 0; }
                break;

            case _KEY::ENTER: // выбор
                return Select; // вернуть индекс выбранного пункта

            case _KEY::ESC: // отказ от выбора
                return -1; // вернуть несущ. индекс

            } // switch (key)

        } // if (_kbhit())

    } // while (true)
} // SettingsMenu()