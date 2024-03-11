//
// Created by Семён Логинов on 11.03.2024.
//

#include "tui.h"
#include <panel.h>
#include <string>

using namespace std;

void tui_win_label(WINDOW *win, char *string, int attrib) {
  if (!string[0])
    return;
  if (attrib == 0)
    attrib = A_NORMAL;
  // Draw borders
  int i = 0, len = 0, j = 0, k = 0, height, width;
  char label[80] = {0};
  getmaxyx(win, height, width);
  mvwhline(win, 2, 1, ACS_HLINE, width - 2);
  char clearw[128] = {' '};
  clearw[width - 1] = 0;
  mvwprintw(win, 1, 1, clearw);

  len = strlen(string);
  if (len > width)
    i = len - width;
  else
    i = 0;
  for (j = i; j < len; j++) {
    label[k] = string[j];
    k++;
  }
  label[k + 1] = '\0';
  if (len > width)
    label[0] = '~';
  wattron(win, attrib);
  mvwprintw(win, 1, (width - strlen(string)) / 2, "%s", label);
  wattroff(win, attrib);
}

curw *tui_new_win(int sy, int sx, int h, int w, char *label, int bckg) {
  curw *obj;
  obj = new curw;

  // Создаем окно для рамки, это уже дочернее окно для фона. Поэтому координаты
  // указываются Относительно родительского окна
  obj->decoration = newwin(h, w, sy, sx);
  wbkgd(obj->decoration, COLOR_PAIR(bckg));
  // box(new->decoration, 0, 0);
  // wborder(new->decoration, '|', '|', '-', '-', '+', '+', '+', '+');
  wborder(obj->decoration, ACS_VLINE, ACS_VLINE, ACS_HLINE, ACS_HLINE,
          ACS_ULCORNER, ACS_URCORNER, ACS_LLCORNER, ACS_LRCORNER);
  int x, y;
  getmaxyx(obj->decoration, y, x);
  obj->overlay = derwin(obj->decoration, y - 4, x - 2, 3, 1);
  wbkgd(obj->overlay, COLOR_PAIR(bckg));
  // scrollok(new->overlay, TRUE);
  obj->panel = new_panel(obj->decoration);
  tui_win_label(obj->decoration, label, 0);
  update_panels();
  doupdate();
  return obj;
}

void tui_move_panel(curw *win, int ch) {
  int begy, begx, maxx, maxy, x, y;
  getbegyx(panel_window(win->panel), begy, begx);
  getmaxyx(panel_window(win->panel), maxy, maxx);
  getmaxyx(stdscr, y, x);
  switch (ch) {
  case KEY_UP:
    if ((begy - 1) >= 0)
      begy--;
    break;
  case KEY_DOWN:
    if (((begy + 1) + maxy) <= y)
      begy++;
    break;
  case KEY_LEFT:
    if ((begx - 1) >= 0)
      begx--;
    break;
  case KEY_RIGHT:
    if (((begx + 1) + maxx) <= x)
      begx++;
    break;
  }
  move_panel(win->panel, begy, begx);
}
