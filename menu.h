#include <iostream>
#include <ncurses.h>
#include <string>
#include <vector>

#include "matrix.h"
#include <string>
#include <unistd.h>

#define CLEAR_CODE "\x1B[2J\x1B[H"
#define NORMALIZE_CONSOLE_CODE "\x1B[2J\x1B[H"

void unconfig() {
    printf("\033[?1003h\n");
    endwin();
    cout << CLEAR_CODE;
}

using namespace std;

class Menu {
private:
    vector<string> menuItems;
    int startx, starty;
    int HEIGHT, WIDTH;
    int n_choices;
    int choice = 1;
    MEVENT event{};
    WINDOW *menuWin;

public:
    Menu(vector<string> items, WINDOW *menuScreen) {
        menuItems = std::move(items);
        HEIGHT = menuItems.size() + 2;
        int maxLength = 0;
        for (const auto &str: menuItems) {
            maxLength = max(maxLength, static_cast<int>(str.size()));
        }
        WIDTH = maxLength + 4;
        n_choices = menuItems.size();
        int parentWindowSizeX, parentWindowSizeY;
        getmaxyx(menuScreen, parentWindowSizeY, parentWindowSizeX);
        startx = (parentWindowSizeX - WIDTH) / 2;
        starty = (parentWindowSizeY - HEIGHT) / 2;
        menuWin = newwin(HEIGHT, WIDTH, starty, startx);
    }

    void run() {
        this->print_menu();
        while (TRUE) {
            int c = wgetch(stdscr);

            if (c == '\n')
                break;

            if (c == KEY_MOUSE) {
                if (getmouse(&event) == OK) {
                    if (event.bstate & BUTTON1_CLICKED) {
                        this->report_choice(event.x - 3, event.y + 1, &choice);
                        if (choice == -1)
                            break;
                        this->print_menu();
                        usleep(100000);
                        if (choice < 6) {
                            unconfig();
                            MatrixCalculator calculator = MatrixCalculator(choice);
                            calculator.run();
                        } else if (choice == 6) {
                            WINDOW *animateWin = newwin(HEIGHT+20, WIDTH+22, starty-12, startx-12);
                            int posX = 1, posY = 1;
                            int directionX = 1, directionY = 1;
                            init_pair(1, COLOR_RED, COLOR_CYAN);
                            init_pair(2, COLOR_CYAN, COLOR_BLUE);
                            wattron(animateWin, COLOR_PAIR(1));
                            box(animateWin, 0, 0);
                            wattron(animateWin, COLOR_PAIR(2));
                            mvwprintw(animateWin, posY + 2, posX + 2, "0");
                            wrefresh(animateWin);

                            for (int i = 0; i < 460; i++) {
                                mvwprintw(animateWin, posY, posX, "0");
                                wrefresh(animateWin);

                                posX += directionX;
                                posY += directionY;

                                if (posX <= 1 || posX >= WIDTH+22 - 2)
                                    directionX *= -1;
                                if (posY <= 1 || posY >= HEIGHT+20 - 2)
                                    directionY *= -1;
                                usleep(10000);
                            }
                            getch();
                            wclear(animateWin);
                            clear();
                            delwin(animateWin);
                            refresh();
                            this->print_menu();
                        } else if (choice == 7) {
                            WINDOW *aboutWin = newwin(HEIGHT+1, WIDTH + 6, starty, startx-3);
                            wborder(aboutWin, '[', ']', '^', '~', '+', '+', '+', '+');

                            init_pair(2, COLOR_CYAN, COLOR_BLUE);
                            init_pair(1, COLOR_BLUE, COLOR_MAGENTA);

                            wattron(aboutWin, COLOR_PAIR(1));
                            mvwprintw(aboutWin, 2, 8, "RGR Programming");
                            wattron(aboutWin, COLOR_PAIR(2));
                            mvwprintw(aboutWin, 4, 5, "University:       OmSTU");
                            mvwprintw(aboutWin, 6, 5, "Group:            PI-231");
                            mvwprintw(aboutWin, 8, 4, "Loginov Semen Alekseevich");
                            wrefresh(aboutWin);
                            getch();
                            wclear(aboutWin);
                            clear();
                            delwin(aboutWin);
                            refresh();
                            this->print_menu();
                        }
                    }
                }
                this->print_menu();
            }
        }
    }

private:
    void print_menu() {
        int x, y, i;
        x = 2;
        y = 1;
        box(menuWin, 0, 0);
        for (i = 0; i < n_choices; ++i) {
            if (choice == i + 1) {
                wattron(menuWin, A_REVERSE);
                mvwprintw(menuWin, y, x, "%s", menuItems[i].c_str());
                wattroff(menuWin, A_REVERSE);
            } else
                mvwprintw(menuWin, y, x, "%s", menuItems[i].c_str());
            y++;
        }
        wrefresh(menuWin);
    }

    void report_choice(int mouse_x, int mouse_y, int *p_choice) {
        int i, j;

        i = startx - 2;
        j = starty + 2;

        for (choice = 0; choice < n_choices; ++choice)
            if (mouse_y == j + choice && mouse_x >= i &&
                mouse_x <= i + menuItems[choice].length()) {
                if (choice == n_choices - 1)
                    *p_choice = -1;
                else
                    *p_choice = choice + 1;
                break;
            }
    }
};
