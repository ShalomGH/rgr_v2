#include <iostream>
#include <ncurses.h>
#include <string>

#define WIDTH 30
#define HEIGHT 10

int startx = 0;
int starty = 0;

char *choices[] = {
        "Choice 1", "Choice 2", "Choice 3", "Choice 4", "Exit",
};

int n_choices = sizeof(choices) / sizeof(char *);

void print_menu(WINDOW *menu_win, int highlight);

void report_choice(int mouse_x, int mouse_y, int *p_choice);

void draw_mouse(WINDOW *mouse_win, MEVENT event) {
    werase(mouse_win);
    mvwprintw(mouse_win, 1, 1, "x = %d", event.x);
    mvwprintw(mouse_win, 3, 1, "y = %d", event.y);
    wrefresh(mouse_win);
}

int main() {
    int c, choice = 0;
    WINDOW *menu_win;
    WINDOW *mouse_win;
    MEVENT event;

    /* Initialize curses */
    initscr();
    clear();
    noecho();
    cbreak(); // Line buffering disabled. pass on everything
    printf("\033[?1003h\n");

    /* Try to put the window in the middle of screen */
    startx = (80 - WIDTH) / 2;
    starty = (24 - HEIGHT) / 2;

    attron(A_REVERSE);
    mvprintw(23, 1, "Click on Exit to quit (Works best in a virtual console)");
    refresh();
    attroff(A_REVERSE);

    /* Print the menu for the first time */
    menu_win = newwin(HEIGHT, WIDTH, starty, startx);
    mouse_win = newwin(4, 7, 1, 1);
    box(mouse_win, 1, 1);
    wrefresh(mouse_win);
    print_menu(menu_win, 1);
    /* Get all the mouse events */
    keypad(stdscr, true);
    mousemask(ALL_MOUSE_EVENTS | BUTTON1_RELEASED, NULL);
    int b = 0;
    while (1) {
        int c = wgetch(stdscr);

        // Exit the program on new line fed
        if (c == '\n')
            break;

        if (c == KEY_MOUSE) {
            if (getmouse(&event) == OK) {
                // MEVENT event;
                if (event.bstate & BUTTON1_CLICKED) {
                    report_choice(event.x + 1, event.y + 1, &choice);
                    draw_mouse(mouse_win, event);
                    if (choice == -1) // Exit
                        break;
                    mvprintw(22, 1, "Choice made is : %d String Chosen is \"%10s\"",
                             choice, choices[choice - 1]);
                    refresh();
                }
            }
            print_menu(menu_win, choice);
        }

        b++;
    }
    printf("\033[?1003l\n");
    endwin();
    std::cout << "dadad " << b;
    return 0;
}

void print_menu(WINDOW *menu_win, int highlight) {
    int x, y, i;

    x = 2;
    y = 2;
    box(menu_win, 0, 0);
    for (i = 0; i < n_choices; ++i) {
        if (highlight == i + 1) {
            wattron(menu_win, A_REVERSE);
            mvwprintw(menu_win, y, x, "%s", choices[i]);
            wattroff(menu_win, A_REVERSE);
        } else
            mvwprintw(menu_win, y, x, "%s", choices[i]);
        ++y;
    }
    wrefresh(menu_win);
}

/* Report the choice according to mouse position */
void report_choice(int mouse_x, int mouse_y, int *p_choice) {
    int i, j, choice;

    i = startx + 2;
    j = starty + 3;

    for (choice = 0; choice < n_choices; ++choice)
        if (mouse_y == j + choice && mouse_x >= i &&
            mouse_x <= i + strlen(choices[choice])) {
            if (choice == n_choices - 1)
                *p_choice = -1;
            else
                *p_choice = choice + 1;
            break;
        }
}
