// g++ -std=c++17 -o screen_name screen_name.cpp -lX11 -lXmu

#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <iostream>

// Получение полный список всех открытых окон
Window* getWindowList(Display* disp, unsigned long* len) {
    Atom prop = XInternAtom(disp, "_NET_CLIENT_LIST", False);
    Atom type;
    int format;
    unsigned long remain;
    unsigned char* list;

    if (XGetWindowProperty(disp, XDefaultRootWindow(disp), prop, 0, 1024, False,
                           XA_WINDOW, &type, &format, len, &remain, &list) != Success) {
        return nullptr;
    }

    return (Window*)list;
}

// Получение название окон
std::string getWindowName(Display* disp, Window win) {
    Atom netWmName = XInternAtom(disp, "_NET_WM_NAME", False);
    Atom utf8Str = XInternAtom(disp, "UTF8_STRING", False);
    Atom type;
    int format;
    unsigned long len, bytes_after;
    unsigned char* prop = nullptr;

    int status = XGetWindowProperty(
        disp, win, netWmName, 0, (~0L), False, utf8Str,
        &type, &format, &len, &bytes_after, &prop
    );

    std::string name;
    if (status == Success && prop) {
        name = std::string((char*)prop);
        XFree(prop);
    } else {
        name = "<без имени>";
    }

    return name;
}

// Получение открытых
int getWindowWorkspace(Display* disp, Window win) {
    Atom wsAtom = XInternAtom(disp, "_NET_WM_DESKTOP", True);
    Atom type;
    int format;
    unsigned long len, bytes_after;
    unsigned char* prop = nullptr;

    if (XGetWindowProperty(disp, win, wsAtom, 0, 1, False,
                           XA_CARDINAL, &type, &format, &len,
                           &bytes_after, &prop) == Success && prop) {
        int ws = *(unsigned long*)prop;
        XFree(prop);
        return ws;
    }

    return -1; // неизвестно
}

// Получение кардинат окон
void getWindowGeometry(Display* disp, Window win, int& x, int& y, int& width, int& height) {
    XWindowAttributes attr;
    if (XGetWindowAttributes(disp, win, &attr)) {
        Window child;
        int abs_x, abs_y;
        XTranslateCoordinates(disp, win, XDefaultRootWindow(disp), 0, 0, &abs_x, &abs_y, &child);

        x = abs_x;
        y = abs_y;
        width = attr.width;
        height = attr.height;
    } else {
        x = y = width = height = -1;
    }
}


int main() {
    Display* disp = XOpenDisplay(NULL);
    if (!disp) {
        std::cerr << "Не удалось открыть дисплей\n";
        return 1;
    }

    unsigned long len;
    Window* list = getWindowList(disp, &len);
    if (!list) {
        std::cerr << "Не удалось получить список окон\n";
        XCloseDisplay(disp);
        return 1;
    }

    for (unsigned long i = 0; i < len; ++i) {
        std::string name = getWindowName(disp, list[i]);
        int ws = getWindowWorkspace(disp, list[i]);
        int x, y, w, h;
        getWindowGeometry(disp, list[i], x, y, w, h);

        std::cout << "Окно #" << i << ":\n";
        std::cout << "  Название: " << name << "\n";
        std::cout << "  Рабочее пространство: " << ws << "\n";
        std::cout << "  Размер: " << w << "x" << h << " (x=" << x << ", y=" << y << ")\n\n";
    }

    XFree(list);
    XCloseDisplay(disp);
    return 0;
}