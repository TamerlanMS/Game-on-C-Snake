#include <iostream>
#include <string>
#include "Win10.h"
using namespace std;

// ����� ��� ��������� ������ ����
int clSelFon = COLOR::green; // ��� ����
int clSelSym = COLOR::black; // ��� ��������

// ����� ��� ���������� ������� ����
int clUnselFon = COLOR::blue;         // ��� ����
int clUnselSym = COLOR::light_yellow; // ��� ��������

// ���� ������� �� ���������
int clDefFon = COLOR::black; // ������ ���
int clDefSym = COLOR::white; // ����� �����


int Menu(string* Items, int cntItems,
    int Row, int Col, int Select)
{
    CursorHide(); // ==> CursorHide(FALSE, 10);
    while (true) // ���� ������ ����
    {
        // 1) ����� ������� ����
        for (int i = 0; i < cntItems; i++)
        {
            SetPos(Row + i, Col); // ���������� ��� i-�� ������ ����
            if (i == Select) { SetColor(clSelFon, clSelSym); }
            else { SetColor(clUnselFon, clUnselSym); }
            cout << Items[i]; // ����� i-�� ������ ����
        } // for();

        // ��������������� ���� ������ �� ������� �� ���������
        SetColor(clDefFon, clDefSym);

        // 2) ��������� ������� ������
        //if (_kbhit()) // �������� �� ������� �������
        {
            int key = _getch(); // �������� ��� ������� �������
            switch (key)
            {
            case _KEY::UP:  case _KEY::LEFT: // �����
                if (Select > 0) { Select--; }
                else { Select = cntItems - 1; }
                break;

            case _KEY::DOWN:
            case _KEY::RIGHT: // ����
                if (Select < cntItems - 1) { Select++; }
                else { Select = 0; }
                break;

            case _KEY::ENTER: // �����
                return Select; // ������� ������ ���������� ������

            case _KEY::ESC: // ����� �� ������
                return -1; // ������� �����. ������

            } // switch (key)

        } // if (_kbhit())

    } // while (true)
} // Menu()

int SettingsMen(string* Items, int cntItems,
    int Row, int Col, int Select) {
    CursorHide(); // ==> CursorHide(FALSE, 10);
    while (true) // ���� ������ ����
    {
        // 1) ����� ������� ����
        for (int i = 0; i < cntItems; i++)
        {
            SetPos(Row + i, Col); // ���������� ��� i-�� ������ ����
            if (i == Select) { SetColor(clSelFon, clSelSym); }
            else { SetColor(clUnselFon, clUnselSym); }
            cout << Items[i]; // ����� i-�� ������ ����
        } // for();

        // ��������������� ���� ������ �� ������� �� ���������
        SetColor(clDefFon, clDefSym);

        // 2) ��������� ������� ������
        //if (_kbhit()) // �������� �� ������� �������
        {
            int key = _getch(); // �������� ��� ������� �������
            switch (key)
            {
            case _KEY::UP:  case _KEY::LEFT: // �����
                if (Select > 0) { Select--; }
                else { Select = cntItems - 1; }
                break;

            case _KEY::DOWN:
            case _KEY::RIGHT: // ����
                if (Select < cntItems - 1) { Select++; }
                else { Select = 0; }
                break;

            case _KEY::ENTER: // �����
                return Select; // ������� ������ ���������� ������

            case _KEY::ESC: // ����� �� ������
                return -1; // ������� �����. ������

            } // switch (key)

        } // if (_kbhit())

    } // while (true)
} // SettingsMenu()