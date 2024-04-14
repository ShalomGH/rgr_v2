#include "menu.h"
#include <ncurses.h>

#define CLEAR_CODE "\x1B[2J\x1B[H"
#define NORMALIZE_CONSOLE_CODE "\x1B[2J\x1B[H"

void config() {
    initscr();
    clear();
    noecho();
    curs_set(0);
    cbreak();
    printf("\033[?1003h\n");
    start_color();
    attron(A_REVERSE);
    refresh();
    attroff(A_REVERSE);
    keypad(stdscr, true);
    mousemask(ALL_MOUSE_EVENTS | BUTTON1_RELEASED, NULL);
}

int main() {
    config();
    Menu menu = Menu({"Addition", "Subtraction", "Multiplication", "Multiplication with num",
                      "Transposition", "Animation", "About", "Exit"},
                     stdscr);
    menu.run();
    cout << NORMALIZE_CONSOLE_CODE;
    endwin();
    cout << CLEAR_CODE;
}
