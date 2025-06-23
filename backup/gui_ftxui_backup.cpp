// g++ -std=c++17 -o tui interfeyc.cpp -lftxui-component -lftxui-dom -lftxui-screen
// g++ gui.cpp -o screen2 -I/usr/include/opencv4 -lX11 -lopencv_core -lopencv_highgui  -lopencv_imgproc -lopencv_imgcodecs -lftxui-component -lftxui-dom -lftxui-screen

#include <functional>  // for function
#include <iostream>  // for basic_ostream::operator<<, operator<<, endl, basic_ostream, basic_ostream<>::__ostream_type, cout, ostream
#include <cstdint>
#include <memory>  // for shared_ptr, __shared_ptr_access
#include <vector>  // for vector
#include <string>    // for string, basic_string, allocator

#include "ftxui/component/captured_mouse.hpp"      // for ftxui
#include "ftxui/component/component.hpp"           // for Radiobox, Renderer
#include "ftxui/component/component_base.hpp"      // for ComponentBase
#include "ftxui/component/screen_interactive.hpp"  // for ScreenInteractive
#include "ftxui/component/component_options.hpp"   // for MenuOption
#include "ftxui/dom/elements.hpp"  // for operator|, Element, size, border, frame, HEIGHT, LESS_THAN

#include <opencv2/opencv.hpp>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>

// Захват экрана
cv::Mat captureScreen(Display* disp, int width, int height, int x, int y);

// Получение список окон 
Window* getWindowList(Display* disp, unsigned long* len);

// Получение название окок
std::string getWindowName(Display* disp, Window win); 

// Получение открытых окон
int getWindowWorkspace(Display* disp, Window win);

// Получение размер и кардинат окон
std::vector<int> getWindowGeometry(Display* disp, Window win);


int main(int argc, char const *argv[])
{
    // The data
    std::string winWidth;
    std::string winHeight;

    // Открываем окно
    Display* display = XOpenDisplay(nullptr);
    if (!display) 
    {
        std::cerr << "Не удалось открыть дисплей\n";
        return 1;
    }

    /* *********************
     * Список открытых окон
     * ********************/

    // List of window
    std::vector<std::string> windowList;
    int selectedWindow = 0;

    // Getting windows list
    unsigned long len;
    Window* list = getWindowList(display, &len);
    if (!list) 
    {
        std::cerr << "Не удалось получить список окон\n";
        XCloseDisplay(display);
        return 1;
    }

    // Gettings windows name
    for (unsigned long i = 0; i < len; ++i) 
    {
        std::string name = getWindowName(display, list[i]);
        int ws = getWindowWorkspace(display, list[i]);
        std::vector<int> windowParametr = getWindowGeometry(display, list[i]);

        windowList.push_back(name + " |" + std::to_string(windowParametr[2]) + "x" + std::to_string(windowParametr[3]));

        // std::cout << "Окно #" << i << ":\n";
        // std::cout << "  Название: " << name << "\n";
        // std::cout << "  Рабочее пространство: " << ws << "\n";
        // std::cout << "  Размер: " << w << "x" << h << " (x=" << x << ", y=" << y << ")\n\n";
    }

    std::shared_ptr<ftxui::ComponentBase> windowMenu = ftxui::Radiobox( &windowList, &selectedWindow );
    
    // * Окно со списком окон --------------------------------------------------------------------------------------------------------
    ftxui::Component WindowMenu = ftxui::Renderer( windowMenu, [&] 
    {
        return ftxui::vbox({
            ftxui::text( "Список окон:" ) | ftxui::bold | ftxui::color(ftxui::Color::CyanLight),
            ftxui::separator(),
            windowMenu->Render()
        }) | ftxui::vscroll_indicator | ftxui::frame | ftxui::border | ftxui::size(ftxui::HEIGHT, ftxui::LESS_THAN, 100);
    });

    /* **********************
     * Параметры размера окон
     * *********************/
    // winWidth = std::to_string(selectedWindow);

    // Input Objects
    ftxui::Component windowWidth   = ftxui::Input(&winWidth, "Width");
    ftxui::Component windowHeight  = ftxui::Input(&winHeight, "Height");
    
    ftxui::Component component = ftxui::Container::Vertical({
        windowWidth, windowHeight 
    });

    // * Окно со параметрами --------------------------------------------------------------------------------------------------------
    ftxui::Component WindowSizeInput = ftxui::Renderer(component, [&] {
        int w = selectedWindow;
        return ftxui::vbox({
            ftxui::text( std::to_string(w) ), ftxui::separator(),
            ftxui::text( "Параметры экрана:" ) | ftxui::bold | ftxui::color(ftxui::Color::CyanLight),
            ftxui::separator(),
            ftxui::hbox( ftxui::text( "-> Width  : " ), windowWidth->Render() ),
            ftxui::hbox( ftxui::text( "-> Height : " ), windowHeight->Render() ),
        }) | ftxui::frame | ftxui::border | ftxui::size(ftxui::HEIGHT, ftxui::LESS_THAN, 100);
    });




    // Global GUI
    ftxui::Component global = ftxui::Container::Vertical({ WindowMenu, WindowSizeInput });

    auto screen = ftxui::ScreenInteractive::TerminalOutput();
    screen.Loop(global);
    return 0;
}


// Захват экрана через X11 и конвертация в OpenCV-матрицу
cv::Mat captureScreen(Display* disp, int width, int height, int x, int y) 
{
    // Display* display = XOpenDisplay(":0");
    Window root = DefaultRootWindow(disp);

    // Получаем размеры экрана
    XWindowAttributes attributes;
    XGetWindowAttributes(disp, root, &attributes);

    
    //750x487 (x=1158, y=59)
    // Создаем XImage
    // XImage* img = XGetImage(display, root, 0, 0, width, height, AllPlanes, ZPixmap);
        
    XImage* img = XGetImage(disp, root, x, y, width, height, AllPlanes, ZPixmap);
    if (!img) 
    {
        XCloseDisplay(disp);
        throw std::runtime_error("Не удалось создать XImage");
    }

    // Конвертируем XImage в OpenCV Mat (BGR формат)
    cv::Mat mat(height, width, CV_8UC4, img->data);
    cv::Mat bgrMat;
    cv::cvtColor(mat, bgrMat, cv::COLOR_BGRA2BGR);  // Убираем альфа-канал

    // Освобождаем ресурсы
    XDestroyImage(img);
    XCloseDisplay(disp);

    return bgrMat;
}

// Получение полный список всех открытых окон
Window* getWindowList(Display* disp, unsigned long* len) 
{
    Atom prop = XInternAtom(disp, "_NET_CLIENT_LIST", False);
    Atom type;
    int format;
    unsigned long remain;
    unsigned char* list;

    if (XGetWindowProperty(disp, XDefaultRootWindow(disp), prop, 0, 1024, False,
                           XA_WINDOW, &type, &format, len, &remain, &list) != Success) 
    {
        return nullptr;
    }

    return (Window*)list;
}

// Получение название окон
std::string getWindowName(Display* disp, Window win) 
{
    Atom netWmName  = XInternAtom(disp, "_NET_WM_NAME", False);
    Atom utf8Str    = XInternAtom(disp, "UTF8_STRING", False);
    Atom type;
    int format;
    unsigned long len, bytes_after;
    unsigned char* prop = nullptr;

    int status = XGetWindowProperty(
        disp, win, netWmName, 0, (~0L), False, utf8Str,
        &type, &format, &len, &bytes_after, &prop
    );

    std::string name;
    if (status == Success && prop) 
    {
        name = std::string((char*)prop);
        XFree(prop);
    } else 
    {
        name = "<без имени>";
    }

    return name;
}

// Получение открытых
int getWindowWorkspace(Display* disp, Window win) 
{
    Atom wsAtom = XInternAtom(disp, "_NET_WM_DESKTOP", True);
    Atom type;
    int format;
    unsigned long len, bytes_after;
    unsigned char* prop = nullptr;

    if (XGetWindowProperty(disp, win, wsAtom, 0, 1, False,
                           XA_CARDINAL, &type, &format, &len,
                           &bytes_after, &prop) == Success && prop) 
    {
        int ws = *(unsigned long*)prop;
        XFree(prop);
        return ws;
    }

    return -1; // неизвестно
}

// Получение кардинат окон
std::vector<int> getWindowGeometry(Display* disp, Window win) 
{
    int x, y, width, height;
    XWindowAttributes attr;
    if (XGetWindowAttributes(disp, win, &attr)) 
    {
        Window child;
        int abs_x, abs_y;
        XTranslateCoordinates(disp, win, XDefaultRootWindow(disp), 0, 0, &abs_x, &abs_y, &child);

        x       =  abs_x;
        y       =  abs_y;
        width   =  attr.width;
        height  =  attr.height;
    } else {
        x = y = width = height = -1;
    }

    return { x, y, width, height };
}