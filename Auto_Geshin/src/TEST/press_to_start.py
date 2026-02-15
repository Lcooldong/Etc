from subprocess import Popen
import pyautogui
import win32gui

import numpy as np
import cv2
import time

import pytesseract
from pytesseract import Output

from PIL import Image, ImageGrab
from functools import partial

genshin_program_path = r'C:\Program Files\Genshin Impact\launcher.exe'
ESC_KEY = 27
FRAME_RATE = 1
SLEEP_TIME = 1/FRAME_RATE
game_x = 0
game_y = 0
game_h = 0
start_x = 0

class WindowCapture:
    def __init__(self, window_name, capture_rate):
        self.window_name = window_name
        self.wait_time = 1/capture_rate
        self.frame = self.screenshot()

    def screenshot(self):
        hwnd = win32gui.FindWindow(None, self.window_name)
        if not hwnd:
            raise Exception('Window not found: ' + self.window_name)

        left, top, right, bot = win32gui.GetClientRect(hwnd)
        x, y = win32gui.ClientToScreen(hwnd, (left, top))
        return cv2.cvtColor(
            np.asarray(
                pyautogui.screenshot(
                    region=(x, y,
                            *win32gui.ClientToScreen(hwnd, (right - x, bot - y))))), cv2.COLOR_RGB2BGR)


def capture_all():
    ImageGrab.grab = partial(ImageGrab.grab, all_screens=True)
    img = ImageGrab.grab()
    img.save(r'.\..\..\images\full_screen.png')


if __name__ == '__main__':
    screen_w, screen_h = pyautogui.size()
    print(screen_w, screen_h)

    image = cv2.imread(r'.\..\..\images\full_screen.png')
    gray_image = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)
    ret, thresh1 = cv2.threshold(gray_image, 127, 255, cv2.THRESH_BINARY)

    custom_oem_psm_config = '-l kor+eng --oem 3 --psm 11'
    results = pytesseract.image_to_data(thresh1, config=custom_oem_psm_config, output_type=Output.DICT)
    boxes = pytesseract.image_to_boxes(gray_image, config=custom_oem_psm_config, output_type=Output.DICT)
    print(results)
    print(boxes)

    for i in range(0, len(results["text"])):
        x = results["left"][i]
        y = results["top"][i]

        w = results["width"][i]
        h = results["height"][i]
        text = results["text"][i]
        conf = int(results["conf"][i])

        if conf > 70:
            if text == '게임':
                print("====================================")
                print(x, y, w, h)
                game_x = x
                game_y = y
                game_h = h
                print("====================================")

    while True:
        cv2.imshow('frame1', thresh1)

        key = cv2.waitKey(1) & 0xFF
        if key == ESC_KEY:
            break

