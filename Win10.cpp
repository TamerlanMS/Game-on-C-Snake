#include <iostream>
#include "Win10.h"    // из текущего каталога, папки проекта


// перегруженные функции дл€ смена цвета
void SetColor(int colBkgr, int colSym)
{
    int wAttributes = colBkgr * 16 + colSym;
    HANDLE hOUTPUT = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hOUTPUT, wAttributes);
}

void SetColor(WORD wAttributes)
{
    HANDLE hOUTPUT = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hOUTPUT, wAttributes);
}


// ‘ункци€ дл€ установки позиции курсора вывода
void SetPos(int Row, int Col)
{
    COORD cd;
    cd.X = Col;
    cd.Y = Row;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cd);
}

// ‘ункци€ дл€ скрыти€ каретки
void CursorHide(BOOL bVisible, DWORD dwSize)
{
    HANDLE hOUTPUT = GetStdHandle(STD_OUTPUT_HANDLE); // Get console handle
    // Hide console cursor
    CONSOLE_CURSOR_INFO cursorInfo;
    cursorInfo.dwSize = dwSize;
    cursorInfo.bVisible = bVisible;
    SetConsoleCursorInfo(hOUTPUT, &cursorInfo);
}

void start(int nFont, int Lines, int Columns)
{
    CONSOLE_FONT_INFOEX cfon{};
    ZeroMemory(&cfon, sizeof(CONSOLE_FONT_INFOEX));
    cfon.cbSize = sizeof(CONSOLE_FONT_INFOEX);
    cfon.nFont = nFont;
    cfon.FontFamily = 0;
    SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), 0, &cfon);

    char str[100];
    sprintf_s(str, "mode con cols=%d lines=%d", Columns, Lines);
    system(str);
    //system("mode con cols=80 lines=50");
    system("cls");
    CursorHide(0, 10);
}

// ‘-ци€ определени€ видимого размера консольного окна
void GetConsoleSize(int& Width, int& Height)
{
    CONSOLE_SCREEN_BUFFER_INFOEX sb{ sizeof(CONSOLE_SCREEN_BUFFER_INFOEX) };
    GetConsoleScreenBufferInfoEx(GetStdHandle(STD_OUTPUT_HANDLE), &sb);
    Width = sb.srWindow.Right - sb.srWindow.Left + 1;
    Height = sb.srWindow.Bottom - sb.srWindow.Top + 1;
    /*
    std::cout << "CONSOLE_SCREEN_BUFFER_INFOEX::" << std::endl
      << " cbSize   = " << sb.cbSize << std::endl
      << " dwSize.X = " << sb.dwSize.X << std::endl
      << " dwSize.Y = " << sb.dwSize.Y << std::endl
      << " dwCursorPosition.X = " << sb.dwCursorPosition.X << std::endl
      << " dwCursorPosition.Y = " << sb.dwCursorPosition.Y << std::endl
      << " wAttributes     = " << sb.wAttributes << std::endl
      << " srWindow.Left   = " << sb.srWindow.Left   << std::endl
      << " srWindow.Top    = " << sb.srWindow.Top    << std::endl
      << " srWindow.Right  = " << sb.srWindow.Right  << std::endl
      << " srWindow.Bottom = " << sb.srWindow.Bottom << std::endl
      << " dwMaximumWindowSize.X = " << sb.dwMaximumWindowSize.X << std::endl
      << " dwMaximumWindowSize.Y = " << sb.dwMaximumWindowSize.Y << std::endl
      << " wPopupAttributes = " << sb.wPopupAttributes << std::endl
      << " bFullscreenSupported = " << sb.bFullscreenSupported << std::endl;
    std::cout << " ColorTable[16]:\n    N: A  R  G  B\n";
    for(size_t i = 0; i < sizeof(sb.ColorTable)/sizeof(sb.ColorTable[0]); i++) {
      unsigned char *pdw = (unsigned char*)(sb.ColorTable+i);
      printf("   %2d: %02X %02X %02X %02X\n", i, pdw[3], pdw[0], pdw[1], pdw[2] );
      //std::cout << std::hex << sb.ColorTable[i] << " ";
    }
    std::cout << std::dec << std::endl;
    */
}

// ‘-ци€ установки размера консольного окна
void SetConsoleSize(int Width, int Height)
{
    HANDLE hCon = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFOEX sb{ sizeof(CONSOLE_SCREEN_BUFFER_INFOEX) };
    GetConsoleScreenBufferInfoEx(hCon, &sb);
    sb.dwSize.X = Width;
    sb.dwSize.Y = Height;
    sb.srWindow.Left = sb.srWindow.Top = 0;
    sb.srWindow.Right = ((Width > sb.dwMaximumWindowSize.X) ? sb.dwMaximumWindowSize.X : Width);
    sb.srWindow.Bottom = ((Height > sb.dwMaximumWindowSize.Y) ? sb.dwMaximumWindowSize.Y : Height);
    SetConsoleScreenBufferInfoEx(hCon, &sb);
}

void SetConsoleSize(SIZE szConsWin) {
    SetConsoleSize(szConsWin.cx, szConsWin.cy);
}



//----------------------------------------------------
// ‘ункции работы с манипул€тором типа "мышь"
//

// ассинхронна€ (не блокирующа€) проверка нажати€ левой кнопки мыши
bool isLButtonDown() {
    SHORT state = GetAsyncKeyState(VK_LBUTTON);
    return (state & 0x8000) != 0;
}

// ассинхронна€ (не блокирующа€) проверка нажати€ правой кнопки мыши
bool isRButtonDown() {
    SHORT state = GetAsyncKeyState(VK_RBUTTON);
    return (state & 0x8000) != 0;
}

// ассинхронна€ (не блокирующа€) проверка нажати€ средней кнопки мыши
bool isMButtonDown() {
    SHORT state = GetAsyncKeyState(VK_MBUTTON);
    return (state & 0x8000) != 0;
}

// получить текущие координаты указател€ мыши в пикселах экрана
//   относительно видимой области консольного окна
// возвращает структуру типа POINT с экранными координатами в пикселах
POINT GetMousePoint() {
    POINT pt;
    GetCursorPos(&pt);
    HWND my_consoleH = GetConsoleWindow();
    ScreenToClient(my_consoleH, &pt);
    return pt;
}

// получить текущие координаты указател€ мыши в консольных координатах
//   относительно видимой области консольного окна
// возвращает структуру типа POINT с консольными координатами
POINT GetMouseCoord() {
    POINT res, mp = GetMousePoint();
    CONSOLE_FONT_INFOEX cfon{ sizeof(CONSOLE_FONT_INFOEX) };
    GetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), 0, &cfon);
    res.x = mp.x / cfon.dwFontSize.X;
    res.y = mp.y / cfon.dwFontSize.Y;
    return res;
}

// получить текущие координаты указател€ мыши в консольных координатах
//   относительно видимой области консольного окна
// возвращает координаты в раздельных переменных дл€ строки и столбца
void GetMouseCoord(int& Row, int& Col) {
    POINT mcoord = GetMouseCoord();
    Row = mcoord.y;
    Col = mcoord.x;
}
