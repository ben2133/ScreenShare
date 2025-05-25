# importing the required packages
from socket import socket, AF_INET, SOCK_STREAM
import numpy as np
import pyautogui
import zlib
import cv2

# Создаем сокет
server_socket = socket(AF_INET, SOCK_STREAM)
server_socket.bind(("0.0.0.0", 5000))
server_socket.listen(1)

print("Ожидание подключения клиента...")
conn, addr = server_socket.accept()
print(f"Подключен клиент: {addr}")


try:
    while True:
        # Захватываем скриншот экрана
        img = pyautogui.screenshot()
        frame = np.array(img)
        frame = cv2.cvtColor(frame, cv2.COLOR_RGB2BGR)


        # Сжимаем изображение (можно использовать JPEG-сжатие)
        _, buffer = cv2.imencode('.jpg', frame, [cv2.IMWRITE_JPEG_QUALITY, 70])
        compressed_data = zlib.compress(buffer.tobytes())

        # Отправляем размер данных и сами данные
        size = len(compressed_data)
        conn.sendall(size.to_bytes(4, byteorder='big'))
        conn.sendall(compressed_data)

        if cv2.waitKey(1) == ord('q'):
            break

except KeyboardInterrupt:
    print("Сервер остановлен.")
finally:
    conn.close()
    server_socket.close()


# # Specify resolution
# resolution = (1920, 1080)

# # Specify video codec
# codec = cv2.VideoWriter_fourcc(*"XVID")

# # Specify name of Output file
# filename = "Recording.avi"

# # Specify frames rate. We can choose any 
# # value and experiment with it
# fps = 60.0

# # Creating a VideoWriter object
# out = cv2.VideoWriter(filename, codec, fps, resolution)

# # Create an Empty window
# cv2.namedWindow("Live", cv2.WINDOW_NORMAL)

# # Resize this window
# cv2.resizeWindow("Live", 720, 480)

# while True:
#     # Take screenshot using PyAutoGUI
#     img = pyautogui.screenshot()

#     # Convert the screenshot to a numpy array
#     frame = np.array(img)

#     # Convert it from BGR(Blue, Green, Red) to
#     # RGB(Red, Green, Blue)
#     frame = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)

#     # Write it to the output file
#     out.write(frame)
    
#     # Optional: Display the recording screen
#     cv2.imshow('Live', frame)
    
#     # Stop recording when we press 'q'
#     if cv2.waitKey(1) == ord('q'):
#         break

# # Release the Video writer
# out.release()

# # Destroy all windows
# cv2.destroyAllWindows()