
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <curses.h>

#define BLACK_ON_WHITE 1
#define WHITE_ON_CYAN 2

#define true 1
#define false 0

#define DISPLAY_WIDTH 16
#define DISPLAY_HEIGHT 2

WINDOW *window;
int32_t windowWidth;
int32_t windowHeight;


void drawBackground() {
    int32_t tempPosY = 0;
    while (tempPosY < windowHeight) {
        int32_t tempPosX = 0;
        while (tempPosX < windowWidth) {
            int8_t tempColor;
            if (tempPosX >= 2 && tempPosX < 2 + DISPLAY_WIDTH && tempPosY >= 3 && tempPosY < 3 + DISPLAY_HEIGHT) {
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

int8_t getKey() {
    while (true) {
        int32_t tempKey = getch();
    }
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
    
    getKey();
    
    endwin();
    
}

