/* compiler command 
 g++ screen.cpp -o screen \
  -I/usr/include/opencv4 \
  -lX11 \
  -lopencv_core \
  -lopencv_highgui \
  -lopencv_imgproc \
  -lopencv_imgcodecs  # Добавьте эту библиотеку!
 * 
 */

#include <opencv2/opencv.hpp>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>
#include <iostream>
#include <cstdint>
#include <thread>
#include <memory>

// Захват экрана
cv::Mat captureScreen(Display* disp, int width, int height, int x, int y);

// Получение список окон 
Window* getWindowList(Display* disp, unsigned long* len);

// Получение название окок
std::string getWindowName(Display* disp, Window win); 

// Получение открытых окон
int getWindowWorkspace(Display* disp, Window win);

// Получение размер и кардинат окон
void getWindowGeometry(Display* disp, Window win, int& x, int& y, int& width, int& height);


// Главная функция
int main(int argc, char const *argv[]) 
{
    Display* display = XOpenDisplay(nullptr);
    if (!display) 
    {
        std::cerr << "Не удалось открыть дисплей\n";
        return 1;
    }

    unsigned long len;
    Window* list = getWindowList(display, &len);
    if (!list) 
    {
        std::cerr << "Не удалось получить список окон\n";
        XCloseDisplay(display);
        return 1;
    }

    for (unsigned long i = 0; i < len; ++i) 
    {
        std::string name = getWindowName(display, list[i]);
        int ws = getWindowWorkspace(display, list[i]);
        int x, y, w, h;
        getWindowGeometry(display, list[i], x, y, w, h);

        std::cout << "Окно #" << i << ":\n";
        std::cout << "  Название: " << name << "\n";
        std::cout << "  Рабочее пространство: " << ws << "\n";
        std::cout << "  Размер: " << w << "x" << h << " (x=" << x << ", y=" << y << ")\n\n";
    }
    
    // while (true) 
    // {
    //     // Захватываем экран
    //     cv::Mat screen = captureScreen();

    //     // Сохраняем в файл
    //     //cv::imwrite("screenshot.png", screen);
    //     std::cout << "Скриншот сохранен в screenshot.png" << std::endl;

    //     // Показываем в окне (опционально)
    //     cv::imshow("Screen", screen);
    //     char key = cv::waitKey(1000);
    //     if (key == 27) break;  // 27 = ESC    
    // }

    XFree(list);
    cv::destroyAllWindows();

    return 0;
}

/* ~~~ ****   Работа с получением данных об окон   **** ~~~
 
int main() 
{
    Display* disp = XOpenDisplay(NULL);
    if (!disp) 
    {
        std::cerr << "Не удалось открыть дисплей\n";
        return 1;
    }

    unsigned long len;
    Window* list = getWindowList(disp, &len);
    if (!list) 
    {
        std::cerr << "Не удалось получить список окон\n";
        XCloseDisplay(disp);
        return 1;
    }

    for (unsigned long i = 0; i < len; ++i) 
    {
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

*/

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
void getWindowGeometry(Display* disp, Window win, int& x, int& y, int& width, int& height) 
{
    XWindowAttributes attr;
    if (XGetWindowAttributes(disp, win, &attr)) 
    {
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