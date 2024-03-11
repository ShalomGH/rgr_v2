#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <sys/ioctl.h>
#include <unistd.h>
#include <termios.h>

#define RESET_CODE   "\033[0m"
#define GREEN_CODE   "\033[32m"      /* Green */
#define MAGENTA_CODE "\033[35m"      /* Magenta */
#define CLEAR_CODE u8"\033[2J\033[1;1H" /* clear console */


int getch() {
    struct termios oldt{}, newt{};
    int ch;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return ch;
}


using namespace std;

static int SCREEN_HEIGHT;
static int SCREEN_WIDTH;

class Color {
public:
    static const char GREEN = ';';
    static const char MAGENTA = '?';
    static const char RESET = '%';
};

static auto previousButtonsTime = std::chrono::system_clock::now();
static const int baseButtonsDelay = 250;

class Buttons {
public:
    enum Keys {
        NOTHING = 0,
        ARROW_UP,
        ARROW_DOWN,
        ENTER,
        ESC,
    };

    static int getKeyCode() {
        auto now = std::chrono::system_clock::now();
        if (std::chrono::duration_cast<std::chrono::milliseconds>(now - previousButtonsTime).count() -
            baseButtonsDelay < 0)
            return NOTHING;
        int input = getch();
        switch (input) {
            case 65: // up
                previousButtonsTime = now;
                return ARROW_UP;
            case 66: // down
                previousButtonsTime = now;
                return ARROW_DOWN;
            case 10: // enter
                previousButtonsTime = now;
                return ENTER;
            case 9: // esc (tab)
                previousButtonsTime = now;
                return ESC;
            default:
                return NOTHING;
        }
    }
};

enum ScreenIds {
    MENU = 0,
    TABLE,
    AUTHOR,
    EXIT,
};
ScreenIds screenId = ScreenIds::MENU;

class Screen {
protected:
    vector<vector<char>> canvas;
    vector<string> menuItems;
    size_t yStart;
    size_t xStart;
public:
    virtual void render() {
        if  (Buttons::getKeyCode() == Buttons::Keys::ESC) {
                screenId = ScreenIds::MENU;
        }
    };

    virtual void update() {
        for (int i = 0; i < SCREEN_HEIGHT; ++i) {
            for (int j = 0; j < SCREEN_WIDTH; ++j)
                if (canvas[i][j]) {
                    switch (canvas[i][j]) {
                        case Color::GREEN:
                            cout << GREEN_CODE << " ";
                            break;
                        case Color::MAGENTA:
                            cout << MAGENTA_CODE << " ";
                            break;
                        case Color::RESET:
                            cout << RESET_CODE << " ";
                            break;
                        default:
                            cout << canvas[i][j];
                            break;
                    }
                } else cout << " ";
            cout << endl;
        }
    }

protected:
    void configureScreen() {
        canvas = generateCanvas();
        fillMenuItems();
        calculateCords();
        drawMenuItems();
    }

    static vector<vector<char>> generateCanvas() {
        vector<vector<char>> canvas1;
        canvas1.resize(SCREEN_HEIGHT);
        for (auto &i: canvas1) i.resize(SCREEN_WIDTH);
        return canvas1;
    }

    virtual void drawMenuItems() {
        for (int i = 0; i < menuItems.size(); i++) {
            for (int j = 0; j < menuItems[i].size(); j++) {
                if (
                        i + yStart < canvas.size() &&
                        i + yStart >= 0 &&
                        j + xStart < canvas[0].size() &&
                        j + xStart >= 0
                        )
                    canvas[i + yStart][j + xStart] = menuItems[i][j];
            }
        }
    }

    virtual void fillMenuItems() {};
private:
    virtual void calculateCords() {
        yStart = (SCREEN_HEIGHT - menuItems.size()) / 2;
        xStart = (SCREEN_WIDTH - menuItems[0].size()) / 2;
    }
};

class Menu : public Screen {
protected:
    void fillMenuItems() override {
        menuItems = {
                "Menu         ",
                "1.  Table    ",
                "2.  Author   ",
                "3.  Exit     ",
        };
    }

private:
    int point = 1;
    size_t yPoint;
    size_t xPoint;
public:
    Menu() {
        configureScreen();
        yPoint = yStart + point;
        xPoint = xStart - 2;
        canvas[yPoint][xPoint] = '*';
    }

    void render() override {
        switch (Buttons::getKeyCode()) {
            case (Buttons::Keys::ARROW_DOWN):
                movePointDown();
                break;
            case (Buttons::Keys::ARROW_UP):
                movePointUp();
                break;
            case (Buttons::Keys::ENTER):
                screenId = static_cast<ScreenIds>(point);
                break;
        }
    }

    void update() override {
        checkPointPosition();
        Screen::update();
    }

    void movePointDown() {
        if (point < 3) point++;
        else point = 1;
        update();
    }

    void movePointUp() {
        if (point > 1) point--;
        else point = 3;
        update();
    }

private:
    void checkPointPosition() {
        if (yStart + point != yPoint) {
            canvas[yPoint][xPoint] = ' ';
            yPoint = yStart + point;
            canvas[yPoint][xPoint] = '-';
        }
    }
};

class Table : public Screen {
private:
    const int N = 12, A = 2, B = 4;

    static double F1(double x) { return pow(2, x) * log10(x) - pow(3, x) * log10(x); }

    static double F2(double x) { return 1 / tan(x); }

protected:
    void fillMenuItems() override {
        vector<vector<double>> XF1F2 = calculateArray();
        double maxF1 = findMax(XF1F2[1]);
        double maxF2 = findMax(XF1F2[2]);
        double minF1 = findMin(XF1F2[1]);
        double minF2 = findMin(XF1F2[2]);
        menuItems = {
                "____________________________________________",
                "|   i    |   x[i]  |    F1[i]  |    F2[i]  |",
                "|________|_________|___________|___________|",
        };
        for (int i = 0; i < N; i++) {
            menuItems.emplace_back("|        |        |            |           |");
            sprintf(menuItems[i + 3].data(), "|   %-2d   | %#7.5f | %#9g | %#9g |", i + 1, XF1F2[0][i], XF1F2[1][i],
                    XF1F2[2][i]);
            menuItems[i + 3][21] = (XF1F2[1][i] == maxF1) ? Color::GREEN : (XF1F2[1][i] == minF1) ? Color::MAGENTA
                                                                                                  : ' ';
            menuItems[i + 3][30] = Color::RESET;
            menuItems[i + 3][33] = (XF1F2[2][i] == maxF2) ? Color::GREEN : (XF1F2[2][i] == minF2) ? Color::MAGENTA
                                                                                                  : ' ';
            menuItems[i + 3][42] = Color::RESET;
        }
        menuItems.emplace_back("|__________________________________________|");
        menuItems.emplace_back(";Max F1: " + to_string(maxF1));
        menuItems.emplace_back(" Max F2: " + to_string(maxF2) + "%");
        menuItems.emplace_back("?Min F1: " + to_string(minF1));
        menuItems.emplace_back(" Min F2: " + to_string(minF2) + "%");
    }

private:
    const double dX = fabs(B - A) / (N - 1.0);
public:
    Table() {
        configureScreen();
    }

private:
    [[nodiscard]] vector<vector<double>> calculateArray() const {
        vector<vector<double>> XF1F2;
        XF1F2.resize(3);
        for (auto &i: XF1F2) i.resize(N);
        XF1F2[0][0] = double(A);
        for (int i = 0; i < N; i++) {
            if (i != 0) XF1F2[0][i] = XF1F2[0][i - 1] + dX;
            XF1F2[1][i] = F1(XF1F2[0][i]);
            XF1F2[2][i] = F2(XF1F2[0][i]);
        }
        return XF1F2;
    }

    [[nodiscard]] double findMax(vector<double> F) const {
        double max1 = F[0];
        for (int i = 0; i < N; i++) if (F[i] > max1)max1 = F[i];
        return max1;
    }

    [[nodiscard]] double findMin(vector<double> F) const {
        double min1 = F[0];
        for (int i = 0; i < N; i++) if (F[i] < min1)min1 = F[i];
        return min1;
    }
};


class Author : public Screen {
protected:
    void fillMenuItems() override {
        menuItems = {
                R"(?~$~$~$~$~$~$~$~$~$~$~$~$~$~$~$~$~$~$~$~$~$~$~$~$~$~)",
                "?$;               RGR Programming                 ?$;",
                "?$; University:       ;OmSTU                      ?$;",
                "?$; Faculty:          ;FiTIKS                     ?$;",
                "?$; Group:            ;PI-231                     ?$;",
                "?$?             Loginov Semen Alekseevich         ?$;",
                R"(?~$~$~$~$~$~$~$~$~$~$~$~$~$~$~$~$~$~$~$~$~$~$~$~$~$~%)",
        };
    }

public:
    Author() {
        configureScreen();
    }
};

static void configure() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    struct winsize size{};
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);
    SCREEN_HEIGHT = size.ws_row - 1;
    SCREEN_WIDTH = size.ws_col;
}

int main() {
    configure();
    Screen *screens[7];
    screens[0] = new Menu;
    screens[1] = new Table;
    screens[2] = new Author;
    ScreenIds preId = ScreenIds::TABLE;
    while (screenId != ScreenIds::EXIT) {
        if (screenId != preId) {
            screens[screenId]->update();
            preId = screenId;
        }
        screens[screenId]->render();
    }
    for (auto &screen: screens) delete screen;
    cout << CLEAR_CODE;
    exit(1);
}