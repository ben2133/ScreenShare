
## 🖥️ Screen_Sharre — Захват экрана через HTTP

**Screen_Share** — это C++ приложение, которое позволяет просматривать содержимое экрана или определённого окна в реальном времени через веб-браузер. Использует OpenCV, X11.

---

### 📦 Зависимости

- C++17
    
- OpenCV (v4+)
    
- X11
    

### 📥 Установка зависимостей (Ubuntu / Manjaro)

```bash
# Ubuntu/Debian
sudo apt install libopencv-dev libx11-dev

# Manjaro
sudo pacman -S opencv xorg-server-devel
```

---

### ⚙️ Компиляция

```bash
g++ src/srceen_share.cpp -o screen2 -std=c++17 \
  -I/usr/include/opencv4 \
  -lX11 \
  -lopencv_core -lopencv_highgui -lopencv_imgproc -lopencv_imgcodecs \
  -lftxui-component -lftxui-dom -lftxui-screen
```

---

### 🚀 Запуск

```bash
./screen_share
```

После запуска программа выведет список окон:

```
Окно [ 0 ] ::: ~ Firefox
Окно [ 1 ] ::: ~ Terminal
...
::: Для вывод плного экрана введите команду 'full'
>>> 
```

Введите номер нужного окна или `full` для трансляции всего экрана.

Затем открой браузер и перейди по адресу:  
📡 **[http://localhost:8085](http://localhost:8085)**

На другом компьютер откройте по ip адресу вашего компьютера:
📡 **[http://ваш_ip_адрес:8085](http://ваш_ip_адрес:8085)**

---

### 📸 Особенности

- Захват изображения экрана или окна.
    
- Просмотр изображения в браузере (обновление каждые 300мс).
    
- Простая HTML-страница с автообновлением скриншота.
    
- Поддержка многопоточности для обслуживания клиентов.
    

---

### 📁 Структура кода

|Файл|Описание|
|---|---|
|`srceen_share.cpp`|Основной файл с сервером, логикой захвата и интерфейсом|
|`captureScreen(...)`|Захват изображения через X11 и OpenCV|
|`getWindowList(...)`|Получение всех окон|
|`getWindowName(...)`|Названия окон|
|`getWindowGeometry(...)`|Координаты и размеры окна|
|`sendHttpResponse(...)`|Отправка JPEG изображения клиенту|

---

### 🧪 Пример запроса изображения

```bash
curl http://localhost:8085/screenshot.jpg --output screen.jpg
```


---

### 📄 Лицензия

MIT License

---

### 🤝 Авторы

- [@beorht](https://github.com/beorht)
    

---
