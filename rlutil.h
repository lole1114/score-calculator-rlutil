#ifndef RLUTIL_H
#define RLUTIL_H

/*
 rlutil.h - Console utility library
 Public domain / MIT-like usage
 Works on Windows / Linux / macOS (basic features)
*/

#ifdef _WIN32
    #include <windows.h>
    #include <conio.h>
#else
    #include <termios.h>
    #include <unistd.h>
    #include <stdio.h>
#endif

#include <stdlib.h>

#define BLACK           0
#define BLUE            1
#define GREEN           2
#define CYAN            3
#define RED             4
#define MAGENTA         5
#define BROWN           6
#define LIGHTGRAY       7
#define DARKGRAY        8
#define LIGHTBLUE       9
#define LIGHTGREEN      10
#define LIGHTCYAN       11
#define LIGHTRED        12
#define LIGHTMAGENTA    13
#define YELLOW          14
#define WHITE           15

#define KEY_ESCAPE 27
#define KEY_ENTER  13
#define KEY_UP     14
#define KEY_DOWN   15
#define KEY_LEFT   16
#define KEY_RIGHT  17

#ifdef _WIN32

static HANDLE hConsole = NULL;

static void rlutil_init() {
    if (!hConsole) hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
}

static void setColor(int color) {
    rlutil_init();
    SetConsoleTextAttribute(hConsole, color);
}

static void setBackgroundColor(int color) {
    rlutil_init();
    CONSOLE_SCREEN_BUFFER_INFO info;
    GetConsoleScreenBufferInfo(hConsole, &info);
    SetConsoleTextAttribute(hConsole, (info.wAttributes & 0x0F) | (color << 4));
}

static void cls() {
    system("cls");
}

static void locate(int x, int y) {
    rlutil_init();
    COORD coord;
    coord.X = x - 1;
    coord.Y = y - 1;
    SetConsoleCursorPosition(hConsole, coord);
}

static int getkey() {
    int c = _getch();
    if (c == 224 || c == 0) {
        c = _getch();
        switch (c) {
            case 72: return KEY_UP;
            case 80: return KEY_DOWN;
            case 75: return KEY_LEFT;
            case 77: return KEY_RIGHT;
            default: return c;
        }
    }
    return c;
}

#else   // UNIX-like systems

static struct termios orig_termios;

static void disableRawMode() {
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
}

static void enableRawMode() {
    tcgetattr(STDIN_FILENO, &orig_termios);
    atexit(disableRawMode);
    struct termios raw = orig_termios;
    raw.c_lflag &= ~(ECHO | ICANON);
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

static void setColor(int color) {
    printf("\033[%dm", 30 + (color % 8));
}

static void setBackgroundColor(int color) {
    printf("\033[%dm", 40 + (color % 8));
}

static void cls() {
    printf("\033[2J\033[H");]()]()
