from subprocess import Popen
import pyautogui
import win32gui

import numpy as np
import cv2
import time

import pytesseract
from PIL import Image

genshin_program_path = r'C:\Program Files\Genshin Impact\launcher.exe'
ESC_KEY = 27
FRAME_RATE = 1
SLEEP_TIME = 1/FRAME_RATE


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


if __name__ == '__main__':
    Popen([genshin_program_path])
    time.sleep(3)
    capture = WindowCapture("Genshin Impact", FRAME_RATE)
    image_capture = capture.screenshot()

    # cv2.imwrite(r'.\..\images\start.png', image_capture)

    # start_image = cv2.imread(r'.\..\images\start.png')
    # cv2.imshow('open image', start_image)
    # start_image = cv2.cvtColor(start_image, cv2.COLOR_BGR2RGB)
    # img_rgb = Image.frombytes('RGB', start_image.shape[:2], start_image, 'raw', 'BGR', 0, 0)

    print(pytesseract.image_to_string(Image.open(r'.\..\images\start.png')))

    # opencv, mser 모든 글자 상자 -> OCR 적용 -> pyautogui 로 조작

    # mser = cv2.MSER_create()
    # regions, _ = mser.detectRegions(src_transform)



    # while True:
    #
    #     cv2.imshow('frame1', image_capture)
    #
    #     key = cv2.waitKey(1) & 0xFF
    #     if key == ESC_KEY:
    #         break




