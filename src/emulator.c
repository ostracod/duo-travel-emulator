
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <curses.h>

#define BLACK_ON_WHITE 1
#define WHITE_ON_CYAN 2
#define DISPLAY_OFFSET_X 2
#define DISPLAY_OFFSET_Y 3

#define true 1
#define false 0

#define DISPLAY_WIDTH 16
#define DISPLAY_HEIGHT 2

#define KEY_CURSOR_LEFT 0
#define KEY_CURSOR_RIGHT 1
#define KEY_CURSOR_UP 2
#define KEY_CURSOR_DOWN 3
#define KEY_CHARACTER_LEFT 4
#define KEY_CHARACTER_RIGHT 5
#define KEY_CHARACTER_UP 6
#define KEY_CHARACTER_DOWN 7
#define KEY_SELECT_OPTION 8
#define KEY_DELETE 9
#define KEY_NEWLINE 10
#define KEY_ESCAPE 11

WINDOW *window;
int32_t windowWidth;
int32_t windowHeight;


void drawBackground() {
    int32_t tempPosY = 0;
    while (tempPosY < windowHeight) {
        int32_t tempPosX = 0;
        while (tempPosX < windowWidth) {
            int8_t tempColor;
            if (tempPosX >= DISPLAY_OFFSET_X && tempPosX < DISPLAY_OFFSET_X + DISPLAY_WIDTH
                    && tempPosY >= DISPLAY_OFFSET_Y && tempPosY < DISPLAY_OFFSET_Y + DISPLAY_HEIGHT) {
                tempColor = BLACK_ON_WHITE;
            } else {
                tempColor = WHITE_ON_CYAN;
            }
            attron(COLOR_PAIR(tempColor));
            mvaddch(tempPosY, tempPosX, ' ');
            attroff(COLOR_PAIR(tempColor));
            tempPosX += 1;
        }
        tempPosY += 1;
    }
    attron(COLOR_PAIR(WHITE_ON_CYAN));
    mvprintw(1, 1, "DUO Travel Emulator");
    attroff(COLOR_PAIR(WHITE_ON_CYAN));
    refresh();
}

void handleResize() {
    int32_t tempWidth;
    int32_t tempHeight;
    getmaxyx(window, tempHeight, tempWidth);
    if (tempWidth == windowWidth && tempHeight == windowHeight) {
        return;
    }
    windowWidth = tempWidth;
    windowHeight = tempHeight;
    drawBackground();
}

int8_t getKey() {
    while (true) {
        int32_t tempKey = getch();
        if (tempKey == KEY_RESIZE) {
            handleResize();
        }
        if (tempKey == KEY_LEFT) {
            return KEY_CURSOR_LEFT;
        }
        if (tempKey == KEY_RIGHT) {
            return KEY_CURSOR_RIGHT;
        }
        if (tempKey == KEY_UP) {
            return KEY_CURSOR_UP;
        }
        if (tempKey == KEY_DOWN) {
            return KEY_CURSOR_DOWN;
        }
        if (tempKey == 'a') {
            return KEY_CHARACTER_LEFT;
        }
        if (tempKey == 'd') {
            return KEY_CHARACTER_RIGHT;
        }
        if (tempKey == 'w') {
            return KEY_CHARACTER_UP;
        }
        if (tempKey == 's') {
            return KEY_CHARACTER_DOWN;
        }
        if (tempKey == ' ') {
            return KEY_SELECT_OPTION;
        }
        if (tempKey == 127 || tempKey == 263) {
            return KEY_DELETE;
        }
        if (tempKey == '\n') {
            return KEY_NEWLINE;
        }
        if (tempKey == 27) {
            return KEY_ESCAPE;
        }
    }
}

void displayCharacter(int8_t posX, int8_t posY, int8_t character) {
    if (posX < 0 || posX >= DISPLAY_WIDTH || posY < 0 || posY >= DISPLAY_HEIGHT) {
        return;
    }
    attron(COLOR_PAIR(BLACK_ON_WHITE));
    mvaddch(posY + DISPLAY_OFFSET_Y, posX + DISPLAY_OFFSET_X, character);
    attroff(COLOR_PAIR(BLACK_ON_WHITE));
}

void displayText(int8_t posX, int8_t posY, int8_t *text) {
    int16_t index = 0;
    while (posY < DISPLAY_HEIGHT) {
        int8_t tempCharacter = text[index];
        if (tempCharacter == 0) {
            break;
        }
        displayCharacter(posX, posY, tempCharacter);
        index += 1;
        posX += 1;
        if (posX >= DISPLAY_WIDTH) {
            posX = 0;
            posY += 1;
        }
    }
}

int main(int argc, const char *argv[]) {
    
    window = initscr();
    noecho();
    curs_set(0);
    keypad(window, true);
    ESCDELAY = 50;
    start_color();
    init_pair(BLACK_ON_WHITE, COLOR_BLACK, COLOR_WHITE);
    init_pair(WHITE_ON_CYAN, COLOR_WHITE, COLOR_CYAN);
    handleResize();
    
    displayText(0, 0, "HELLO");
    getKey();
    
    endwin();
    
}

