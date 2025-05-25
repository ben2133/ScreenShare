from socket import socket, AF_INET, SOCK_STREAM
from zlib import decompress
import numpy as np
import cv2


# Подключаемся к серверу
client_socket = socket(AF_INET, SOCK_STREAM)
client_socket.connect(("localhost", 5000))

cv2.namedWindow('Screen Stream', cv2.WINDOW_NORMAL)
cv2.resizeWindow('Screen Stream', 1920, 1080)

try:
    while True:
        # Получаем размер данных
        size_data = client_socket.recv(4)
        if not size_data:
            break
        size = int.from_bytes(size_data, byteorder='big')

        # Получаем сами данные
        compressed_data = b''
        while len(compressed_data) < size:
            packet = client_socket.recv(size - len(compressed_data))
            if not packet:
                break
            compressed_data += packet

        # Распаковываем и декодируем изображение
        buffer = decompress(compressed_data)
        frame = cv2.imdecode(np.frombuffer(buffer, dtype=np.uint8), cv2.IMREAD_COLOR)

        # Показываем изображение
        cv2.imshow('Screen Stream', frame)
        if cv2.waitKey(1) == ord('q'):
            break

except KeyboardInterrupt:
    print("Клиент остановлен.")
finally:
    client_socket.close()
    cv2.destroyAllWindows()