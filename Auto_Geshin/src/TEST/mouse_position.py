import pyautogui
import time

if __name__ == '__main__':
    while True:
        mouse_x, mouse_y = pyautogui.position()
        print(mouse_x, mouse_y)
        time.sleep(0.1)