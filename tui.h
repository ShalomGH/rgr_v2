#ifndef RGR_V2_TUI_H
#define RGR_V2_TUI_H

#include <panel.h>

struct cursed_window {
  WINDOW *background;
  WINDOW *decoration;
  WINDOW *overlay;
  PANEL *panel;
};

typedef struct cursed_window curw;

curw *tui_new_win(int sy, int sx, int h, int w, char *label, int bckg);
void tui_move_panel(curw *win, int ch);
void tui_win_label(WINDOW *win, char *string, int attrib);

#endif // RGR_V2_TUI_H
